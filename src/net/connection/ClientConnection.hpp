#ifndef NET_CONNECTION_CLIENT_CONNECTION_HPP
#define NET_CONNECTION_CLIENT_CONNECTION_HPP

#include "net/connection/RealmConnection.hpp"
#include "net/Types.hpp"

class RealmResponse;

class ClientConnection : public RealmConnection {
    public:
        // Member variables
        int32_t m_statusComplete = 1;
        int32_t m_statusResult = 1;
        WOWCS_OPS m_statusCop = COP_NONE;
        int32_t m_errorCode = 0;
        void (*m_cleanup)() = nullptr;

        // Member functions
        ClientConnection(RealmResponse* realmResponse)
            : RealmConnection(realmResponse)
            {};
        void Cancel(int32_t errorCode);
        void Cleanup();
        void Connect();
};

#endif
