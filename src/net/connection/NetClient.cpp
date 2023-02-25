#include "net/connection/NetClient.hpp"
#include "net/connection/WowConnection.hpp"
#include <cstdlib>
#include <cstring>
#include <new>
#include <common/DataStore.hpp>
#include <common/Prop.hpp>
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
    // TODO
}

void NetClient::AuthChallengeHandler(WowConnection* conn, CDataStore* msg) {
    // TODO
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

void NetClient::PongHandler(WowConnection* conn, CDataStore* msg) {
    // TODO
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
