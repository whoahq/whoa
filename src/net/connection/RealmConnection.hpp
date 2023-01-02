#ifndef NET_CONNECTION_REALM_CONNECTION_HPP
#define NET_CONNECTION_REALM_CONNECTION_HPP

#include "net/connection/NetClient.hpp"
#include <cstdint>

class CDataStore;
class RealmResponse;

class RealmConnection : public NetClient {
    public:
        // Static functions
        int32_t static MessageHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

        // Member variables
        RealmResponse* m_realmResponse;

        // Member functions
        RealmConnection(RealmResponse* realmResponse);
};

#endif
