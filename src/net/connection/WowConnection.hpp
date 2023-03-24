#ifndef NET_CONNECTION_WOW_CONNECTION_HPP
#define NET_CONNECTION_WOW_CONNECTION_HPP

#include "net/Types.hpp"
#include <cstdint>
#include <storm/Atomic.hpp>
#include <storm/Crypto.hpp>
#include <storm/List.hpp>
#include <storm/Thread.hpp>

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <netinet/in.h>
#endif

#if defined(WHOA_SYSTEM_WIN)
#include <winsock2.h>
#endif

class CDataStore;
class WowConnectionNet;
class WowConnectionResponse;

class WowConnection {
    public:
        // Types
        struct SENDNODE : public TSLinkedNode<SENDNODE> {
            uint8_t* data;
            uint32_t size;
            uint32_t offset;
            uint32_t datasize;
            uint8_t header[8];
            uint32_t uint20;
            uint32_t allocsize;

            SENDNODE(void* data, int32_t size, uint8_t* buf, bool raw);
        };

        // Static variables
        static uint64_t s_countTotalBytes;
        static int32_t s_destroyed;
        static int32_t s_lagTestDelayMin;
        static WowConnectionNet* s_network;
        static ATOMIC32 s_numWowConnections;
        static bool (*s_verifyAddr)(const NETADDR*);

        // Static functions
        static int32_t CreateSocket();
        static int32_t InitOsNet(bool (*fcn)(const NETADDR*), void (*threadinit)(), int32_t numThreads, bool useEngine);

        // Member variables
        ATOMIC32 m_refCount;
        int32_t m_sock;
        WOW_CONN_STATE m_connState;
        WowConnectionResponse* m_response;
        uint8_t* m_readBuffer;
        int32_t m_readBytes;
        int32_t m_readBufferSize;
        uint32_t m_connectAddress;
        uint16_t m_connectPort;
        NETCONNADDR m_peer;
        SCritSect m_responseLock;
        int32_t m_responseRef;
        uintptr_t m_responseRefThread;
        STORM_LIST(SENDNODE) m_sendList;
        int32_t m_sendDepth;
        uint32_t m_sendDepthBytes;
        int32_t m_maxSendDepth;
        uint32_t m_serviceFlags;
        TSLink<WowConnection> m_netlink;
        SCritSect m_lock;
        ATOMIC32 m_serviceCount;
        void* m_event;
        WOWC_TYPE m_type;
        SARC4Key m_sendKey;
        SARC4Key m_receiveKey;
        uint8_t m_sendKeyInit[20];
        uint8_t m_receiveKeyInit[20];
        bool m_encrypt;
        uint8_t uint375;
        uint8_t uint376;

        // Member functions
        WowConnection(WowConnectionResponse* response, void (*func)(void));
        WowConnection(int32_t sock, sockaddr_in* addr, WowConnectionResponse* response);
        void AcquireResponseRef();
        void AddRef();
        void CheckAccept();
        void CheckConnect();
        void CloseSocket(int32_t sock);
        bool Connect(char const* address, int32_t retryMs);
        bool Connect(char const* address, uint16_t port, int32_t retryMs);
        void Disconnect();
        void DoDisconnect();
        void DoExceptions();
        void DoMessageReads();
        void DoReads();
        void DoStreamReads();
        void DoWrites();
        void FreeSendNode(SENDNODE* sn);
        WOW_CONN_STATE GetState();
        void Init(WowConnectionResponse* response, void (*func)(void));
        SENDNODE* NewSendNode(void* data, int32_t size, bool raw);
        void Release();
        void ReleaseResponseRef();
        WC_SEND_RESULT Send(CDataStore* msg, int32_t a3);
        WC_SEND_RESULT SendRaw(uint8_t* data, int32_t len, bool a4);
        void SetEncryption(bool enabled);
        void SetEncryptionKey(const uint8_t* key, uint8_t keyLen, uint8_t a4, const uint8_t* seed, uint8_t seedLen);
        void SetState(WOW_CONN_STATE state);
        void SetType(WOWC_TYPE type);
        void StartConnect();
};

#endif
