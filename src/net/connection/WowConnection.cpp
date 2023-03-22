#include "net/connection/WowConnection.hpp"
#include "net/connection/WowConnectionNet.hpp"
#include "net/connection/WowConnectionResponse.hpp"
#include <common/DataStore.hpp>
#include <common/Time.hpp>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <storm/Thread.hpp>
#include <algorithm>
#include <cstring>
#include <new>

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#if defined(WHOA_SYSTEM_WIN)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

uint64_t WowConnection::s_countTotalBytes;
int32_t WowConnection::s_destroyed;
int32_t WowConnection::s_lagTestDelayMin;
WowConnectionNet* WowConnection::s_network;
ATOMIC32 WowConnection::s_numWowConnections;
bool (*WowConnection::s_verifyAddr)(const NETADDR*);

WowConnection::SENDNODE::SENDNODE(void* data, int32_t size, uint8_t* buf, bool raw) : TSLinkedNode<WowConnection::SENDNODE>() {
    if (data) {
        this->data = buf;
    }

    if (raw) {
        memcpy(this->data, data, size);
        this->size = size;
    } else {
        uint32_t headerSize = size > 0x7FFF ? 3 : 2;

        if (!data) {
            this->data = &buf[-headerSize];
        }

        auto headerBuf = static_cast<uint8_t*>(this->data);

        // Write 2 or 3 byte data size value to header in big endian order
        if (size > 0x7FFF) {
            headerBuf[0] = ((size >> (8 * 2)) & 0xff) | 0x80;
            headerBuf[1] = (size >> (8 * 1)) & 0xff;
            headerBuf[2] = (size >> (8 * 0)) & 0xff;
        } else {
            headerBuf[0] = (size >> (8 * 1)) & 0xff;
            headerBuf[1] = (size >> (8 * 0)) & 0xff;
        }

        if (data) {
            memcpy(static_cast<uint8_t*>(&this->data[headerSize]), data, size);
        }

        this->size = size + headerSize;
    }

    this->datasize = size;
    this->offset = 0;
    this->allocsize = 0;

    memcpy(this->header, this->data, std::min(this->size, 8u));
}

int32_t WowConnection::CreateSocket() {
    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    // TODO

    return sock;
}

int32_t WowConnection::InitOsNet(bool (*fcn)(const NETADDR*), void (*threadinit)(), int32_t numThreads, bool useEngine) {
    if (!WowConnection::s_network) {
        // TODO s_usedSocketBits logic
        // TODO WDataStore::StaticInitialize();

        WowConnection::s_verifyAddr = fcn;
        WowConnection::s_destroyed = 0;

        numThreads = std::min(numThreads, 32);

        auto networkMem = SMemAlloc(sizeof(WowConnectionNet), __FILE__, __LINE__, 0x0);
        auto network = new (networkMem) WowConnectionNet(numThreads, threadinit);

        WowConnection::s_network = network;
        WowConnection::s_network->PlatformInit(useEngine);
        WowConnection::s_network->Start();
    }

    return 1;
}

WowConnection::WowConnection(WowConnectionResponse* response, void (*func)(void)) {
    // TODO

    this->Init(response, func);

    this->m_sock = -1;
}

WowConnection::WowConnection(int32_t sock, sockaddr_in* addr, WowConnectionResponse* response) {
    // TODO

    this->Init(response, nullptr);

    // TODO

    this->m_sock = sock;
    this->m_connState = WOWC_CONNECTED;
}

void WowConnection::AcquireResponseRef() {
    this->m_responseLock.Enter();

    STORM_ASSERT(this->m_responseRef == 0 || this->GetState() == WOWC_LISTENING);

    this->m_responseRef++;
    this->m_responseRefThread = SGetCurrentThreadId();

    this->m_responseLock.Leave();
}

void WowConnection::AddRef() {
    SInterlockedIncrement(&this->m_refCount);
}

void WowConnection::CheckAccept() {
    for (int32_t i = 0; i < 10000; i++) {
        NETADDR addr;
        socklen_t addrLen = sizeof(addr);

        int32_t sock = accept(this->m_sock, reinterpret_cast<sockaddr*>(&addr), &addrLen);
        if (sock < 0) {
            break;
        }

        if (WowConnection::s_verifyAddr) {
            NETADDR verifyAddr;
            socklen_t verifyAddrLen = sizeof(verifyAddr);

            getpeername(sock, reinterpret_cast<sockaddr*>(&verifyAddr), &verifyAddrLen);
            if (!WowConnection::s_verifyAddr(&verifyAddr)) {
#if defined(WHOA_SYSTEM_WIN)
                closesocket(sock);
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
                close(sock);
#endif
                continue;
            }
        }

        // TODO
        // RegisterSocket(sock);

#if defined(WHOA_SYSTEM_WIN)
        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
        fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

        auto connMem = SMemAlloc(sizeof(WowConnection), __FILE__, __LINE__, 0x0);
        auto conn = new (connMem) WowConnection(sock, reinterpret_cast<sockaddr_in*>(&addr), this->m_response);
        conn->AddRef();

        this->AddRef();
        this->AcquireResponseRef();

        this->m_lock.Leave();

        if (this->m_response) {
            this->m_response->WCConnected(this, conn, OsGetAsyncTimeMs(), &conn->m_peer);
        }

        WowConnection::s_network->Add(conn);
        WowConnection::s_network->PlatformChangeState(conn, conn->GetState());

        conn->Release();

        this->m_lock.Enter();

        this->ReleaseResponseRef();

        this->Release();
    }
}

void WowConnection::CheckConnect() {
    int32_t err;
    socklen_t errLen = sizeof(err);
    if (getsockopt(this->m_sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &errLen)) {
        return;
    }

    if (err) {
        WowConnection::s_network->Remove(this);

        WowConnection::CloseSocket(this->m_sock);
        this->m_sock = -1;

        this->SetState(WOWC_DISCONNECTED);
        this->AddRef();
        this->AcquireResponseRef();

        this->m_lock.Leave();

        if (this->m_response) {
            this->m_response->WCCantConnect(this, OsGetAsyncTimeMsPrecise(), &this->m_peer);
        }
    } else {
        this->SetState(WOWC_CONNECTED);
        this->AddRef();
        this->AcquireResponseRef();

        this->m_lock.Leave();

        socklen_t peerLen = sizeof(this->m_peer.peerAddr);
        getpeername(this->m_sock, reinterpret_cast<sockaddr*>(&this->m_peer.peerAddr), &peerLen);

        socklen_t selfLen = sizeof(this->m_peer.selfAddr);
        getsockname(this->m_sock, reinterpret_cast<sockaddr*>(&this->m_peer.selfAddr), &selfLen);

        if (this->m_response) {
            this->m_response->WCConnected(this, nullptr, OsGetAsyncTimeMsPrecise(), &this->m_peer);
        }
    }

    this->m_lock.Enter();

    this->ReleaseResponseRef();
    this->Release();
}

void WowConnection::CloseSocket(int32_t sock) {
#if defined(WHOA_SYSTEM_WIN)
    closesocket(sock);
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    close(sock);
#endif

    if (sock >= 0) {
        // TODO
    }
}

bool WowConnection::Connect(char const* address, int32_t retryMs) {
    char host[256];
    auto port = SStrChr(address, ':');

    if (port) {
        this->m_connectPort = SStrToInt(port + 1);

        size_t portIndex = port - address + 1;
        portIndex = std::min(portIndex, sizeof(host));
        SStrCopy(host, address, portIndex);
    } else {
        this->m_connectPort = 0;

        SStrCopy(host, address, sizeof(host));
    }

    this->Connect(host, this->m_connectPort, retryMs);

    return true;
}

bool WowConnection::Connect(char const* address, uint16_t port, int32_t retryMs) {
    auto connectAddress = inet_addr(address);

    if (connectAddress == -1 || connectAddress == 0) {
        auto entry = gethostbyname(address);
        if (entry) {
            auto addrs = reinterpret_cast<uint8_t**>(entry->h_addr_list);
            auto addr0 = addrs[0];
            this->m_connectAddress = addr0[0]
                | addr0[1] << 8
                | addr0[2] << 16
                | addr0[3] << 24;
        } else {
            this->m_connectAddress = 0;
        }
    } else {
        this->m_connectAddress = connectAddress;
    }

    this->m_connectPort = port;

    this->StartConnect();

    return true;
}

void WowConnection::Disconnect() {
    this->m_lock.Enter();

    if (this->m_sock >= 0 && this->GetState() == WOWC_CONNECTED) {
        this->m_connState = WOWC_DISCONNECTING;

        if (WowConnection::s_network) {
            WowConnection::s_network->PlatformChangeState(this, WOWC_CONNECTED);
        }
    }

    this->m_lock.Leave();
}

void WowConnection::DoDisconnect() {
    this->m_lock.Enter();

    if (this->m_sock >= 0) {
        WowConnection::s_network->Remove(this);
        this->CloseSocket(this->m_sock);
    }

    this->SetState(WOWC_DISCONNECTED);

    this->AddRef();
    this->AcquireResponseRef();

    this->m_lock.Leave();

    if (this->m_response && this->m_sock >= 0) {
        // TODO
        // this->m_response->Vfunc4(this, OsGetAsyncTimeMsPrecise());
    }

    this->m_lock.Enter();

    this->m_sock = -1;
    this->ReleaseResponseRef();

    this->m_lock.Leave();

    this->Release();
}

void WowConnection::DoExceptions() {
    this->AddRef();
    this->m_lock.Enter();

    if (this->GetState() == WOWC_CONNECTING) {
        this->CheckConnect();
    }

    this->m_lock.Leave();
    this->Release();
}

void WowConnection::DoMessageReads() {
    if (!this->m_readBuffer) {
        this->m_readBuffer = static_cast<uint8_t*>(SMemAlloc(1024, __FILE__, __LINE__, 0x0));
        this->m_readBufferSize = 1024;
        this->m_readBytes = 0;
    }

    uint32_t timeStamp = OsGetAsyncTimeMsPrecise();

    this->AcquireResponseRef();
    this->m_response->NotifyAboutToDoReads();
    this->ReleaseResponseRef();

    // TODO

    while (true) {
        auto v36 = 2;
        auto v35 = -1;

        if (this->m_readBytes >= 2) {
            uint8_t v14;

            if (*this->m_readBuffer >= 0) {
                v35 = (this->m_readBuffer[1] | ((this->m_readBuffer[0] & 0x7F) << 8)) + 2;
            } else {
                v36 = 3;

                if (this->m_readBytes >= 3) {
                    v35 = (this->m_readBuffer[2] | ((this->m_readBuffer[1] | ((this->m_readBuffer[0] & 0x7F) << 8)) << 8)) + 3;
                }
            }
        }

        if (this->m_readBytes >= this->m_readBufferSize) {
            auto readBuffer = SMemReAlloc(
                this->m_readBuffer,
                this->m_readBufferSize * 2,
                __FILE__,
                __LINE__,
                0x0
            );
            this->m_readBuffer = static_cast<uint8_t*>(readBuffer);
            this->m_readBufferSize = this->m_readBufferSize * 2;

            if (this->m_readBufferSize > 32000000) {
                this->Disconnect();
                return;
            }
        }

        int32_t v17;

        if (v35 >= 0) {
            v17 = v35 - this->m_readBytes;
            if (this->m_readBufferSize - this->m_readBytes < v35 - this->m_readBytes) {
                v17 = this->m_readBufferSize - this->m_readBytes;
            }
        } else {
            v17 = v36 - this->m_readBytes;
        }

#if defined(WHOA_SYSTEM_WIN)
        int32_t bytesRead;
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
        ssize_t bytesRead;
#endif

        if (v17 <= 0) {
            bytesRead = 0;
        } else {
            while (true) {
                bytesRead = recv(this->m_sock, reinterpret_cast<char*>(&this->m_readBuffer[this->m_readBytes]), v17, 0x0);

                if (bytesRead >= 0) {
                    break;
                }

#if defined(WHOA_SYSTEM_WIN)
                if (WSAGetLastError() != WSAEINTR) {
                    break;
                }
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
                if (errno != EINTR) {
                    break;
                }
#endif
            }

            // TODO
        }

        if (bytesRead <= 0 && v17 > 0) {
            break;
        }

        // TODO

        this->m_readBytes += bytesRead;

        if (v35 >= 0 && this->m_readBytes >= v35) {
            CDataStore msg;
            msg.m_data = &this->m_readBuffer[v36];
            msg.m_alloc = -1;
            msg.m_size = v35 - v36;
            msg.m_read = 0;

            this->AcquireResponseRef();

            // TODO NewSendNode conditional

            if (this->m_response) {
                this->m_lock.Leave();
                this->m_response->WCMessageReady(this, timeStamp, &msg);
                this->m_lock.Enter();
            }

            this->m_readBytes = 0;

            this->ReleaseResponseRef();
        }

        if (bytesRead <= 0) {
            return;
        }
    }
}

void WowConnection::DoReads() {
    this->AddRef();

    this->m_lock.Enter();

    if (this->m_connState == WOWC_LISTENING) {
        this->CheckAccept();
    } else if (this->m_connState == WOWC_CONNECTED) {
        if (this->m_type == WOWC_TYPE_STREAM) {
            this->DoStreamReads();
        } else {
            this->DoMessageReads();
        }
    }

    this->m_lock.Leave();

    this->Release();
}

void WowConnection::DoStreamReads() {
    uint32_t startTime = OsGetAsyncTimeMsPrecise();
    uint8_t buf[4096];
#if defined(WHOA_SYSTEM_WIN)
    int32_t bytesRead;
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    ssize_t bytesRead;
#endif

    while (1) {
        while (1) {
            bytesRead = recv(this->m_sock, reinterpret_cast<char*>(buf), sizeof(buf), 0);

            if (bytesRead >= 0) {
                break;
            }

#if defined(WHOA_SYSTEM_WIN)
            if (WSAGetLastError() != WSAEINTR) {
                break;
            }
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
            if (errno != EINTR) {
                break;
            }
#endif
        }

        if (bytesRead <= 0) {
            break;
        }

        this->AcquireResponseRef();
        this->m_lock.Leave();

        if (this->m_response) {
            this->m_response->WCDataReady(this, OsGetAsyncTimeMs(), buf, bytesRead);
        }

        this->m_lock.Enter();
        this->ReleaseResponseRef();

        if (this->GetState() == WOWC_DISCONNECTING || (OsGetAsyncTimeMsPrecise() - startTime) >= 5) {
            return;
        }
    }

    bool shouldDisconnect = false;
#if defined(WHOA_SYSTEM_WIN)
    shouldDisconnect = bytesRead >= 0 || WSAGetLastError() != WSAEWOULDBLOCK;
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    shouldDisconnect = bytesRead >= 0 || errno != EAGAIN;
#endif

    if (shouldDisconnect) {
        this->AcquireResponseRef();

        WowConnection::s_network->Remove(this);
        this->CloseSocket(this->m_sock);
        this->SetState(WOWC_DISCONNECTED);

        this->m_lock.Leave();

        if (this->m_response && this->m_sock >= 0) {
            this->m_response->WCDisconnected(this, OsGetAsyncTimeMs(), &this->m_peer);
        }

        this->m_lock.Enter();

        this->m_sock = -1;

        this->ReleaseResponseRef();
    }
}

void WowConnection::DoWrites() {
    this->AddRef();

    this->m_lock.Enter();

    if (this->m_connState == WOWC_CONNECTING) {
        this->CheckConnect();
    } else {
        // TODO
    }

    // TODO

    this->m_lock.Leave();

    this->Release();
}

void WowConnection::FreeSendNode(SENDNODE* sn) {
    // TODO WDataStore::FreeBuffer(sn, sn->datasize + sizeof(SENDNODE) + 3);
    SMemFree(sn, __FILE__, __LINE__, 0x0);
}

WOW_CONN_STATE WowConnection::GetState() {
    return this->m_connState;
}

void WowConnection::Init(WowConnectionResponse* response, void (*func)(void)) {
    SInterlockedIncrement(&WowConnection::s_numWowConnections);

    this->m_refCount = 1;
    this->m_responseRef = 0;

    // TODO

    this->m_sendDepth = 0;
    this->m_sendDepthBytes = 0;
    this->m_maxSendDepth = 100000;

    // TODO

    this->m_connState = WOWC_UNINITIALIZED;

    // TODO

    this->m_response = response;

    // TODO

    this->m_connectAddress = 0;
    this->m_connectPort = 0;

    // TODO

    this->m_serviceFlags = 0x0;
    this->m_serviceCount = 0;

    // TODO

    this->m_readBuffer = nullptr;
    this->m_readBytes = 0;
    this->m_readBufferSize = 0;

    this->m_event = nullptr;
    this->m_encrypt = false;

    // TODO

    this->SetState(WOWC_INITIALIZED);
    this->m_type = WOWC_TYPE_MESSAGES;
}

WowConnection::SENDNODE* WowConnection::NewSendNode(void* data, int32_t size, bool raw) {
    // TODO counters

    // SENDNODEs are prefixed to their buffers, with an extra 3 bytes reserved for size-prefixing
    uint32_t allocsize = size + sizeof(SENDNODE) + 3;

    // TODO WDataStore::AllocBuffer(allocsize);

    auto m = SMemAlloc(allocsize, __FILE__, __LINE__, 0x0);
    auto buf = &static_cast<uint8_t*>(m)[sizeof(SENDNODE)];
    auto sn = new (m) SENDNODE(data, size, buf, raw);

    sn->allocsize = allocsize;

    // TODO latency tracking

    return sn;
}

void WowConnection::Release() {
    if (SInterlockedDecrement(&this->m_refCount) <= 0) {
        if (WowConnection::s_network) {
            WowConnection::s_network->Delete(this);
        } else {
            // TODO SMemFree
            delete this;
        }
    }
}

void WowConnection::ReleaseResponseRef() {
    this->m_responseLock.Enter();

    STORM_ASSERT(this->m_responseRef > 0);

    this->m_responseRef--;

    // TODO
    // dwordD4 = (void *)this->dwordD4;
    // if (dwordD4) {
    //     this->m_response = dwordD4;
    //     this->dwordD4 = 0;
    // }

    this->m_responseLock.Leave();
}

WC_SEND_RESULT WowConnection::Send(CDataStore* msg, int32_t a3) {
    uint8_t* data;
    msg->GetDataInSitu(reinterpret_cast<void*&>(data), msg->Size());

    WowConnection::s_countTotalBytes += msg->Size();

    this->m_lock.Enter();

    if (msg->Size() == 0 || this->m_connState != WOWC_CONNECTED) {
        this->m_lock.Leave();
        return WC_SEND_ERROR;
    }

    // Queue send

    if (WowConnection::s_lagTestDelayMin || this->m_sendList.Head()) {
        auto sn = this->NewSendNode(data, msg->Size(), false);

        if (this->m_encrypt) {
            // TODO encryption
        }

        this->m_sendList.LinkToTail(sn);

        this->m_sendDepth++;
        this->m_sendDepthBytes += sn->size;

        WowConnection::s_network->PlatformChangeState(this, this->m_connState);

        if (this->m_sendDepth < this->m_maxSendDepth) {
            this->m_lock.Leave();
            return WC_SEND_QUEUED;
        } else {
            // TODO handle max queue send depth reached

            this->m_lock.Leave();
            return WC_SEND_ERROR;
        }
    }

    // Send immediately

    SENDNODE* sn;
    bool snOnStack;

    if (msg->Size() > 100000) {
        snOnStack = false;
        sn = this->NewSendNode(data, msg->Size(), false);
    } else if (a3 < 3) {
        snOnStack = true;

        auto m = alloca(msg->Size() + sizeof(SENDNODE) + 3);
        auto buf = &static_cast<uint8_t*>(m)[sizeof(SENDNODE)];
        sn = new (m) SENDNODE(data, msg->Size(), buf, false);
    } else {
        snOnStack = true;

        auto m = alloca(msg->Size() + sizeof(SENDNODE));
        auto buf = &static_cast<uint8_t*>(m)[sizeof(SENDNODE)];
        sn = new (m) SENDNODE(nullptr, msg->Size(), buf, false);
    }

    if (this->m_encrypt) {
        // TODO encryption
    }

    uint32_t written;
#if defined(WHOA_SYSTEM_WIN)
    written = send(this->m_sock, reinterpret_cast<char*>(sn->data), sn->size, 0x0);
#elif defined(WHOA_SYSTEM_MAC)
    written = write(this->m_sock, sn->data, sn->size);
#endif

    if (written == sn->size) {
        if (!snOnStack) {
            this->FreeSendNode(sn);
        }

        this->m_lock.Leave();
        return WC_SEND_SENT;
    }

    // TODO split writes, errors, etc
    STORM_ASSERT(false);

    return WC_SEND_ERROR;
}

WC_SEND_RESULT WowConnection::SendRaw(uint8_t* data, int32_t len, bool a4) {
    WowConnection::s_countTotalBytes += len;

    this->m_lock.Enter();

    // TODO

    if (len > 0 && this->m_connState == WOWC_CONNECTED) {
        STORM_ASSERT(this->m_sock >= 0);

#if defined (WHOA_SYSTEM_WIN)
        if (this->m_sendList.Head()) {
            // TODO
        } else {
            auto written = send(this->m_sock, reinterpret_cast<char*>(data), len, 0x0);

            if (written == len) {
                this->m_lock.Leave();
                return WC_SEND_SENT;
            }

            if (written < 0) {
                // TODO
            }
        }
#elif defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
        if (this->m_sendList.Head()) {
            // TODO
        } else {
            auto written = write(this->m_sock, data, len);

            if (written <= 0) {
                // TODO
            } else if (written == len) {
                this->m_lock.Leave();
                return WC_SEND_SENT;
            }
        }
#endif
    }

    this->m_lock.Leave();

    return WC_SEND_ERROR;
}

void WowConnection::SetEncryptionType(WC_ENCRYPT_TYPE encryptType) {
    // TODO
}

void WowConnection::SetState(WOW_CONN_STATE state) {
    WOW_CONN_STATE oldState = this->m_connState;
    this->m_connState = state;

    if (WowConnection::s_network) {
        WowConnection::s_network->PlatformChangeState(this, oldState);
    }
}

void WowConnection::SetType(WOWC_TYPE type) {
    this->m_lock.Enter();
    this->m_type = type;
    this->m_lock.Leave();
}

void WowConnection::StartConnect() {
    if (this->m_sock >= 0) {
        if (this->m_netlink.IsLinked()) {
            WowConnection::s_network->Remove(this);
        }

        this->CloseSocket(this->m_sock);
        this->m_sock = -1;
    }

    this->m_lock.Enter();

    this->m_sock = WowConnection::CreateSocket();

    if (this->m_sock < 0) {
        this->SetState(WOWC_ERROR);
        this->m_lock.Leave();

        return;
    }

#if defined(WHOA_SYSTEM_WIN)
    u_long argp = 1;
    ioctlsocket(this->m_sock, FIONBIO, &argp);
#elif defined(WHOA_SYSTEM_MAC)
    fcntl(this->m_sock, F_SETFL, O_NONBLOCK);

    uint32_t opt = 1;
    setsockopt(this->m_sock, SOL_SOCKET, 4130, &opt, sizeof(opt));
#endif

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->m_connectPort);
    addr.sin_addr.s_addr = this->m_connectAddress;

    if (!this->m_netlink.IsLinked()) {
        WowConnection::s_network->Add(this);
    }

    this->SetState(WOWC_CONNECTING);

    if (connect(this->m_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) >= 0) {
        this->m_lock.Leave();

        return;
    }

#if defined(WHOA_SYSTEM_WIN)
    if (WSAGetLastError() == WSAEWOULDBLOCK) {
        this->m_lock.Leave();

        return;
    }
#elif defined(WHOA_SYSTEM_MAC)
    if (errno == EAGAIN || errno == EINTR || errno == EINPROGRESS) {
        this->m_lock.Leave();

        return;
    }
#endif

    WowConnection::s_network->Remove(this);
    this->CloseSocket(this->m_sock);
    this->m_sock = -1;

    this->SetState(WOWC_ERROR);

    this->m_lock.Leave();
}
