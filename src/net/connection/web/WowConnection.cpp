#include "net/connection/WowConnection.hpp"
#include "net/connection/WowConnectionNet.hpp"
#include "net/connection/WowConnectionResponse.hpp"
#include "net/connection/web/WsState.hpp"
#include "util/HMAC.hpp"
#include <common/DataStore.hpp>
#include <common/Time.hpp>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <algorithm>
#include <cstring>
#include <new>
#include <emscripten.h>
#include <emscripten/websocket.h>

// Static variables
uint64_t WowConnection::s_countTotalBytes;
int32_t WowConnection::s_destroyed;
int32_t WowConnection::s_lagTestDelayMin;
WowConnectionNet* WowConnection::s_network;
ATOMIC32 WowConnection::s_numWowConnections;
bool (*WowConnection::s_verifyAddr)(const NETADDR*);

static uint8_t s_arc4drop1024[1024] = { 0x00 };
static uint8_t s_arc4seed[] = {
    // Receive key
    0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91, 0x53, 0x57,
    // Send key
    0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43, 0x67, 0xCE,
};

// WebSocket callbacks

static EM_BOOL ws_onopen(int eventType, const EmscriptenWebSocketOpenEvent* event, void* userData) {
    auto conn = static_cast<WowConnection*>(userData);
    auto wsState = static_cast<WsState*>(conn->m_event);
    if (wsState) {
        wsState->connectPending = true;
    }
    return EM_TRUE;
}

static EM_BOOL ws_onmessage(int eventType, const EmscriptenWebSocketMessageEvent* event, void* userData) {
    auto conn = static_cast<WowConnection*>(userData);
    auto wsState = static_cast<WsState*>(conn->m_event);
    if (wsState && event->numBytes > 0) {
        wsState->recvBuf.append(event->data, event->numBytes);
    }
    return EM_TRUE;
}

static EM_BOOL ws_onclose(int eventType, const EmscriptenWebSocketCloseEvent* event, void* userData) {
    auto conn = static_cast<WowConnection*>(userData);
    auto wsState = static_cast<WsState*>(conn->m_event);
    if (wsState) {
        wsState->closePending = true;
    }
    return EM_TRUE;
}

static EM_BOOL ws_onerror(int eventType, const EmscriptenWebSocketErrorEvent* event, void* userData) {
    auto conn = static_cast<WowConnection*>(userData);
    auto wsState = static_cast<WsState*>(conn->m_event);
    if (wsState) {
        wsState->errorPending = true;
    }
    return EM_TRUE;
}

// SENDNODE - same framing logic as native

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
    // Not used on web - WebSocket handles are managed via WsState
    return 0;
}

int32_t WowConnection::InitOsNet(bool (*fcn)(const NETADDR*), void (*threadinit)(), int32_t numThreads, bool useEngine) {
    if (!WowConnection::s_network) {
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
    this->Init(response, func);
    this->m_sock = -1;
}

WowConnection::WowConnection(int32_t sock, sockaddr_in* addr, WowConnectionResponse* response) {
    this->Init(response, nullptr);
    this->m_sock = -1;
    this->m_connState = WOWC_DISCONNECTED;
}

void WowConnection::AcquireResponseRef() {
    // Simplified for single-threaded web - no locking or thread ID needed
    this->m_responseRef++;
}

void WowConnection::AddRef() {
    SInterlockedIncrement(&this->m_refCount);
}

void WowConnection::CheckAccept() {
    // Not applicable on web - no listening sockets
}

void WowConnection::CheckConnect() {
    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState) {
        return;
    }

    if (wsState->errorPending) {
        wsState->errorPending = false;

        WowConnection::s_network->Remove(this);

        if (wsState->ws > 0) {
            emscripten_websocket_close(wsState->ws, 1000, "error");
            emscripten_websocket_delete(wsState->ws);
            wsState->ws = 0;
        }
        this->m_sock = -1;

        this->SetState(WOWC_DISCONNECTED);
        this->AddRef();
        this->AcquireResponseRef();

        this->m_lock.Leave();

        if (this->m_response) {
            this->m_response->WCCantConnect(this, OsGetAsyncTimeMsPrecise(), &this->m_peer);
        }
    } else if (wsState->connectPending) {
        wsState->connectPending = false;

        this->SetState(WOWC_CONNECTED);
        this->AddRef();
        this->AcquireResponseRef();

        this->m_lock.Leave();

        if (this->m_response) {
            this->m_response->WCConnected(this, nullptr, OsGetAsyncTimeMsPrecise(), &this->m_peer);
        }
    } else {
        return;
    }

    this->m_lock.Enter();
    this->ReleaseResponseRef();
    this->Release();
}

void WowConnection::CloseSocket(int32_t sock) {
    auto wsState = static_cast<WsState*>(this->m_event);
    if (wsState && wsState->ws > 0) {
        emscripten_websocket_close(wsState->ws, 1000, nullptr);
        emscripten_websocket_delete(wsState->ws);
        wsState->ws = 0;
        wsState->recvBuf.clear();
        wsState->connectPending = false;
        wsState->closePending = false;
        wsState->errorPending = false;
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
    // Store hostname for WebSocket URL construction
    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState) {
        wsState = new WsState();
        this->m_event = wsState;
    }

    SStrCopy(wsState->connectHost, address, sizeof(wsState->connectHost));
    this->m_connectAddress = 1; // Non-zero to indicate valid target
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
        this->m_response->WCDisconnected(this, OsGetAsyncTimeMsPrecise(), &this->m_peer);
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

    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState) {
        return;
    }

    while (true) {
        auto headerSize = 2;
        auto size = -1;

        if (this->m_readBytes >= headerSize) {
            if ((this->m_readBuffer[0] & 0x80) == 0) {
                size = (this->m_readBuffer[1] | ((this->m_readBuffer[0] & 0x7F) << 8)) + headerSize;
            } else {
                headerSize = 3;

                if (this->m_readBytes >= headerSize) {
                    size = (this->m_readBuffer[2] | ((this->m_readBuffer[1] | ((this->m_readBuffer[0] & 0x7F) << 8)) << 8)) + headerSize;
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

        int32_t bytesToRead;

        if (size >= 0) {
            bytesToRead = size - this->m_readBytes;
            if (this->m_readBufferSize - this->m_readBytes < size - this->m_readBytes) {
                bytesToRead = this->m_readBufferSize - this->m_readBytes;
            }
        } else {
            bytesToRead = headerSize - this->m_readBytes;
        }

        int32_t bytesRead;

        if (bytesToRead <= 0) {
            bytesRead = 0;
        } else {
            // Read from WebSocket receive buffer instead of recv()
            bytesRead = wsState->recvBuf.read(
                &this->m_readBuffer[this->m_readBytes],
                bytesToRead
            );

            if (bytesRead <= 0) {
                break;
            }
        }

        if (this->m_encrypt) {
            auto v22 = headerSize + this->uint376 - this->m_readBytes;
            auto v23 = v22 <= 0 ? 0 : v22;
            if (v23 >= bytesRead) {
                v23 = bytesRead;
            }

            SARC4ProcessBuffer(
                &this->m_readBuffer[this->m_readBytes],
                v23,
                &this->m_receiveKey,
                &this->m_receiveKey
            );
        }

        this->m_readBytes += bytesRead;

        if (size >= 0 && this->m_readBytes >= size) {
            CDataStore msg = CDataStore(&this->m_readBuffer[headerSize], size - headerSize);

            this->AcquireResponseRef();

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

    if (this->m_connState == WOWC_CONNECTED) {
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

    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState) {
        return;
    }

    while (1) {
        int32_t bytesRead = wsState->recvBuf.read(buf, sizeof(buf));

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
}

void WowConnection::DoWrites() {
    this->AddRef();

    this->m_lock.Enter();

    if (this->m_connState == WOWC_CONNECTING) {
        this->CheckConnect();
    }

    this->m_lock.Leave();

    this->Release();
}

void WowConnection::FreeSendNode(SENDNODE* sn) {
    SMemFree(sn, __FILE__, __LINE__, 0x0);
}

WOW_CONN_STATE WowConnection::GetState() {
    return this->m_connState;
}

void WowConnection::Init(WowConnectionResponse* response, void (*func)(void)) {
    SInterlockedIncrement(&WowConnection::s_numWowConnections);

    this->m_refCount = 1;
    this->m_responseRef = 0;
    this->m_sendDepth = 0;
    this->m_sendDepthBytes = 0;
    this->m_maxSendDepth = 100000;
    this->m_connState = WOWC_UNINITIALIZED;
    this->m_response = response;
    this->m_connectAddress = 0;
    this->m_connectPort = 0;
    this->m_serviceFlags = 0x0;
    this->m_serviceCount = 0;
    this->m_readBuffer = nullptr;
    this->m_readBytes = 0;
    this->m_readBufferSize = 0;
    this->m_event = nullptr;
    this->m_encrypt = false;

    this->SetState(WOWC_INITIALIZED);
    this->m_type = WOWC_TYPE_MESSAGES;
}

WowConnection::SENDNODE* WowConnection::NewSendNode(void* data, int32_t size, bool raw) {
    uint32_t allocsize = size + sizeof(SENDNODE) + 3;

    auto m = SMemAlloc(allocsize, __FILE__, __LINE__, 0x0);
    auto buf = &static_cast<uint8_t*>(m)[sizeof(SENDNODE)];
    auto sn = new (m) SENDNODE(data, size, buf, raw);

    sn->allocsize = allocsize;

    return sn;
}

void WowConnection::Release() {
    if (SInterlockedDecrement(&this->m_refCount) <= 0) {
        // Clean up WebSocket state
        auto wsState = static_cast<WsState*>(this->m_event);
        if (wsState) {
            if (wsState->ws > 0) {
                emscripten_websocket_close(wsState->ws, 1000, nullptr);
                emscripten_websocket_delete(wsState->ws);
            }
            wsState->recvBuf.clear();
            delete wsState;
            this->m_event = nullptr;
        }

        if (WowConnection::s_network) {
            WowConnection::s_network->Delete(this);
        } else {
            delete this;
        }
    }
}

void WowConnection::ReleaseResponseRef() {
    // Simplified for single-threaded web - no locking needed
    this->m_responseRef--;
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

    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState || wsState->ws <= 0) {
        this->m_lock.Leave();
        return WC_SEND_ERROR;
    }

    // Build framed packet with header
    auto sn = this->NewSendNode(data, msg->Size(), false);

    if (this->m_encrypt) {
        auto bufSize = std::min(sn->size, sn->size + this->uint375 - sn->datasize);
        SARC4ProcessBuffer(sn->data, bufSize, &this->m_sendKey, &this->m_sendKey);
    }

    // Send via WebSocket binary
    auto result = emscripten_websocket_send_binary(wsState->ws, sn->data, sn->size);

    this->FreeSendNode(sn);

    this->m_lock.Leave();

    if (result == EMSCRIPTEN_RESULT_SUCCESS) {
        return WC_SEND_SENT;
    }

    return WC_SEND_ERROR;
}

WC_SEND_RESULT WowConnection::SendRaw(uint8_t* data, int32_t len, bool a4) {
    WowConnection::s_countTotalBytes += len;

    this->m_lock.Enter();

    if (len > 0 && this->m_connState == WOWC_CONNECTED) {
        auto wsState = static_cast<WsState*>(this->m_event);
        if (wsState && wsState->ws > 0) {
            auto result = emscripten_websocket_send_binary(wsState->ws, data, len);
            this->m_lock.Leave();

            if (result == EMSCRIPTEN_RESULT_SUCCESS) {
                return WC_SEND_SENT;
            }

            return WC_SEND_ERROR;
        }
    }

    this->m_lock.Leave();

    return WC_SEND_ERROR;
}

void WowConnection::SetEncryption(bool enabled) {
    this->m_lock.Enter();

    this->m_encrypt = enabled;

    SARC4PrepareKey(this->m_sendKeyInit, sizeof(this->m_sendKeyInit), &this->m_sendKey);
    SARC4PrepareKey(this->m_receiveKeyInit, sizeof(this->m_receiveKeyInit), &this->m_receiveKey);

    SARC4ProcessBuffer(s_arc4drop1024, sizeof(s_arc4drop1024), &this->m_sendKey, &this->m_sendKey);
    SARC4ProcessBuffer(s_arc4drop1024, sizeof(s_arc4drop1024), &this->m_receiveKey, &this->m_receiveKey);

    this->m_lock.Leave();
}

void WowConnection::SetEncryptionKey(const uint8_t* key, uint8_t keyLen, uint8_t a4, const uint8_t* seedData, uint8_t seedLen) {
    if (!seedData) {
        seedData = s_arc4seed;
        seedLen = sizeof(s_arc4seed);
    }

    const uint8_t* seeds[] = {
        seedData,
        &seedData[seedLen / 2]
    };

    HMAC_SHA1(seeds[a4], seedLen / 2, key, keyLen, this->m_sendKeyInit);
    HMAC_SHA1(seeds[a4 ^ 1], seedLen / 2, key, keyLen, this->m_receiveKeyInit);

    SARC4PrepareKey(this->m_sendKeyInit, sizeof(this->m_sendKeyInit), &this->m_sendKey);
    SARC4PrepareKey(this->m_receiveKeyInit, sizeof(this->m_receiveKeyInit), &this->m_receiveKey);

    SARC4ProcessBuffer(s_arc4drop1024, sizeof(s_arc4drop1024), &this->m_sendKey, &this->m_sendKey);
    SARC4ProcessBuffer(s_arc4drop1024, sizeof(s_arc4drop1024), &this->m_receiveKey, &this->m_receiveKey);
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
    auto wsState = static_cast<WsState*>(this->m_event);
    if (!wsState) {
        wsState = new WsState();
        this->m_event = wsState;
    }

    // Close existing WebSocket if reconnecting
    if (wsState->ws > 0) {
        if (this->m_netlink.IsLinked()) {
            WowConnection::s_network->Remove(this);
        }

        emscripten_websocket_close(wsState->ws, 1000, "reconnecting");
        emscripten_websocket_delete(wsState->ws);
        wsState->ws = 0;
        wsState->recvBuf.clear();
        wsState->connectPending = false;
        wsState->closePending = false;
        wsState->errorPending = false;
        this->m_sock = -1;
    }

    this->m_lock.Enter();

    // Build proxy WebSocket URL from the page origin
    char url[512];
    EM_ASM({
        var proto = (location.protocol === 'https:') ? 'wss:' : 'ws:';
        var url = proto + '//' + location.host + '/proxy/tcp/' + UTF8ToString($0) + '/' + $1;
        stringToUTF8(url, $2, $3);
    }, wsState->connectHost, this->m_connectPort, url, sizeof(url));

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);
    attr.url = url;

    wsState->ws = emscripten_websocket_new(&attr);

    if (wsState->ws <= 0) {
        this->SetState(WOWC_ERROR);
        this->m_lock.Leave();
        return;
    }

    this->m_sock = wsState->ws;

    // Register WebSocket callbacks
    emscripten_websocket_set_onopen_callback(wsState->ws, this, ws_onopen);
    emscripten_websocket_set_onmessage_callback(wsState->ws, this, ws_onmessage);
    emscripten_websocket_set_onclose_callback(wsState->ws, this, ws_onclose);
    emscripten_websocket_set_onerror_callback(wsState->ws, this, ws_onerror);

    if (!this->m_netlink.IsLinked()) {
        WowConnection::s_network->Add(this);
    }

    this->SetState(WOWC_CONNECTING);

    this->m_lock.Leave();
}
