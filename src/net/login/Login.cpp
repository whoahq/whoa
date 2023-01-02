#include "net/login/Login.hpp"
#include <cstring>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

Login::~Login() {
    // TODO
}

bool Login::IsReconnect() {
    return this->m_reconnect;
}

bool Login::OnlineIdle() {
    // TODO

    return true;
}

void Login::SetLogonCreds(const char* accountName, const char* password) {
    SStrCopy(this->m_accountName, accountName, 1280);

    if (this->m_password) {
        memset(this->m_password, 0, SStrLen(this->m_password));
        SMemFree(this->m_password, __FILE__, __LINE__, 0x0);
    }

    this->m_password = SStrDupA(password, __FILE__, __LINE__);
}
