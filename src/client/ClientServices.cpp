#include "client/ClientServices.hpp"
#include "client/ClientRealmResponseAdapter.hpp"
#include "console/CVar.hpp"
#include "console/Command.hpp"
#include "glue/CGlueMgr.hpp"
#include "net/Connection.hpp"
#include "net/Login.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <new>

ClientConnection* g_clientConnection;

char ClientServices::s_accountName[1280];
CHARACTER_INFO ClientServices::s_characterInfo;
RealmResponse* ClientServices::s_clientRealmResponse;
ClientConnection* ClientServices::s_currentConnection;
CVar* ClientServices::s_darkPortalVar;
CVar* ClientServices::s_decorateAccountName;
ClientServices* ClientServices::s_instance;
Login* ClientServices::s_loginObj;
bool ClientServices::s_newLogin;
CVar* ClientServices::s_realmListVar;
CVar* ClientServices::s_realmListBNVar;
CVar* ClientServices::s_realmNameVar;
REALM_INFO ClientServices::s_selectRealmInfo;
bool ClientServices::s_selectRealmInfoValid;
CVar* ClientServices::s_serverAlertVar;

const char* s_errorCodeTokens[] = {
    "RESPONSE_SUCCESS",
    "RESPONSE_FAILURE",
    "RESPONSE_CANCELLED",
    "RESPONSE_DISCONNECTED",
    "RESPONSE_FAILED_TO_CONNECT",
    "RESPONSE_CONNECTED",
    "RESPONSE_VERSION_MISMATCH",
    "CSTATUS_CONNECTING",
    "CSTATUS_NEGOTIATING_SECURITY",
    "CSTATUS_NEGOTIATION_COMPLETE",
    "CSTATUS_NEGOTIATION_FAILED",
    "CSTATUS_AUTHENTICATING",
    "AUTH_OK",
    "AUTH_FAILED",
    "AUTH_REJECT",
    "AUTH_BAD_SERVER_PROOF",
    "AUTH_UNAVAILABLE",
    "AUTH_SYSTEM_ERROR",
    "AUTH_BILLING_ERROR",
    "AUTH_BILLING_EXPIRED",
    "AUTH_VERSION_MISMATCH",
    "AUTH_UNKNOWN_ACCOUNT",
    "AUTH_INCORRECT_PASSWORD",
    "AUTH_SESSION_EXPIRED",
    "AUTH_SERVER_SHUTTING_DOWN",
    "AUTH_ALREADY_LOGGING_IN",
    "AUTH_LOGIN_SERVER_NOT_FOUND",
    "AUTH_WAIT_QUEUE",
    "AUTH_BANNED",
    "AUTH_ALREADY_ONLINE",
    "AUTH_NO_TIME",
    "AUTH_DB_BUSY",
    "AUTH_SUSPENDED",
    "AUTH_PARENTAL_CONTROL",
    "AUTH_LOCKED_ENFORCED",
    "REALM_LIST_IN_PROGRESS",
    "REALM_LIST_SUCCESS",
    "REALM_LIST_FAILED",
    "REALM_LIST_INVALID",
    "REALM_LIST_REALM_NOT_FOUND",
    "ACCOUNT_CREATE_IN_PROGRESS",
    "ACCOUNT_CREATE_SUCCESS",
    "ACCOUNT_CREATE_FAILED",
    "CHAR_LIST_RETRIEVING",
    "CHAR_LIST_RETRIEVED",
    "CHAR_LIST_FAILED",
    "CHAR_CREATE_IN_PROGRESS",
    "CHAR_CREATE_SUCCESS",
    "CHAR_CREATE_ERROR",
    "CHAR_CREATE_FAILED",
    "CHAR_CREATE_NAME_IN_USE",
    "CHAR_CREATE_DISABLED",
    "CHAR_CREATE_PVP_TEAMS_VIOLATION",
    "CHAR_CREATE_SERVER_LIMIT",
    "CHAR_CREATE_ACCOUNT_LIMIT",
    "CHAR_CREATE_SERVER_QUEUE",
    "CHAR_CREATE_ONLY_EXISTING",
    "CHAR_CREATE_EXPANSION",
    "CHAR_CREATE_EXPANSION_CLASS",
    "CHAR_CREATE_LEVEL_REQUIREMENT",
    "CHAR_CREATE_UNIQUE_CLASS_LIMIT",
    "CHAR_CREATE_CHARACTER_IN_GUILD",
    "CHAR_CREATE_RESTRICTED_RACECLASS",
    "CHAR_CREATE_CHARACTER_CHOOSE_RACE",
    "CHAR_CREATE_CHARACTER_ARENA_LEADER",
    "CHAR_CREATE_CHARACTER_DELETE_MAIL",
    "CHAR_CREATE_CHARACTER_SWAP_FACTION",
    "CHAR_CREATE_CHARACTER_RACE_ONLY",
    "CHAR_CREATE_CHARACTER_GOLD_LIMIT",
    "CHAR_CREATE_FORCE_LOGIN",
    "CHAR_DELETE_IN_PROGRESS",
    "CHAR_DELETE_SUCCESS",
    "CHAR_DELETE_FAILED",
    "CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER",
    "CHAR_DELETE_FAILED_GUILD_LEADER",
    "CHAR_DELETE_FAILED_ARENA_CAPTAIN",
    "CHAR_LOGIN_IN_PROGRESS",
    "CHAR_LOGIN_SUCCESS",
    "CHAR_LOGIN_NO_WORLD",
    "CHAR_LOGIN_DUPLICATE_CHARACTER",
    "CHAR_LOGIN_NO_INSTANCES",
    "CHAR_LOGIN_FAILED",
    "CHAR_LOGIN_DISABLED",
    "CHAR_LOGIN_NO_CHARACTER",
    "CHAR_LOGIN_LOCKED_FOR_TRANSFER",
    "CHAR_LOGIN_LOCKED_BY_BILLING",
    "CHAR_LOGIN_LOCKED_BY_MOBILE_AH",
    "CHAR_NAME_SUCCESS",
    "CHAR_NAME_FAILURE",
    "CHAR_NAME_NO_NAME",
    "CHAR_NAME_TOO_SHORT",
    "CHAR_NAME_TOO_LONG",
    "CHAR_NAME_INVALID_CHARACTER",
    "CHAR_NAME_MIXED_LANGUAGES",
    "CHAR_NAME_PROFANE",
    "CHAR_NAME_RESERVED",
    "CHAR_NAME_INVALID_APOSTROPHE",
    "CHAR_NAME_MULTIPLE_APOSTROPHES",
    "CHAR_NAME_THREE_CONSECUTIVE",
    "CHAR_NAME_INVALID_SPACE",
    "CHAR_NAME_CONSECUTIVE_SPACES",
    "CHAR_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS",
    "CHAR_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END",
    "CHAR_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME",
};

void ClientServices::ConnectToSelectedServer() {
    if (!ClientServices::s_selectRealmInfoValid && !ClientServices::SetSelectedRealmInfo(0)) {
        ClientServices::Connection()->Complete(0, 39);
        return;
    }

    if (ClientServices::Connection()->GetState() != NS_INITIALIZED) {
        ClientServices::Connection()->Complete(0, 39);
        return;
    }

    ClientServices::Connection()->SetSelectedRealm(
        ClientServices::s_selectRealmInfo.uint130,
        ClientServices::s_selectRealmInfo.uint134,
        ClientServices::s_selectRealmInfo.sort
    );

    ClientServices::Connection()->NetClient::Connect(ClientServices::s_selectRealmInfo.address);
}

ClientConnection* ClientServices::Connection() {
    STORM_ASSERT(ClientServices::s_currentConnection);
    return ClientServices::s_currentConnection;
}

const char* ClientServices::GetCurrentLoginPortal() {
    return ClientServices::s_loginObj->GetLoginServerType() == 1
        ? ClientServices::s_darkPortalVar->GetString()
        : "";
}

const char* ClientServices::GetCurrentLoginServer() {
    return ClientServices::s_loginObj->GetLoginServerType() == 1
        ? ClientServices::s_realmListBNVar->GetString()
        : ClientServices::s_realmListVar->GetString();
}

const char* ClientServices::GetErrorToken(int32_t errorCode) {
    if (errorCode >= 104) {
        return "";
    }

    return s_errorCodeTokens[errorCode];
}

ClientServices* ClientServices::GetInstance() {
    if (ClientServices::s_instance) {
        return ClientServices::s_instance;
    }

    auto m = SMemAlloc(sizeof(ClientServices), __FILE__, __LINE__, 0x0);
    auto instance = new (m) ClientServices();
    ClientServices::s_instance = instance;

    return ClientServices::s_instance;
}

REALM_INFO* ClientServices::GetRealmInfoByIndex(int32_t index) {
    if (index >= ClientServices::GetInstance()->m_realmList.Count()) {
        return nullptr;
    }

    return &ClientServices::GetInstance()->m_realmList[index];
}

const char* ClientServices::GetSelectedRealmName() {
    if (!ClientServices::s_realmNameVar) {
        ClientServices::s_realmNameVar = CVar::Register(
            "realmName",
            "Last realm connected to",
            0,
            "",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );
    }

    return ClientServices::s_realmNameVar->GetString();
}

const REALM_INFO* ClientServices::GetSelectedRealm() {
    return &ClientServices::s_selectRealmInfo;
}

void ClientServices::Initialize() {
    if (!g_clientConnection) {
        auto adapterMem = SMemAlloc(sizeof(ClientRealmResponseAdapter), __FILE__, __LINE__, 0x0);
        auto clientRealmResponse = new (adapterMem) ClientRealmResponseAdapter();
        ClientServices::s_clientRealmResponse = clientRealmResponse;

        auto connectionMem = SMemAlloc(sizeof(ClientConnection), __FILE__, __LINE__, 0x0);
        auto clientConnection = new (connectionMem) ClientConnection(ClientServices::s_clientRealmResponse);
        g_clientConnection = clientConnection;
    }

    ClientServices::s_currentConnection = g_clientConnection;

    // TODO ConsoleCommandRegister("logout", &Sub6B2030, 5, nullptr);
}

void ClientServices::InitLoginServerCVars(int32_t force, const char* locale) {
    if (!ClientServices::s_realmListBNVar || !ClientServices::s_realmListVar || force) {
        ClientServices::s_decorateAccountName = CVar::Register(
            "decorateAccountName",
            "",
            0x0,
            "0",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );
    }

    char dataPath[STORM_MAX_PATH];
    if (locale && *locale) {
        SStrPrintf(dataPath, sizeof(dataPath), "data\\%s\\", locale);
    } else {
        dataPath[0] = '\0';
    }

    if (!ClientServices::s_realmListBNVar || force) {
        ClientServices::s_realmListBNVar = CVar::Register(
            "realmListbn",
            "Address of Battle.net server",
            0x0,
            "",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );

        char realmListPath[STORM_MAX_PATH];
        SStrPrintf(realmListPath, sizeof(realmListPath), "%srealmlistbn.wtf", dataPath);

        if (!CVar::Load(realmListPath)) {
            CVar::Load("realmlistbn.wtf");
        }
    }

    if (!ClientServices::s_darkPortalVar || force) {
        ClientServices::s_darkPortalVar = CVar::Register(
            "portal",
            "Name of Battle.net portal to use",
            0x0,
            "",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );
    }

    if (!ClientServices::s_serverAlertVar || force) {
        ClientServices::s_serverAlertVar = CVar::Register(
            "serverAlert",
            "Get the glue-string tag for the URL",
            0x0,
            "SERVER_ALERT_URL",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );
    }

    if (!ClientServices::s_realmListVar || force) {
        ClientServices::s_realmListVar = CVar::Register(
            "realmList",
            "Address of realm list server",
            0x0,
            "us.logon.worldofwarcraft.com:3724",
            nullptr,
            NET,
            false,
            nullptr,
            false
        );

        char realmListPath[STORM_MAX_PATH];
        SStrPrintf(realmListPath, sizeof(realmListPath), "%srealmlist.wtf", dataPath);

        if (!CVar::Load(realmListPath)) {
            CVar::Load("realmlist.wtf");
        }
    }
}

Login* ClientServices::LoginConnection() {
    return ClientServices::s_loginObj;
}

void ClientServices::Logon(const char* accountName, const char* password) {
    if (ClientServices::s_newLogin) {
        if (ClientServices::s_loginObj) {
            // TODO
            // ClientServices::s_loginObj->Vfunc48(1);

            ClientServices::s_loginObj = nullptr;
        }

        ClientServices::s_newLogin = false;
    }

    auto useBattlenet = SStrChr(accountName, '@') != 0;

    Login* loginObj;

    if (useBattlenet) {
        // TODO
    } else {
        auto loginMem = SMemAlloc(sizeof(GruntLogin), __FILE__, __LINE__, 0x0);
        loginObj = new (loginMem) GruntLogin();
    }

    ClientServices::s_loginObj = loginObj;
    ClientServices::s_loginObj->Init(ClientServices::GetInstance());

    // TODO

    ClientServices::s_loginObj->SetLogonCreds(accountName, password);

    ClientServices::s_loginObj->Logon(
        ClientServices::GetCurrentLoginServer(),
        ClientServices::GetCurrentLoginPortal()
    );
}

void ClientServices::SelectRealm(const char* realmName) {
    STORM_ASSERT(ClientServices::s_realmNameVar);

    ClientServices::s_realmNameVar->Set(realmName, true, false, false, true);
    ClientServices::SetSelectedRealmInfo(1);
}

void ClientServices::SetAccountName(const char* accountName) {
    SStrCopy(ClientServices::s_accountName, accountName, sizeof(ClientServices::s_accountName));
}

void ClientServices::SetCharacterInfo(const CHARACTER_INFO* info) {
    STORM_ASSERT(info);

    ClientServices::s_characterInfo = *info;
}

void ClientServices::SetMessageHandler(NETMESSAGE msgId, MESSAGE_HANDLER handler, void* param) {
    STORM_ASSERT(handler);
    ClientServices::Connection()->SetMessageHandler(msgId, handler, param);
}

int32_t ClientServices::SetSelectedRealmInfo(int32_t a1) {
    auto instance = ClientServices::GetInstance();

    for (uint32_t index = 0; index < instance->m_realmList.Count(); index++) {
        auto& realmInfo = instance->m_realmList[index];

        if (!SStrCmpI(realmInfo.name, ClientServices::GetSelectedRealmName(), STORM_MAX_STR) /* TODO SStrCmpUTF8I */) {
            if (!(realmInfo.flags & 0x1) || a1) {
                ClientServices::s_selectRealmInfoValid = true;
                memcpy(&ClientServices::s_selectRealmInfo, &realmInfo, sizeof(ClientServices::s_selectRealmInfo));

                return 1;
            }
        }
    }

    ClientServices::s_selectRealmInfoValid = false;

    return 0;
}

const char* ClientServices::GetLoginServer() {
    return ClientServices::s_loginObj->GetLoginServerType() == 1
        ? ClientServices::s_realmListBNVar->GetString()
        : ClientServices::s_realmListVar->GetString();
}

int32_t ClientServices::GetLoginServerType() {
    if (!ClientServices::LoginConnection()) {
        return 0;
    }

    return ClientServices::LoginConnection()->GetLoginServerType();
}

void ClientServices::JoinRealmResult(uint32_t addr, int32_t port, int32_t a3, int32_t a4) {
    // TODO
}

void ClientServices::LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags) {
    CGlueMgr::SetLoginStateAndResult(state, result, addrStr, stateStr, resultStr, flags);

    if (state == LOGIN_STATE_AUTHENTICATED) {
        LoginData loginData;
        SStrCopy(loginData.m_account, ClientServices::LoginConnection()->m_accountName, sizeof(loginData.m_account));
        loginData.m_loginServerID = ClientServices::LoginConnection()->GetServerId();
        memcpy(loginData.m_sessionKey, ClientServices::LoginConnection()->m_sessionKey, sizeof(loginData.m_sessionKey));
        loginData.m_loginServerType = ClientServices::LoginConnection()->GetLoginServerType();

        ClientServices::Connection()->SetLoginData(&loginData);
        ClientServices::SetAccountName(loginData.m_account);

        CGlueMgr::SetCurrentAccount(loginData.m_account);

        // TODO
        // - initialize addons?
        // sub_5F9080(v7.m_account);

        // TODO CVar::DeleteAccountCVars();
    }
}

void ClientServices::RealmEnumCallback(uint32_t a2) {
    auto connection = ClientServices::Connection();

    if (a2 == 1) {
        connection->Complete(0, 23);
        return;
    }

    if (a2 == 2 || a2 == 3 || a2 == 4) {
        connection->Complete(0, 37);
        return;
    }

    // TODO statusCop checks

    if (ClientServices::LoginConnection()->GetLoginServerType() == 1) {
        // TODO Battlenet logic

        return;
    }

    ClientServices::ConnectToSelectedServer();
}
