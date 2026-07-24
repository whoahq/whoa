#include "net/connection/WowConnectionNet.hpp"
#include "net/connection/WowConnection.hpp"
#include "net/connection/web/WsState.hpp"
#include <storm/Atomic.hpp>
#include <emscripten.h>

// Periodic callback that polls all connections for WebSocket events.
// Replaces the native select()/thread model with an interval-based poll.
static void webNetworkPoll(void* userData) {
    auto net = static_cast<WowConnectionNet*>(userData);

    // Copy connection pointers to a local array so we can safely iterate
    // even if Service() modifies the connection list
    WowConnection* conns[64];
    int32_t count = 0;

    net->m_connectionsLock.Enter();

    for (auto conn = net->m_connections.Head(); conn && count < 64; conn = net->m_connections.Link(conn)->Next()) {
        auto wsState = static_cast<WsState*>(conn->m_event);
        if (!wsState) {
            continue;
        }

        uint32_t flags = 0;

        switch (conn->m_connState) {
            case WOWC_CONNECTING:
                if (wsState->connectPending || wsState->errorPending) {
                    flags |= 0x1; // DoWrites -> CheckConnect
                }
                break;

            case WOWC_CONNECTED:
                if (wsState->recvBuf.size > 0) {
                    flags |= 0x2; // DoReads
                }
                if (wsState->closePending || wsState->errorPending) {
                    flags |= 0x8; // DoDisconnect
                }
                break;

            case WOWC_DISCONNECTING:
                flags |= 0x8; // DoDisconnect
                break;

            default:
                break;
        }

        if (flags) {
            conn->AddRef();
            conn->m_serviceFlags = flags;
            conns[count++] = conn;
        }
    }

    net->m_connectionsLock.Leave();

    // Process connections outside the lock
    for (int32_t i = 0; i < count; i++) {
        auto conn = conns[i];
        uint32_t flags = conn->m_serviceFlags;
        conn->m_serviceFlags = 0;

        net->Service(conn, flags);

        conn->Release();
    }
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

void WowConnectionNet::PlatformAdd(WowConnection* connection) {
    // No TCP_NODELAY or fd_set management needed on web
}

void WowConnectionNet::PlatformChangeState(WowConnection* connection, WOW_CONN_STATE state) {
    // No notification needed - polling handles state changes
}

void WowConnectionNet::PlatformInit(bool useEngine) {
}

void WowConnectionNet::PlatformRemove(WowConnection* connection) {
}

void WowConnectionNet::PlatformRun() {
    // Not used on web - polling is driven by emscripten_set_interval
}

void WowConnectionNet::PlatformWorkerReady() {
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
    // Not used on web
}

void WowConnectionNet::RunWorker(int32_t id) {
    // Not used on web - no worker threads
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
    // On web, service directly instead of dispatching to worker threads
    connection->AddRef();
    connection->m_serviceFlags = flags;

    this->Service(connection, flags);

    connection->Release();
}

void WowConnectionNet::Start() {
    // Use a periodic interval instead of threads to poll connections
    emscripten_set_interval(webNetworkPoll, 16.0, this);
}
