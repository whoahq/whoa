#ifndef NET_CONNECTION_NET_CLIENT_HPP
#define NET_CONNECTION_NET_CLIENT_HPP

#include "net/connection/WowConnectionResponse.hpp"
#include "net/Types.hpp"
#include <storm/List.hpp>
#include <storm/Thread.hpp>
#include <cstdint>

class CDataStore;
class NetClient;
class WowConnection;

typedef int32_t (*MESSAGE_HANDLER)(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

class NETEVENTQUEUENODE : public TSLinkedNode<NETEVENTQUEUENODE> {
    public:
};

class NETEVENTQUEUE {
    public:
        // Member variables
        NetClient* m_client;
        SCritSect m_critSect;
        STORM_LIST(NETEVENTQUEUENODE) m_eventQueue;

        // Member functions
        NETEVENTQUEUE(NetClient* client)
            : m_client(client)
            {};
};

class NetClient : public WowConnectionResponse {
    public:
        // Static variables
        static int32_t s_clientCount;

        // Member variables
        LoginData m_loginData;
        NETSTATE m_netState = NS_UNINITIALIZED;
        MESSAGE_HANDLER m_handlers[NUM_MSG_TYPES];
        void* m_handlerParams[NUM_MSG_TYPES];
        NETEVENTQUEUE* m_netEventQueue = nullptr;
        WowConnection* m_serverConnection = nullptr;

        // Virtual member functions
        virtual void WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr);
        virtual void WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual void WCDisconnected(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual void WCDataReady(WowConnection* conn, uint32_t timeStamp, uint8_t* data, int32_t len);

        // Member functions
        void Connect(const char* addrStr);
        int32_t ConnectInternal(const char* host, uint16_t port);
        int32_t Initialize();
        void SetLoginData(LoginData* loginData);
        void SetMessageHandler(NETMESSAGE msgId, MESSAGE_HANDLER handler, void* param);
};

#endif
