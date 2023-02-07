#ifndef NET_GRUNT_CLIENT_LINK_HPP
#define NET_GRUNT_CLIENT_LINK_HPP

#include "net/grunt/Grunt.hpp"
#include "net/connection/WowConnectionResponse.hpp"
#include "net/grunt/Pending.hpp"
#include "net/grunt/Timer.hpp"
#include "net/srp/SRP6_Client.hpp"
#include "net/Types.hpp"
#include <common/DataStore.hpp>
#include <storm/Thread.hpp>

class WowConnection;

class Grunt::ClientLink : public WowConnectionResponse, Grunt::Pending, Grunt::Timer::Event {
    public:
        // Types
        enum COMMAND {
            CMD_AUTH_LOGON_CHALLENGE        = 0,
            CMD_AUTH_LOGON_PROOF            = 1,
            CMD_AUTH_RECONNECT_CHALLENGE    = 2,
            CMD_AUTH_RECONNECT_PROOF        = 3,
            CMD_REALM_LIST                  = 16,
            CMD_XFER_INITIATE               = 48,
            CMD_XFER_DATA                   = 49,
        };

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
        uint32_t m_accountFlags = 0x0;
        uint32_t m_surveyID = 0;
        uint32_t m_clientIP = 0;
        int32_t m_state;
        SRP6_Client m_srpClient;
        SCritSect m_critSect;
        CDataStore m_datastore1B0;
        WowConnection* m_connection = nullptr;
        ClientResponse* m_clientResponse;
        char m_accountName[1280];
        char m_serverPublicKey[32];

        // Virtual member functions
        virtual void WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr);
        virtual void WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr);
        virtual void WCDataReady(WowConnection* conn, uint32_t timeStamp, uint8_t* data, int32_t len);
        virtual void Call();

        // Member functions
        ClientLink(Grunt::ClientResponse& clientResponse);
        int32_t CmdAuthLogonChallenge(CDataStore& msg);
        int32_t CmdAuthLogonProof(CDataStore& msg);
        int32_t CmdAuthReconnectChallenge(CDataStore& msg);
        int32_t CmdAuthReconnectProof(CDataStore& msg);
        int32_t CmdRealmList(CDataStore& msg);
        int32_t CmdXferData(CDataStore& msg);
        int32_t CmdXferInitiate(CDataStore& msg);
        void Connect(const char* a2);
        void Disconnect();
        void LogonNewSession(const Logon& logon);
        void PackLogon(CDataStore& msg, const Logon& logon);
        void ProveVersion(const uint8_t* versionChecksum);
        void Send(CDataStore& msg);
        void SetState(int32_t state);
};

#endif
