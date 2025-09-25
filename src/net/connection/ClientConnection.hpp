#ifndef NET_CONNECTION_CLIENT_CONNECTION_HPP
#define NET_CONNECTION_CLIENT_CONNECTION_HPP

#include "net/connection/RealmConnection.hpp"
#include "net/Types.hpp"

class RealmResponse;

class ClientConnection : public RealmConnection {
    public:
        // Member variables
        int32_t m_connected = 0;
        int32_t m_statusComplete = 1;
        int32_t m_statusResult = 1;
        WOWCS_OPS m_statusCop = COP_NONE;
        int32_t m_errorCode = 0;
        void (*m_cleanup)() = nullptr;

        // Virtual member functions
        virtual int32_t HandleConnect();

        // Member functions
        ClientConnection(RealmResponse* realmResponse)
            : RealmConnection(realmResponse)
            {};
        void AccountLogin_Finish(int32_t authResult);
        void AccountLogin_Queued();
        void Cancel(int32_t errorCode);
        void Cleanup();
        void Complete(int32_t result, int32_t errorCode);
        void Connect();
        int32_t Disconnect();
        void Initiate(WOWCS_OPS op, int32_t errorCode, void (*cleanup)());
        int32_t IsConnected();
        int32_t PollStatus(WOWCS_OPS& op, const char** msg, int32_t& result, int32_t& errorCode);
};

#endif
