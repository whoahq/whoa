#ifndef NET_CONNECTION_REALM_CONNECTION_HPP
#define NET_CONNECTION_REALM_CONNECTION_HPP

#include "net/connection/NetClient.hpp"
#include <cstdint>

class CDataStore;
class RealmResponse;

class RealmConnection : public NetClient {
    public:
        // Types
        struct REALMCONNECTIONNODE : TSLinkedNode<REALMCONNECTIONNODE> {
            RealmConnection* connection;
        };

        // Static variables
        SCritSect static s_AllRealmConnectionsCrit;
        STORM_LIST(REALMCONNECTIONNODE) static s_AllRealmConnections;

        // Static functions
        int32_t static MessageHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);
        void static PollNet();

        // Member variables
        RealmResponse* m_realmResponse;

        // Virtual member functions
        virtual int32_t HandleAuthChallenge(AuthenticationChallenge* challenge);

        // Member functions
        RealmConnection(RealmResponse* realmResponse);
        void SetSelectedRealm(uint32_t a2, uint32_t a3, uint32_t a4);
};

#endif
