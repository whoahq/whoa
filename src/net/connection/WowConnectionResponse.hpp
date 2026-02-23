#ifndef NET_CONNECTION_WOW_CONNECTION_RESPONSE_HPP
#define NET_CONNECTION_WOW_CONNECTION_RESPONSE_HPP

#include "net/Types.hpp"

class CDataStore;
class WowConnection;

class WowConnectionResponse {
    public:
        // Virtual destructor
        virtual ~WowConnectionResponse() = default;

        // Virtual member functions
        virtual void WCMessageReady(WowConnection* conn, uint32_t timeStamp, CDataStore* msg) = 0;
        virtual void WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr) = 0;
        virtual void WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) = 0;
        virtual void WCDisconnected(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {};
        virtual void WCDataReady(WowConnection* conn, uint32_t timeStamp, uint8_t* data, int32_t len) {};
        virtual void NotifyAboutToDoReads() {};
};

#endif
