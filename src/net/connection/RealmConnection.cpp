#include "net/connection/RealmConnection.hpp"
#include "net/Types.hpp"
#include <common/DataStore.hpp>
#include <common/SHA1.hpp>
#include <storm/String.hpp>

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

    // TODO switch to WDataStore
    CDataStore msg;

    uint32_t localChallenge;

    msg.Put(static_cast<uint32_t>(CMSG_AUTH_SESSION));

    msg.Put(static_cast<uint32_t>(12340));
    msg.Put(static_cast<uint32_t>(this->GetLoginData().m_loginServerID));
    msg.PutString(this->GetLoginData().m_account);
    msg.Put(static_cast<uint32_t>(this->GetLoginData().m_loginServerType));

    // TODO
    msg.Put(localChallenge);

    // TODO
    msg.Put(static_cast<uint32_t>(0));
    msg.Put(static_cast<uint32_t>(0));
    msg.Put(static_cast<uint32_t>(1));

    // TODO
    msg.Put(static_cast<uint64_t>(0));

    uint32_t msgId = 0;

    SHA1_CONTEXT ctx;
    SHA1_Init(&ctx);

    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(this->GetLoginData().m_account), SStrLen(this->GetLoginData().m_account));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&msgId), sizeof(msgId));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&localChallenge), sizeof(localChallenge));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&challenge->uint0), sizeof(challenge->uint0));
    SHA1_Update(&ctx, this->GetLoginData().m_sessionKey, sizeof(this->GetLoginData().m_sessionKey));

    uint8_t clientProof[SHA1_DIGEST_SIZE];
    SHA1_Final(clientProof, &ctx);

    msg.PutData(clientProof, sizeof(clientProof));

    // TODO addons
    msg.Put(static_cast<uint32_t>(0));

    msg.Finalize();

    this->Send(&msg);

    return 1;
}

void RealmConnection::SetSelectedRealm(uint32_t a2, uint32_t a3, uint32_t a4) {
    // TODO
}
