#include "net/connection/RealmConnection.hpp"
#include "net/Types.hpp"

int32_t RealmConnection::MessageHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO
    return 0;
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

    // TODO
}
