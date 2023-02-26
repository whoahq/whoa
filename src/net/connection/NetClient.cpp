#include "net/connection/NetClient.hpp"
#include "net/connection/WowConnection.hpp"
#include <cstdlib>
#include <cstring>
#include <new>
#include <common/DataStore.hpp>
#include <common/Prop.hpp>
#include <common/SHA1.hpp>
#include <common/Time.hpp>
#include <storm/Error.hpp>
#include <storm/String.hpp>

HPROPCONTEXT s_propContext;

int32_t NetClient::s_clientCount;

void InitializePropContext() {
    if (PropGetSelectedContext() != s_propContext) {
        PropSelectContext(s_propContext);
    }
}

void NETEVENTQUEUE::AddEvent(EVENTID eventId, void* conn, NetClient* client, const void* data, uint32_t bytes) {
    this->m_critSect.Enter();

    auto node = this->m_eventQueue.NewNode(2, 0, 0x0);

    node->m_eventId = eventId;

    if (bytes) {
        node->m_data = SMemAlloc(bytes, __FILE__, __LINE__, 0x0);
        memcpy(node->m_data, data, bytes);
        node->m_dataSize = bytes;
    } else {
        node->m_data = nullptr;
        node->m_dataSize = 0;
    }

    node->m_timeReceived = OsGetAsyncTimeMsPrecise();

    client->AddRef();

    this->m_critSect.Leave();
}

void NETEVENTQUEUE::Poll() {
    this->m_critSect.Enter();

    auto deleted = false;
    auto client = this->m_client;

    client->AddRef();

    for (auto node = this->m_eventQueue.Head(); node; node = this->m_eventQueue.Next(node)) {
        if (!client->GetDelete()) {
            switch (node->m_eventId) {
            case EVENT_ID_NET_DATA:
                client->HandleData(node->m_timeReceived, node->m_data, node->m_dataSize);
                break;

            case EVENT_ID_NET_CONNECT:
                client->HandleConnect();
                break;

            case EVENT_ID_NET_DISCONNECT:
                client->HandleDisconnect();
                break;

            case EVENT_ID_NET_CANTCONNECT:
                client->HandleCantConnect();
                break;

            case EVENT_ID_NET_DESTROY:
                client->SetDelete();
                deleted = true;
                break;

            case EVENT_ID_NET_AUTH_CHALLENGE:
                client->HandleAuthChallenge(static_cast<AuthenticationChallenge*>(node->m_data));
                break;

            default:
                break;
            }
        }

        // Matching 1:1 with the ref added by NETEVENTQUEUE::AddEvent
        client->DelRef();
    }

    if (!deleted) {
        client->HandleIdle();
    }

    client->DelRef();

    this->m_eventQueue.DeleteAll();

    this->m_critSect.Leave();
}

void NetClient::AddRef() {
    SInterlockedIncrement(&this->m_refCount);
}

void NetClient::AuthChallengeHandler(WowConnection* conn, CDataStore* msg) {
    auto challenge = static_cast<AuthenticationChallenge*>(SMemAlloc(sizeof(AuthenticationChallenge), __FILE__, __LINE__, 0x0));

    uint32_t v14;
    msg->Get(v14);

    msg->Get(challenge->uint0);

    // TODO calculate client seed?

    if (conn == this->m_serverConnection) {
        this->m_netEventQueue->AddEvent(EVENT_ID_NET_AUTH_CHALLENGE, conn, this, challenge, sizeof(AuthenticationChallenge));
    } else if (conn == this->m_redirectConnection) {
        // TODO
    } else {
        conn->Disconnect();
    }

    delete challenge;
}

void NetClient::Connect(const char* addrStr) {
    if (this->m_netState != NS_INITIALIZED) {
        SErrDisplayAppFatal("Expected (m_netState == NS_INITIALIZED), got %d", this->m_netState);
    }

    uint16_t port = 9090;

    char host[1024];
    SStrCopy(host, addrStr, sizeof(host));

    auto portDelim = SStrChr(host, ':');
    if (portDelim) {
        *portDelim = '\0';
        port = atoi(portDelim + 1);
    }

    this->m_serverConnection->SetEncryptionType(WC_ENCRYPT_0);
    this->m_netState = NS_INITIALIZED;
    this->ConnectInternal(host, port);
}

int32_t NetClient::ConnectInternal(const char* host, uint16_t port) {
    if (this->m_netState != NS_INITIALIZED) {
        SErrDisplayAppFatal("Expected (m_netState == NS_INITIALIZED), got %d", this->m_netState);
    }

    this->m_netState = NS_CONNECTING;
    this->m_serverConnection->Connect(host, port, -1);

    // TODO

    return 1;
}

void NetClient::DelRef() {
    auto refCount = SInterlockedDecrement(&this->m_refCount);

    if (refCount == 0 && this->m_deleteMe) {
        delete this;
    }
}

bool NetClient::GetDelete() {
    return this->m_deleteMe;
}

NETSTATE NetClient::GetState() {
    return this->m_netState;
}

int32_t NetClient::HandleCantConnect() {
    // TODO
    return 1;
}

int32_t NetClient::HandleConnect() {
    // TODO push obj mgr

    this->m_netState = NS_CONNECTED;

    // TODO pop obj mgr

    return 1;
}

int32_t NetClient::HandleData(uint32_t timeReceived, void* data, int32_t size) {
    // TODO
    return 1;
}

int32_t NetClient::HandleDisconnect() {
    // TODO
    return 1;
}

void NetClient::HandleIdle() {
    // TODO;
}

int32_t NetClient::Initialize() {
    STORM_ASSERT(this->m_netState == NS_UNINITIALIZED);

    if (NetClient::s_clientCount == 0) {
        s_propContext = PropGetSelectedContext();

        if (!WowConnection::InitOsNet(nullptr, InitializePropContext, 1, false)) {
            return 0;
        }
    }

    NetClient::s_clientCount++;

    auto queueMem = SMemAlloc(sizeof(NETEVENTQUEUE), __FILE__, __LINE__, 0x0);
    auto queue = new (queueMem) NETEVENTQUEUE(this);
    this->m_netEventQueue = queue;

    memset(this->m_handlers, 0, sizeof(this->m_handlers));
    memset(this->m_handlerParams, 0, sizeof(this->m_handlerParams));

    auto connectionMem = SMemAlloc(sizeof(WowConnection), __FILE__, __LINE__, 0x0);
    auto connection = new (connectionMem) WowConnection(this, nullptr);
    this->m_serverConnection = connection;

    this->m_netState = NS_INITIALIZED;

    return 1;
}

void NetClient::PollEventQueue() {
    this->m_netEventQueue->Poll();
}

void NetClient::PongHandler(WowConnection* conn, CDataStore* msg) {
    // TODO
}

void NetClient::SetDelete() {
    this->m_deleteMe = true;
}

void NetClient::SetLoginData(LoginData* loginData) {
    memcpy(&this->m_loginData, loginData, sizeof(this->m_loginData));
}

void NetClient::SetMessageHandler(NETMESSAGE msgId, MESSAGE_HANDLER handler, void* param) {
    this->m_handlers[msgId] = handler;
    this->m_handlerParams[msgId] = param;
}

void NetClient::WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {
    // TODO
}

void NetClient::WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr) {
    if (conn != this->m_serverConnection) {
        return;
    }

    this->m_pingLock.Enter();

    // TODO

    this->m_latencyStart = 0;
    this->m_latencyEnd = 0;
    this->m_pingSent = OsGetAsyncTimeMsPrecise();

    this->m_pingLock.Leave();

    this->m_netEventQueue->AddEvent(EVENT_ID_NET_CONNECT, conn, this, nullptr, 0);
}

void NetClient::WCDisconnected(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {
    // TODO
}

void NetClient::WCMessageReady(WowConnection* conn, uint32_t timeStamp, CDataStore* msg) {
    uint8_t* data;
    msg->GetDataInSitu(reinterpret_cast<void*&>(data), msg->m_size);

    // TODO increment byte counter
    // SInterlockedExchangeAdd(this->m_bytesReceived, msg->m_size);

    msg->m_read = 0;

    uint16_t msgId;
    msg->Get(msgId);

    // TODO SMSG_SUSPEND_COMMS (0x50F)
    // TODO SMSG_FORCE_SEND_QUEUED_PACKETS (0x511)
    // TODO SMSG_REDIRECT_CLIENT (0x50D)

    if (msgId == SMSG_PONG) {
        this->PongHandler(conn, msg);
        return;
    } else if (msgId == SMSG_AUTH_CHALLENGE) {
        this->AuthChallengeHandler(conn, msg);
        return;
    }

    if (conn == this->m_serverConnection && !this->m_suspended) {
        msg->m_read = msg->m_size;
        this->m_netEventQueue->AddEvent(EVENT_ID_NET_DATA, conn, this, data, msg->m_size);
    } else {
        conn->Disconnect();
    }
}
