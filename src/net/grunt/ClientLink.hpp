#ifndef NET_GRUNT_CLIENT_LINK_HPP
#define NET_GRUNT_CLIENT_LINK_HPP

#include "net/grunt/Grunt.hpp"
#include "net/connection/WowConnectionResponse.hpp"
#include "net/grunt/Pending.hpp"
#include "net/grunt/Timer.hpp"
#include "net/srp/SRP6_Client.hpp"
#include "net/Types.hpp"
#include <storm/Thread.hpp>

class CDataStore;
class WowConnection;

class Grunt::ClientLink : public WowConnectionResponse, Grunt::Pending, Grunt::Timer::Event {
    public:
        // Types
        struct Logon {
            const char* accountName;
            const char* password;
            uint32_t programID;
            uint32_t processorID;
            uint32_t osID;
            uint8_t version[4];
            uint16_t build;
            uint16_t uint1A;
            uint32_t locale;
            uint32_t tz;
        };

        // Member variables
        Grunt::Timer m_timer;
        uint32_t m_clientIP = 0;
        int32_t m_state;
        SRP6_Client m_srpClient;
        SCritSect m_critSect;
        WowConnection* m_connection = nullptr;
        ClientResponse* m_clientResponse;
        char m_accountName[1280];

        // Virtual member functions
        virtual void WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr);
        virtual void WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual void Call();

        // Member functions
        ClientLink(Grunt::ClientResponse& clientResponse);
        void Connect(const char* a2);
        void Disconnect();
        void LogonNewSession(const Logon& logon);
        void PackLogon(CDataStore& msg, const Logon& logon);
        void Send(CDataStore& msg);
        void SetState(int32_t state);
};

#endif
