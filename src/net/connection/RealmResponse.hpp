#ifndef NET_CONNECTION_REALM_RESPONSE_HPP
#define NET_CONNECTION_REALM_RESPONSE_HPP

class RealmResponse {
    public:
        // Virtual member functions
        virtual void HandleAuthResponse(RealmConnection* connection, uint8_t authResult) = 0;
        virtual void CharacterListReceived(RealmConnection* realmConnection, const TSFixedArray<CHARACTER_INFO>& characterList, int32_t listResult) = 0;
        virtual void GameServerResult(RealmConnection* connection, const char* a3, const char* a4, const char* a5) = 0;
};

#endif
