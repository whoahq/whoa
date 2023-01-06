#include "net/grunt/ClientLink.hpp"
#include "net/connection/WowConnection.hpp"
#include "net/grunt/ClientResponse.hpp"
#include <new>
#include <common/DataStore.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

Grunt::ClientLink::ClientLink(Grunt::ClientResponse& clientResponse) {
    // TODO

    this->m_clientResponse = &clientResponse;

    this->SetState(0);

    if (this->m_timer.m_thread.Valid()) {
        this->m_interval = 100;
        this->m_timer.Insert(*this);
    }
}

void Grunt::ClientLink::Call() {
    // TODO
    // this->CheckExpired(false);

    this->m_critSect.Enter();

    if (this->m_state == 2) {
        this->m_clientResponse->GetLogonMethod();
    } else if (this->m_state == 6 && !this->m_clientResponse->OnlineIdle()) {
        this->Disconnect();
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::Connect(const char* a2) {
    if (this->m_state) {
        return;
    }

    this->SetState(1);

    auto connectionMem = SMemAlloc(sizeof(WowConnection), __FILE__, __LINE__, 0x0);
    auto connection = new (connectionMem) WowConnection(this, nullptr);

    this->m_connection = connection;
    this->m_connection->SetType(WOWC_TYPE_STREAM);

    auto port = SStrChr(a2, ':');
    if (port) {
        this->m_connection->Connect(a2, 5000);
    } else {
        this->m_connection->Connect(a2, 3724, 5000);
    }
}

void Grunt::ClientLink::Disconnect() {
    this->m_critSect.Enter();

    if (this->m_connection) {
        this->m_connection->Disconnect();
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::LogonNewSession(const Grunt::ClientLink::Logon& logon) {
    this->SetState(3);

    SStrCopy(this->m_accountName, logon.accountName, sizeof(this->m_accountName));
    SStrUpper(this->m_accountName);

    char* password = static_cast<char*>(alloca(SStrLen(logon.password) + 1));
    SStrCopy(password, logon.password, STORM_MAX_STR);
    SStrUpper(password);

    static char accountNameUnDecorated[1280];
    SStrCopy(accountNameUnDecorated, this->m_accountName, STORM_MAX_STR);
    auto decoration = const_cast<char*>(SStrChr(accountNameUnDecorated, '#'));
    if (decoration) {
        *decoration = '\0';
    }

    this->m_srpClient.BeginAuthentication(accountNameUnDecorated, password);

    CDataStoreCache<1024> clientChallenge;

    uint8_t opcode = 0;
    clientChallenge.Put(opcode);

    uint8_t protocol = 8;
    clientChallenge.Put(protocol);

    this->PackLogon(clientChallenge, logon);
    clientChallenge.Finalize();

    this->Send(clientChallenge);
}

void Grunt::ClientLink::PackLogon(CDataStore& msg, const Logon& logon) {
    uint32_t startPos = msg.m_size;
    uint16_t tmpSize = 0;
    msg.Put(tmpSize);

    msg.Put(logon.programID);
    msg.Put(logon.version[0]);
    msg.Put(logon.version[1]);
    msg.Put(logon.version[2]);
    msg.Put(logon.build);
    msg.Put(logon.processorID);
    msg.Put(logon.osID);
    msg.Put(logon.locale);
    msg.Put(logon.tz);

    msg.Put(this->m_clientIP);

    uint32_t accountNameLen = SStrLen(this->m_accountName);
    msg.Put(accountNameLen);
    msg.PutData(this->m_accountName, accountNameLen);

    msg.Set(startPos, msg.m_size - startPos - 2);
}

void Grunt::ClientLink::Send(CDataStore& msg) {
    this->m_critSect.Enter();

    if (this->m_connection) {
        void* data;
        msg.GetDataInSitu(data, msg.m_size);

        this->m_connection->SendRaw(static_cast<uint8_t*>(data), msg.m_size, false);
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::SetState(int32_t state) {
    this->m_critSect.Enter();

    this->m_state = state;

    this->m_critSect.Leave();
}

void Grunt::ClientLink::WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {
    // TODO
}

void Grunt::ClientLink::WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr) {
    this->m_critSect.Enter();

    this->SetState(2);

    int32_t connected = this->m_clientResponse->Connected(addr->peerAddr);

    // TODO
    // this->m_clientIP = OsNetAddrGetAddress(&addr->selfAddr, 0);

    this->m_critSect.Leave();

    if (!connected) {
        this->Disconnect();
    }
}
