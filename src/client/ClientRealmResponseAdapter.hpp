#ifndef CLIENT_CLIENT_REALM_RESPONSE_ADAPTER_HPP
#define CLIENT_CLIENT_REALM_RESPONSE_ADAPTER_HPP

#include "net/Connection.hpp"

class ClientRealmResponseAdapter : public RealmResponse {
    public:
        // Virtual member functions
        virtual void HandleAuthResponse(RealmConnection* realmConnection, uint8_t authResult);
        virtual void CharacterListReceived(RealmConnection* realmConnection, const TSFixedArray<CHARACTER_INFO>& characterList, int32_t listSuccess);
        virtual void GameServerResult(RealmConnection* realmConnection, const char* a2, const char* a3, const char* a4) {};
};

#endif
