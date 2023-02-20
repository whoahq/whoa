#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)

#include "net/connection/WowConnectionNet.hpp"
#include "net/connection/WowConnection.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <storm/Array.hpp>

int32_t s_workerPipe[2];

void WowConnectionNet::PlatformAdd(WowConnection* connection) {
    uint32_t on = 1;
    setsockopt(connection->m_sock, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    char buf = '\1';
    write(s_workerPipe[1], &buf, sizeof(buf));
}

void WowConnectionNet::PlatformChangeState(WowConnection* connection, WOW_CONN_STATE state) {
    char buf = '\1';
    write(s_workerPipe[1], &buf, sizeof(buf));
}

void WowConnectionNet::PlatformInit(bool useEngine) {
    // TODO
}

void WowConnectionNet::PlatformRemove(WowConnection* connection) {
    char buf = '\1';
    write(s_workerPipe[1], &buf, sizeof(buf));
}

void WowConnectionNet::PlatformRun() {
    pipe(s_workerPipe);

    if (fcntl(s_workerPipe[0], F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl(worker pipe)");
    }

    TSGrowableArray<WowConnection*> connections;
    char buf[25];

    while (!this->m_stop) {
        timeval timeout = { 30, 0 };

        fd_set readFds;
        FD_ZERO(&readFds);
        fd_set writeFds;
        FD_ZERO(&writeFds);
        fd_set errorFds;
        FD_ZERO(&errorFds);

        FD_SET(s_workerPipe[0], &readFds);

        auto fdCount = s_workerPipe[0];

        int32_t v39 = 0;
        int32_t v41 = 0;

        this->m_connectionsLock.Enter();

        for (auto connection = this->m_connections.Head(); connection; connection = this->m_connections.Link(connection)->Next()) {
            if (connection->m_serviceCount) {
                continue;
            }

            switch (connection->m_connState) {
                case WOWC_CONNECTING: {
                    FD_SET(connection->m_sock, &writeFds);
                    FD_SET(connection->m_sock, &errorFds);

                    connections.Add(1, &connection);
                    connection->AddRef();
                    fdCount = std::max(fdCount, connection->m_sock);

                    break;
                }

                case WOWC_LISTENING: {
                    FD_SET(connection->m_sock, &readFds);

                    connections.Add(1, &connection);
                    connection->AddRef();
                    fdCount = std::max(fdCount, connection->m_sock);

                    break;
                }

                case WOWC_CONNECTED: {
                    FD_SET(connection->m_sock, &readFds);
                    FD_SET(connection->m_sock, &errorFds);

                    // TODO

                    connections.Add(1, &connection);
                    connection->AddRef();
                    fdCount = std::max(fdCount, connection->m_sock);
                }

                case WOWC_DISCONNECTING: {
                    // TODO

                    v41++;
                    connections.Add(1, &connection);
                    connection->AddRef();

                    break;
                }

                default: {
                    break;
                }
            }
        }

        this->m_connectionsLock.Leave();

        if (v41 > 0) {
            timeout = { 0, 0 };
        }

        if (connections.Count() > 0) {
            // TODO
        }

        select(fdCount + 1, &readFds, &writeFds, &errorFds, &timeout);

        auto v1 = s_workerPipe[0];
        if (FD_ISSET(s_workerPipe[0], &readFds)) {
            while (read(v1, buf, 1u) > 0) {
                v1 = s_workerPipe[0];
            }
        }

        for (int32_t i = 0; i < connections.Count(); i++) {
            auto connection = connections[i];
            uint32_t signalFlags = 0x0;

            if (!(connection->m_sock & 0x80000000)) {
                if (FD_ISSET(connection->m_sock, &writeFds)) {
                    signalFlags |= 0x1;
                }

                if (FD_ISSET(connection->m_sock, &readFds)) {
                    signalFlags |= 0x2;
                }

                if (FD_ISSET(connection->m_sock, &errorFds)) {
                    signalFlags |= 0x4;
                }
            }

            if (connection->m_connState == WOWC_DISCONNECTING) {
                signalFlags |= 0x8;
            }

            // TODO
            // auto v15 = connection->dword10C;
            // if (!(v15 & 1) && v15) {
            //     signalFlags |= 0x2;
            // }

            if (signalFlags) {
                this->SignalWorker(connection, signalFlags);
            }

            connection->Release();
        }
    }
}

void WowConnectionNet::PlatformWorkerReady() {
    char buf = '\1';
    write(s_workerPipe[1], &buf, sizeof(buf));
}

#endif
