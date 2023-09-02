#include "client/ClientServices.hpp"
#include "client/ClientRealmResponseAdapter.hpp"
#include "glue/CGlueMgr.hpp"
#include "net/Connection.hpp"
#include "net/Login.hpp"
#include "console/Client.hpp"
#include "console/CVar.hpp"
#include "console/Types.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <new>

ClientConnection* g_clientConnection;

char ClientServices::s_accountName[1280];
RealmResponse* ClientServices::s_clientRealmResponse;
ClientConnection* ClientServices::s_currentConnection;
ClientServices* ClientServices::s_instance;
Login* ClientServices::s_loginObj;
bool ClientServices::s_newLogin;
REALM_INFO ClientServices::s_selectRealmInfo;
bool ClientServices::s_selectRealmInfoValid;

CVar* s_realmNameVar = nullptr;
CVar* s_decorateAccountName = nullptr;
CVar* s_realmListBNVar = nullptr;
CVar* s_darkPortalVar = nullptr;
CVar* s_ServerAlertVar = nullptr;
CVar* s_realmListVar = nullptr;

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
    // TODO assertion?

    return ClientServices::s_currentConnection;
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
            6,
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

    auto loginServerType = ClientServices::s_loginObj->GetLoginServerType();

    auto portal = loginServerType == 1 ?
        ClientServices::s_darkPortalVar->GetString() :
        "";

    auto realmList = loginServerType == 1 ?
        ClientServices::s_realmListBNVar->GetString() :
        ClientServices::s_realmListVar->GetString();

    // TODO

    ClientServices::s_loginObj->Logon(realmList, portal);
}

void ClientServices::SelectRealm(const char* realmName) {
    STORM_ASSERT(ClientServices::s_realmNameVar);

    ClientServices::s_realmNameVar->Set(realmName, true, false, false, true);
    ClientServices::SetSelectedRealmInfo(1);
}

void ClientServices::SetAccountName(const char* accountName) {
    SStrCopy(ClientServices::s_accountName, accountName, sizeof(ClientServices::s_accountName));
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

int32_t ClientServices::GetLoginServerType() {
    if (!ClientServices::LoginConnection()) {
        return 0;
    }

    return ClientServices::LoginConnection()->GetLoginServerType();
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

const char* ClientServices::GetDefaultRealmlistString() {
    // TODO

    // return default us realmlist for now

    return "us.logon.worldofwarcraft.com:3724";
}

const char* ClientServices::GetDefaultPatchListString() {
    // TODO

    // Return default patchlist for now

    return "public-test.patch.battle.net:1119/patch";
}

void ClientServices::InitLoginServerCVars(int32_t overwrite, const char* locale) {
    if ((ClientServices::s_realmListBNVar == nullptr || ClientServices::s_realmListVar == nullptr) || overwrite != 0 ) {
        ClientServices::s_decorateAccountName = CVar::Register(
            "decorateAccountName",
            "",
            0,
            "0",
            nullptr,
            CATEGORY::NET,
            false,
            nullptr,
            false);
    }

    char localRealmList[260] = {0};
    char localDataDir[272]   = {0};

    if (locale == nullptr || *locale == '\0') {
        localDataDir[0] = '\0';
    } else {
        SStrPrintf(localDataDir, STORM_MAX_PATH, "data\\%s\\", locale);
    }

    if ((ClientServices::s_realmListBNVar == nullptr) || overwrite != 0) {
        ClientServices::s_realmListBNVar = CVar::Register(
            "realmListbn",
            "Address of Battle.net server",
            0,
            "",
            nullptr,
            CATEGORY::NET,
            false,
            nullptr,
            false);

        SStrPrintf(localRealmList, STORM_MAX_PATH, "%srealmlistbn.wtf", localDataDir);

        if (ConsoleLoadClientCVar(localRealmList) == 0) {
            ConsoleLoadClientCVar("realmlistbn.wtf");
        }
    }

    if (ClientServices::s_darkPortalVar == nullptr || overwrite != 0) {
        ClientServices::s_darkPortalVar = CVar::Register(
            "portal",
            "Name of Battle.net portal to use",
            0,
            "public-test",
            nullptr,
            CATEGORY::NET,
            false,
            nullptr,
            false);
    }

    if (ClientServices::s_ServerAlertVar == nullptr || overwrite != 0) {
        ClientServices::s_ServerAlertVar = CVar::Register(
            "serverAlert",
            "Get the glue-string tag for the URL",
            0,
            "SERVER_ALERT_URL",
            nullptr,
            CATEGORY::NET,
            false,
            nullptr,
            false);
    }

    if (ClientServices::s_ServerAlertVar == nullptr || overwrite != 0) {
        ClientServices::s_realmListVar = CVar::Register(
            "realmList",
            "Address of realm list server",
            0,
            ClientServices::GetDefaultRealmlistString(),
            nullptr,
            CATEGORY::NET,
            false,
            nullptr,
            false);

        SStrPrintf(localRealmList, STORM_MAX_PATH, "%srealmlist.wtf", localDataDir);

        if (ConsoleLoadClientCVar(localRealmList) == 0) {
            ConsoleLoadClientCVar("realmlist.wtf");
        }
    }
}
