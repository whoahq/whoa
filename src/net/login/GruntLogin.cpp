#include "net/login/GruntLogin.hpp"
#include "net/grunt/ClientLink.hpp"
#include "net/login/LoginResponse.hpp"
#include <cstring>
#include <new>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

GruntLogin::~GruntLogin() {
    // TODO
}

bool GruntLogin::Connected(const NETADDR& addr) {
    this->m_loggedOn = true;

    this->m_loginResponse->m_loginState = LOGIN_STATE_15;
    this->m_loginResponse->m_loginResult = LOGIN_OK;

    char addrStr[32];
    // TODO
    // OsNetAddrToStr(addr, addrStr, sizeof(addrStr));

    char stateStr[64];
    // TODO
    // SStrCopy(stateStr, g_LoginStateStringNames[LOGIN_STATE_15], sizeof(stateStr));

    char resultStr[64];
    // TODO
    // SStrCopy(resultStr, g_LoginResultStringNames[LOGIN_OK], sizeof(resultStr));

    // TODO
    this->m_loginResponse->LoginServerStatus(LOGIN_STATE_15, LOGIN_OK, addrStr, stateStr, resultStr, 0);

    return true;
}

void GruntLogin::GetLogonMethod() {
    Grunt::ClientLink::Logon logon;

    logon.accountName = nullptr;
    logon.password = nullptr;

    // TODO

    // TODO Pull build info into something common
    logon.version[0] = 3;
    logon.version[1] = 3;
    logon.version[2] = 5;
    logon.build = 12340;

    // TODO

    if (this->IsReconnect()) {
        // TODO
    } else if (this->m_password) {
        this->m_loginResponse->m_loginState = LOGIN_STATE_AUTHENTICATING;
        this->m_loginResponse->m_loginResult = LOGIN_OK;

        char stateStr[64];
        // TODO
        // SStrCopy(stateStr, g_LoginStateStringNames[LOGIN_STATE_AUTHENTICATING], sizeof(stateStr));

        char resultStr[64];
        // TODO
        // SStrCopy(resultStr, g_LoginResultStringNames[LOGIN_OK], sizeof(resultStr));

        this->m_loginResponse->LoginServerStatus(
            LOGIN_STATE_AUTHENTICATING,
            LOGIN_OK,
            nullptr,
            stateStr,
            resultStr,
            0
        );

        logon.password = this->m_password;
        logon.accountName = this->m_accountName;
        this->m_clientLink->LogonNewSession(logon);

        auto passwordLen = SStrLen(this->m_password);
        memset(this->m_password, 0, passwordLen);
        SMemFree(this->m_password, __FILE__, __LINE__, 0);
        this->m_password = nullptr;
    }
}

void GruntLogin::GetRealmList() {
    // TODO
}

void GruntLogin::GetVersionProof(const uint8_t* versionChallenge) {
    if (this->IsReconnect()) {
        // TODO
    } else {
        memcpy(this->m_versionChallenge, versionChallenge, sizeof(this->m_versionChallenge));
        LOGIN_STATE nextState = this->NextSecurityState(LOGIN_STATE_FIRST_SECURITY);
        this->m_loginResponse->UpdateLoginStatus(nextState, LOGIN_OK, nullptr, 0);
    }
}

void GruntLogin::Init(LoginResponse* loginResponse) {
    this->m_loginResponse = loginResponse;

    auto clientLinkMem = SMemAlloc(sizeof(Grunt::ClientLink), __FILE__, __LINE__, 0x0);
    auto clientLink = new (clientLinkMem) Grunt::ClientLink(*this);
    this->m_clientLink = clientLink;
}

void GruntLogin::Logoff() {
    // TODO
}

void GruntLogin::Logon(const char* a2, const char* a3) {
    if (this->m_loggedOn) {
        return;
    }

    this->m_reconnect = false;

    // TODO

    this->m_loginResponse->m_loginState = LOGIN_STATE_CONNECTING;
    this->m_loginResponse->m_loginResult = LOGIN_OK;

    // TODO
    // char v6[64], v7[64];
    // SStrCopy(v6, g_LoginStateStringNames[1], sizeof(v6));
    // SStrCopy(v7, g_LoginResultStringNames[0], sizeof(v7));
    // this->m_loginResponse->Vfunc6(1, 0, 0, v6, v7, 0);

    if (!a2) {
        a2 = "us.logon.worldofwarcraft.com:3724";
    }

    this->m_clientLink->Connect(a2);
}

void GruntLogin::LogonResult(Grunt::Result result, const uint8_t* a3, uint32_t a4, uint16_t a5) {
    // TODO
}

LOGIN_STATE GruntLogin::NextSecurityState(LOGIN_STATE state) {
    // TODO
    return LOGIN_STATE_CHECKINGVERSIONS;
}

void GruntLogin::ProveVersion(const uint8_t* versionChecksum) {
    if (!this->m_loggedOn) {
        return;
    }

    this->m_clientLink->ProveVersion(versionChecksum);

    this->m_loginResponse->m_loginState = LOGIN_STATE_HANDSHAKING;
    this->m_loginResponse->m_loginResult = LOGIN_OK;

    char stateStr[64];
    SStrCopy(stateStr, Grunt::g_LoginStateStringNames[LOGIN_STATE_HANDSHAKING], sizeof(stateStr));

    char resultStr[64];
    SStrCopy(resultStr, Grunt::g_LoginResultStringNames[LOGIN_OK], sizeof(resultStr));

    this->m_loginResponse->LoginServerStatus(
        LOGIN_STATE_HANDSHAKING,
        LOGIN_OK,
        nullptr,
        stateStr,
        resultStr,
        0
    );
}

void GruntLogin::SetMatrixInfo(bool enabled, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6, bool a7, uint8_t a8, uint64_t a9, const uint8_t* a10, uint32_t a11) {
    // TODO
}

void GruntLogin::SetPinInfo(bool enabled, uint32_t a3, const uint8_t* a4) {
    // TODO
}

void GruntLogin::SetTokenInfo(bool enabled, uint8_t tokenRequired) {
    // TODO
}
