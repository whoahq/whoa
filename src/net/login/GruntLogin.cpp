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

    this->m_loginResponse->m_loginState = LOGIN_STATE_CONNECTED;
    this->m_loginResponse->m_loginResult = LOGIN_OK;

    char addrStr[32];
    // TODO
    // OsNetAddrToStr(addr, addrStr, sizeof(addrStr));

    this->m_loginResponse->UpdateLoginStatus(
        LOGIN_STATE_CONNECTED,
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
        logon.accountName = this->m_accountName;
        logon.password = reinterpret_cast<const char*>(this->m_sessionKey);

        this->m_clientLink->LogonStoredSession(logon);
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

const uint8_t* GruntLogin::GetVersionChallenge() {
    return this->m_versionChallenge;
}

void GruntLogin::GetVersionProof(const uint8_t* versionChallenge) {
    if (this->IsReconnect()) {
        // During reconnect, version challenge is ignored and version checksum is zeroed out
        uint8_t versionChecksum[LOGIN_VERSION_CHECKSUM_LEN] = {};

        this->m_clientLink->ProveVersion(versionChecksum);
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
    if (!this->m_loggedOn) {
        return;
    }

    this->m_clientLink->Disconnect();
}

void GruntLogin::Logon(const char* loginServer, const char* loginPortal) {
    if (this->m_loggedOn) {
        return;
    }

    this->m_reconnect = false;

    // TODO

    this->m_pinEnabled = false;
    this->m_matrixEnabled = false;
    this->m_tokenEnabled = false;

    // TODO

    this->m_loginResponse->UpdateLoginStatus(
        LOGIN_STATE_CONNECTING,
        LOGIN_OK,
        nullptr,
        0x0
    );

    if (!loginServer) {
        loginServer = "us.logon.worldofwarcraft.com:3724";
    }

    this->m_clientLink->Connect(loginServer);
}

void GruntLogin::LogonResult(Grunt::Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags) {
    using ::Grunt::Result;

    // Reconnect

    if (this->IsReconnect()) {
        this->m_loginResponse->HandleRealmData(1, nullptr);
        this->m_clientLink->Disconnect();

        return;
    }

    // Authentication success

    if (result == Result::SUCCESS || result == Result::SUCCESS_SURVEY) {
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
    case Result::BANNED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_BANNED, nullptr, 0x0);
        break;

    case Result::GRUNT_RESULT_4:
    case Result::GRUNT_RESULT_5: {
        LOGIN_RESULT loginResult = LOGIN_UNKNOWN_ACCOUNT;

        // TODO
        // if (LOBYTE(this->dword1100)) {
        //     loginResult = 9 - (++LOBYTE(this->dword105C) < 3u);
        // }

        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, loginResult, nullptr, 0x0);

        break;
    }

    case Result::ALREADYONLINE:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_ALREADYONLINE, nullptr, 0x0);
        break;

    case Result::NOTIME:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_NOTIME, nullptr, 0x0);
        break;

    case Result::DBBUSY:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_DBBUSY, nullptr, 0x0);
        break;

    case Result::BADVERSION:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_BADVERSION, nullptr, 0x0);
        break;

    case Result::DOWNLOADFILE:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_DOWNLOADFILE, LOGIN_OK, nullptr, 0x0);
        break;

    case Result::SUSPENDED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_SUSPENDED, nullptr, 0x0);
        break;

    case Result::PARENTALCONTROL:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_PARENTALCONTROL, nullptr, 0x0);
        break;

    case Result::LOCKED_ENFORCED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_LOCKED_ENFORCED, nullptr, 0x0);
        break;

    case Result::TRIAL_EXPIRED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_TRIAL_EXPIRED, nullptr, 0x0);
        break;

    case Result::ACCOUNT_CONVERTED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_ACCOUNT_CONVERTED, nullptr, 0x0);
        break;

    case Result::CHARGEBACK:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_CHARGEBACK, nullptr, 0x0);
        break;

    case Result::IGR_WITHOUT_BNET:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_IGR_WITHOUT_BNET, nullptr, 0x0);
        break;

    case Result::GAME_ACCOUNT_LOCKED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_GAME_ACCOUNT_LOCKED, nullptr, 0x0);
        break;

    case Result::UNLOCKABLE_LOCK:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_UNLOCKABLE_LOCK, nullptr, 0x0);
        break;

    case Result::CONVERSION_REQUIRED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_CONVERSION_REQUIRED, nullptr, 0x0);
        break;

    case Result::DISCONNECTED:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_DISCONNECTED, nullptr, 0x0);
        break;

    default:
        this->m_loginResponse->UpdateLoginStatus(LOGIN_STATE_FAILED, LOGIN_FAILED, nullptr, 0x0);
        break;
    }
}

LOGIN_STATE GruntLogin::NextSecurityState(LOGIN_STATE state) {
    switch (state) {
    case LOGIN_STATE_FIRST_SECURITY:
        if (this->m_pinEnabled) {
            return LOGIN_STATE_PIN;
        }

    case LOGIN_STATE_PIN:
        if (this->m_matrixEnabled) {
            return LOGIN_STATE_MATRIX;
        }

    case LOGIN_STATE_MATRIX:
        if (this->m_tokenEnabled) {
            return LOGIN_STATE_TOKEN;
        }

    default:
        return LOGIN_STATE_CHECKINGVERSIONS;
    }
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

void GruntLogin::Reconnect() {
    this->m_reconnect = true;

    this->m_clientLink->Connect(this->m_loginResponse->GetLoginServer());
}

void GruntLogin::ReconnectResult(Grunt::Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags) {
    // TODO
}

void GruntLogin::SetMatrixInfo(bool enabled, uint8_t width, uint8_t height, uint8_t a5, uint8_t a6, bool a7, uint8_t challengeCount, uint64_t seed, const uint8_t* sessionKey, uint32_t a11) {
    // TODO
}

void GruntLogin::SetPinInfo(bool enabled, uint32_t gridSeed, const uint8_t* salt) {
    // TODO
}

void GruntLogin::SetTokenInfo(bool enabled, uint8_t required) {
    // TODO
}
