#include "net/connection/RealmConnection.hpp"
#include "net/Types.hpp"

SCritSect RealmConnection::s_AllRealmConnectionsCrit;
STORM_LIST(RealmConnection::REALMCONNECTIONNODE) RealmConnection::s_AllRealmConnections;

int32_t RealmConnection::MessageHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO
    return 0;
}

void RealmConnection::PollNet() {
    RealmConnection::s_AllRealmConnectionsCrit.Enter();

    for (auto node = RealmConnection::s_AllRealmConnections.Head(); node; node = RealmConnection::s_AllRealmConnections.Next(node)) {
        node->connection->PollEventQueue();
    }

    RealmConnection::s_AllRealmConnectionsCrit.Leave();
}

RealmConnection::RealmConnection(RealmResponse* realmResponse) {
    this->m_realmResponse = realmResponse;

    // TODO

    this->Initialize();

    this->SetMessageHandler(SMSG_AUTH_CHALLENGE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_AUTH_RESPONSE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_ADDON_INFO, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_ENUM_CHARACTERS_RESULT, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CREATE_CHAR, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CHARACTER_LOGIN_FAILED, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_COMPLETE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_CANCEL_ACK, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_RESPONSE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_DELETE_CHAR, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CACHE_VERSION, &RealmConnection::MessageHandler, this);

    RealmConnection::s_AllRealmConnectionsCrit.Enter();
    auto node = RealmConnection::s_AllRealmConnections.NewNode(2, 0, 0x0);
    node->connection = this;
    RealmConnection::s_AllRealmConnectionsCrit.Leave();
}

int32_t RealmConnection::HandleAuthChallenge(AuthenticationChallenge* challenge) {
    // TODO
    return 1;
}

void RealmConnection::SetSelectedRealm(uint32_t a2, uint32_t a3, uint32_t a4) {
    // TODO
}
