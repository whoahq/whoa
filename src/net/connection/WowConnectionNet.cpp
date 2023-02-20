#include "net/connection/WowConnectionNet.hpp"
#include "net/connection/WowConnection.hpp"
#include <storm/Atomic.hpp>
#include <storm/Error.hpp>
#include <storm/String.hpp>
#include <storm/Thread.hpp>

uint32_t MainProc(void* param) {
    auto network = static_cast<WowConnectionNet*>(param);
    network->Run();

    return 0;
}

uint32_t WorkerProc(void* param) {
    auto worker = static_cast<WowConnectionNet::Worker*>(param);
    worker->owner->RunWorker(worker->id);

    return 0;
}

void WowConnectionNet::Add(WowConnection* connection) {
    this->m_connectionsLock.Enter();

    if (!this->m_connections.IsLinked(connection)) {
        this->m_connections.LinkToTail(connection);
        this->PlatformAdd(connection);
    }

    this->m_connectionsLock.Leave();
}

void WowConnectionNet::Delete(WowConnection* connection) {
    this->m_connectionsLock.Enter();

    if (connection->m_refCount == 0) {
        delete connection;
    }

    this->m_connectionsLock.Leave();
}

void WowConnectionNet::Remove(WowConnection* connection) {
    this->m_connectionsLock.Enter();

    if (this->m_connections.IsLinked(connection)) {
        this->m_connections.UnlinkNode(connection);
    }

    this->PlatformRemove(connection);

    this->m_connectionsLock.Leave();
}

void WowConnectionNet::Run() {
    this->PlatformRun();
    this->m_stopEvent.Set();
}

void WowConnectionNet::RunWorker(int32_t id) {
    if (this->m_threadinit) {
        this->m_threadinit();
    }

    this->m_workerSem.Signal(1);

    auto& worker = this->m_workers[id];

    while (true) {
        do {
            // TODO worker.time40 = OsGetAsyncTimeMsPrecise();
        } while (worker.event.Wait(1000));

        if (worker.quit) {
            break;
        }

        auto serviceConn = worker.serviceConn;
        auto serviceFlags = serviceConn->m_serviceFlags;
        serviceConn->m_serviceFlags = 0;

        this->Service(worker.serviceConn, serviceFlags);

        worker.lock.Enter();

        SInterlockedDecrement(&serviceConn->m_serviceCount);
        serviceConn->Release();
        worker.serviceConn = nullptr;

        worker.lock.Leave();

        this->m_workerSem.Signal(1);
        this->PlatformWorkerReady();
    }
}

void WowConnectionNet::Service(WowConnection* connection, uint32_t serviceFlags) {
    while (serviceFlags) {
        if (serviceFlags & 0x1) {
            connection->DoWrites();
        }

        if (serviceFlags & 0x2) {
            connection->DoReads();
        }

        if (serviceFlags & 0x4) {
            connection->DoExceptions();
        }

        if (serviceFlags & 0x8) {
            connection->DoDisconnect();
        }

        this->m_connectionsLock.Enter();

        serviceFlags = connection->m_serviceFlags;
        connection->m_serviceFlags = 0;

        this->m_connectionsLock.Leave();
    }
}

void WowConnectionNet::SignalWorker(WowConnection* connection, uint32_t flags) {
    if (!this->m_workerSem.Wait(500)) {
        connection->AddRef();
        connection->m_serviceFlags = flags;
        SInterlockedIncrement(&connection->m_serviceCount);

        int32_t i = 0;
        while (1) {
            this->m_workers[i].lock.Enter();

            if (!this->m_workers[i].serviceConn) {
                break;
            }

            STORM_ASSERT(this->m_workers[i].serviceConn != connection);

            this->m_workers[i].lock.Leave();

            i++;

            STORM_ASSERT(i < this->m_numWorkers);
        }

        this->m_workers[i].serviceConn = connection;
        this->m_workers[i].lock.Leave();
        this->m_workers[i].event.Set();
    }
}

void WowConnectionNet::Start() {
    for (int32_t i = 0; i < this->m_numWorkers; i++) {
        auto worker = &this->m_workers[i];

        worker->id = i;
        worker->serviceConn = nullptr;
        worker->quit = 0;
        worker->owner = this;
        // TODO worker.time40 = OsGetAsyncTimeMsPrecise();

        char name[32];
        SStrPrintf(name, sizeof(name), "NetThread %d", i);

        SThread::Create(&WorkerProc, worker, worker->thread, name, 0);
    }

    char name[32];
    SStrPrintf(name, sizeof(name), "Network");

    // TODO BYTE1(this->dword8EC) = 0;

    SThread::Create(&MainProc, this, this->m_thread, name, 0);

    // TODO
    // while (!BYTE1(this->dword8EC)) {
    //      OsSleep(100);
    // }
}
