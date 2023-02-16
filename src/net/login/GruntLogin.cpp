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

    this->m_loginResponse->UpdateLoginStatus(
        LOGIN_STATE_15,
        LOGIN_OK,
        addrStr,
        0x0
    );

    return true;
}

int32_t GruntLogin::GetLoginServerType() {
    // Grunt
    return 0;
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

#if defined(WHOA_SYSTEM_WIN)
    logon.osID = '\0Win';
#elif defined(WHOA_SYSTEM_MAC)
    logon.osID = '\0OSX';
#elif defined(WHOA_SYSTEM_LINUX)
    logon.osID = 'Linx';
#endif

    // TODO

    if (this->IsReconnect()) {
        // TODO
    } else if (this->m_password) {
        this->m_loginResponse->UpdateLoginStatus(
            LOGIN_STATE_AUTHENTICATING,
            LOGIN_OK,
            nullptr,
            0x0
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
    this->m_clientLink->GetRealmList();
}

int32_t GruntLogin::GetServerId() {
    // TODO
    return 0;
}

void GruntLogin::GetVersionProof(const uint8_t* versionChallenge) {
    if (this->IsReconnect()) {
        // TODO
    } else {
        memcpy(this->m_versionChallenge, versionChallenge, sizeof(this->m_versionChallenge));
        LOGIN_STATE nextState = this->NextSecurityState(LOGIN_STATE_FIRST_SECURITY);

        this->m_loginResponse->UpdateLoginStatus(
            nextState,
            LOGIN_OK,
            nullptr,
            0x0
        );
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

    this->m_loginResponse->UpdateLoginStatus(
        LOGIN_STATE_CONNECTING,
        LOGIN_OK,
        nullptr,
        0x0
    );

    if (!a2) {
        a2 = "us.logon.worldofwarcraft.com:3724";
    }

    this->m_clientLink->Connect(a2);
}

void GruntLogin::LogonResult(Grunt::Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags) {
    // Reconnect
    if (this->IsReconnect()) {
        // TODO
        // this->m_loginResponse->HandleRealmData(1, 0);
        this->m_clientLink->Disconnect();

        return;
    }

    // Authentication success
    if (result == Grunt::GRUNT_RESULT_0 || result == Grunt::GRUNT_RESULT_14) {
        // TODO
        // this->uint8 = 0;
        // LOBYTE(this->uint105C) = 0;

        if (sessionKeyLen >= 40) {
            sessionKeyLen = 40;
        }
        memcpy(this->m_sessionKey, sessionKey, sessionKeyLen);

        if (this->m_clientLink->m_surveyID == 0) {
            this->m_loginResponse->UpdateLoginStatus(
                LOGIN_STATE_AUTHENTICATED,
                LOGIN_OK,
                nullptr,
                flags
            );
        } else {
            this->m_loginResponse->UpdateLoginStatus(
                LOGIN_STATE_SURVEY,
                LOGIN_OK,
                nullptr,
                flags
            );
        }

        return;
    }

    // Authentication failure or file download

    // TODO this->uint8 = 1;

    switch (result) {
    case 3:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_BANNED, nullptr, 0x0);
        break;

    case 4:
    case 5: {
        LOGIN_RESULT loginResult = LOGIN_UNKNOWN_ACCOUNT;

        // TODO
        // if (LOBYTE(this->dword1100)) {
        //     loginResult = 9 - (++LOBYTE(this->dword105C) < 3u);
        // }

        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, loginResult, nullptr, 0x0);

        break;
    }

    case 6:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_ALREADYONLINE, nullptr, 0x0);
        break;

    case 7:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_NOTIME, nullptr, 0x0);
        break;

    case 8:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_DBBUSY, nullptr, 0x0);
        break;

    case 9:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_BADVERSION, nullptr, 0x0);
        break;

    case 10:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_DOWNLOADFILE, LOGIN_OK, nullptr, 0x0);
        break;

    case 12:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_SUSPENDED, nullptr, 0x0);
        break;

    case 15:
        // TODO

    case 16:
        // TODO

    case 17:
        // TODO

    case 18:
        // TODO

    case 22:
        // TODO

    case 23:
        // TODO

    case 24:
        // TODO

    case 25:
        // TODO

    case 32:
        // TODO

    // TODO case 255

    default:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_FAILED, nullptr, 0x0);
        break;
    }
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

    this->m_loginResponse->UpdateLoginStatus(
        LOGIN_STATE_HANDSHAKING,
        LOGIN_OK,
        nullptr,
        0x0
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
