#ifndef NET_CONNECTION_NET_CLIENT_HPP
#define NET_CONNECTION_NET_CLIENT_HPP

#include "net/connection/WowConnectionResponse.hpp"
#include "event/Event.hpp"
#include "net/Types.hpp"
#include <storm/Atomic.hpp>
#include <storm/List.hpp>
#include <storm/Thread.hpp>
#include <cstdint>

class CDataStore;
class NetClient;
class WowConnection;

typedef int32_t (*MESSAGE_HANDLER)(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

struct AuthenticationChallenge {
    uint32_t uint0;
    uint32_t uint4;
    uint64_t uint8;
};

class NETEVENTQUEUENODE : public TSLinkedNode<NETEVENTQUEUENODE> {
    public:
        // Member variables
        EVENTID m_eventId;
        uint32_t m_timeReceived;
        void* m_data;
        uint32_t m_dataSize;
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
        void AddEvent(EVENTID eventId, void* conn, NetClient* client, const void* data, uint32_t bytes);
        void Poll();
};

class NetClient : public WowConnectionResponse {
    public:
        // Virtual member functions
        virtual void WCMessageReady(WowConnection* conn, uint32_t timeStamp, CDataStore* msg);
        virtual void WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr);
        virtual void WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual void WCDisconnected(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual int32_t HandleData(uint32_t timeReceived, void* data, int32_t size);
        virtual int32_t HandleAuthChallenge(AuthenticationChallenge* challenge) = 0;
        virtual int32_t HandleConnect();
        virtual int32_t HandleDisconnect();
        virtual int32_t HandleCantConnect();

        // Member functions
        void AddRef();
        void AuthChallengeHandler(WowConnection* conn, CDataStore* msg);
        void Connect(const char* addrStr);
        int32_t ConnectInternal(const char* host, uint16_t port);
        void DelRef();
        void Disconnect();
        void EnableEncryption(WowConnection* conn, uint8_t* seed, uint8_t seedLen);
        bool GetDelete();
        const LoginData& GetLoginData();
        NETSTATE GetState();
        void HandleIdle();
        int32_t Initialize();
        void PollEventQueue();
        void PongHandler(WowConnection* conn, CDataStore* msg);
        void ProcessMessage(uint32_t timeReceived, CDataStore* msg, int32_t a4);
        void Send(CDataStore* msg);
        void SetDelete();
        void SetLoginData(LoginData* loginData);
        void SetMessageHandler(NETMESSAGE msgId, MESSAGE_HANDLER handler, void* param);

    private:
        // Static variables
        static int32_t s_clientCount;

        // Member variables
        LoginData m_loginData;
        NETSTATE m_netState = NS_UNINITIALIZED;
        bool m_suspended = false;
        MESSAGE_HANDLER m_handlers[NUM_MSG_TYPES];
        void* m_handlerParams[NUM_MSG_TYPES];
        NETEVENTQUEUE* m_netEventQueue = nullptr;
        WowConnection* m_serverConnection = nullptr;
        WowConnection* m_redirectConnection = nullptr;
        ATOMIC32 m_refCount = 0;
        bool m_deleteMe = false;
        uint32_t m_pingSent = 0;
        uint32_t m_pingSequence = 0;
        uint32_t m_latency[16];
        uint32_t m_latencyStart;
        uint32_t m_latencyEnd;
        uint32_t m_bytesSent = 0;
        uint32_t m_bytesReceived = 0;
        uint32_t m_connectedTimestamp = 0;
        SCritSect m_pingLock;
};

#endif
