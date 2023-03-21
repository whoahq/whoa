#include "net/connection/WowConnectionNet.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

void WowConnectionNet::PlatformAdd(WowConnection* connection) {
    if (!connection->m_event) {
        connection->m_event = WSACreateEvent();
    }

    uint32_t on = 1;
    setsockopt(connection->m_sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&on), sizeof(on));

    SetEvent(this->event8E8);
}

void WowConnectionNet::PlatformChangeState(WowConnection* connection, WOW_CONN_STATE state) {
    uint32_t networkEvents = 0x0;

    switch (connection->GetState()) {
    case WOWC_CONNECTING: {
        networkEvents = FD_CLOSE | FD_CONNECT | FD_WRITE;
    }

    case WOWC_LISTENING: {
        networkEvents = FD_ACCEPT;
    }

    case WOWC_CONNECTED: {
        // TODO conditional network event
        networkEvents = FD_CLOSE | FD_READ;
    }
    }

    if (connection->m_event && connection->m_sock >= 0) {
        WSAEventSelect(connection->m_sock, connection->m_event, networkEvents);
    }

    SetEvent(this->event8E8);
}

void WowConnectionNet::PlatformInit(bool useEngine) {
    WSADATA wsaData;
    auto err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err || wsaData.wVersion != MAKEWORD(2, 2)) {
        return;
    }

    this->event8E8 = CreateEvent(nullptr, true, false, nullptr);
}

void WowConnectionNet::PlatformRemove(WowConnection* connection) {
    SetEvent(this->event8E8);
}

void WowConnectionNet::PlatformRun() {
    uint32_t timeout = 500;

    WowConnection* connections[64];
    HANDLE connectionEvents[64];

    while (!this->m_stop) {
        this->m_connectionsLock.Enter();

        uint32_t connectionCount = 0;

        connections[connectionCount] = nullptr;
        connectionEvents[connectionCount] = this->event8E8;
        connectionCount++;

        for (auto connection = this->m_connections.Head(); connection; connection = this->m_connections.Next(connection)) {
            if (connection->m_serviceCount) {
                continue;
            }

            switch (connection->GetState()) {
            case WOWC_CONNECTING: {
                WSAEventSelect(connection->m_sock, connection->m_event, FD_CONNECT | FD_CLOSE);
                break;
            }

            case WOWC_LISTENING: {
                WSAEventSelect(connection->m_sock, connection->m_event, FD_ACCEPT);
                break;
            }

            case WOWC_CONNECTED: {
                // TODO conditional network event
                WSAEventSelect(connection->m_sock, connection->m_event, FD_CONNECT | FD_READ);
                break;
            }

            case WOWC_DISCONNECTED: {
                timeout = 0;
                break;
            }

            default: {
                break;
            }
            }

            if (connectionCount < 64) {
                connection->AddRef();

                connections[connectionCount] = connection;
                connectionEvents[connectionCount] = connection->m_event;
                connectionCount++;

                // TODO
            }
        }

        this->m_connectionsLock.Leave();

        auto waitIndex = WaitForMultipleObjects(connectionCount, connectionEvents, false, timeout);

        if (waitIndex == 0) {
            ResetEvent(connectionEvents[0]);
        } else {
            for (uint32_t i = 1; i < connectionCount; i++) {
                WSANETWORKEVENTS networkEvents;
                WSAEnumNetworkEvents(connections[i]->m_sock, connectionEvents[i], &networkEvents);

                uint32_t signalFlags = 0x0;
                if (networkEvents.lNetworkEvents & (FD_CLOSE | FD_ACCEPT | FD_READ)) {
                    signalFlags |= 0x2;
                }
                if (networkEvents.lNetworkEvents & (FD_CLOSE | FD_CONNECT | FD_WRITE)) {
                    signalFlags |= 0x1;
                }
                if (connections[i]->m_connState == WOWC_DISCONNECTING) {
                    signalFlags |= 0x8;
                }

                // TODO timeout manipulation

                if (signalFlags) {
                    this->SignalWorker(connections[i], signalFlags);
                }
            }
        }

        for (uint32_t i = 1; i < connectionCount; i++) {
            connections[i]->Release();
        }
    }
}

void WowConnectionNet::PlatformWorkerReady() {
    SetEvent(this->event8E8);
}
