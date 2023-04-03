#ifndef CLIENT_CLIENT_REALM_RESPONSE_ADAPTER_HPP
#define CLIENT_CLIENT_REALM_RESPONSE_ADAPTER_HPP

#include "net/Connection.hpp"

class ClientRealmResponseAdapter : public RealmResponse {
    public:
        // Virtual member functions
        virtual void HandleAuthResponse(RealmConnection* connection, uint8_t authResult);
        virtual void GameServerResult(RealmConnection* connection, const char* a2, const char* a3, const char* a4) {};
};

#endif
