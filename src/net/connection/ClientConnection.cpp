#include "net/connection/ClientConnection.hpp"
#include "net/Login.hpp"
#include "client/ClientServices.hpp"

void ClientConnection::Cancel(int32_t errorCode) {
    this->Cleanup();

    this->m_statusResult = 0;
    this->m_errorCode = errorCode;
    this->m_statusComplete = 1;

    // TODO
    // LogConnectionStatus(this->m_statusCop, errorCode, 0);
}

void ClientConnection::Cleanup() {
    if (this->m_cleanup) {
        this->m_cleanup();
        this->m_cleanup = nullptr;
    }
}

void ClientConnection::Connect() {
    // TODO

    this->m_cleanup = nullptr;
    this->m_statusCop = COP_CONNECT;
    this->m_errorCode = 7;
    this->m_statusComplete = 0;

    // TODO

    ClientServices::LoginConnection()->GetRealmList();
}
