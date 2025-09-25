#ifndef NET_CONNECTION_WOW_CONNECTION_NET_HPP
#define NET_CONNECTION_WOW_CONNECTION_NET_HPP

#include "net/connection/WowConnection.hpp"
#include <cstdint>
#include <storm/List.hpp>
#include <storm/Thread.hpp>

class WowConnectionNet {
    public:
        // Types
        struct Worker {
            WowConnectionNet* owner;
            SThread thread;
            int32_t id;
            WowConnection* serviceConn;
            SEvent event = SEvent(0, 0);
            int8_t quit;
            SCritSect lock;
        };

        // Member variables
        SThread m_thread;
        SEvent m_stopEvent = SEvent(1, 0);
        uint8_t m_stop;
        int32_t m_numWorkers;
        Worker m_workers[32];
        SCritSect m_connectionsLock;
        STORM_EXPLICIT_LIST(WowConnection, m_netlink) m_connections;
        SSemaphore m_workerSem;
        void (*m_threadinit)();
        void* event8E8;

        // Member functions
        WowConnectionNet(uint32_t numThreads, void (*threadinit)())
            : m_workerSem(0, numThreads)
            , m_numWorkers(numThreads)
            , m_threadinit(threadinit)
            , m_stop(0)
            {};
        void Add(WowConnection* connection);
        void Delete(WowConnection* connection);
        void PlatformAdd(WowConnection* connection);
        void PlatformChangeState(WowConnection* connection, WOW_CONN_STATE state);
        void PlatformInit(bool useEngine);
        void PlatformRemove(WowConnection* connection);
        void PlatformRun();
        void PlatformWorkerReady();
        void Remove(WowConnection* connection);
        void Run();
        void RunWorker(int32_t id);
        void Service(WowConnection* connection, uint32_t serviceFlags);
        void SignalWorker(WowConnection* connection, uint32_t flags);
        void Start();
};

#endif
