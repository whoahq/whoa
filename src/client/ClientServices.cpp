#include "client/ClientServices.hpp"
#include "client/ClientRealmResponseAdapter.hpp"
#include "glue/CGlueMgr.hpp"
#include "net/Connection.hpp"
#include "net/Login.hpp"
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

    // TODO

    ClientServices::s_loginObj->Logon(nullptr, nullptr);
}

void ClientServices::SetAccountName(const char* accountName) {
    SStrCopy(ClientServices::s_accountName, accountName, sizeof(ClientServices::s_accountName));
}

void ClientServices::LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags) {
    CGlueMgr::SetLoginStateAndResult(state, result, addrStr, stateStr, resultStr, flags);

    if (state == LOGIN_STATE_AUTHENTICATED) {
        LoginData loginData;
        SStrCopy(loginData.m_account, ClientServices::LoginConnection()->m_accountName, sizeof(loginData.m_account));
        loginData.m_loginServerID = ClientServices::LoginConnection()->GetServerId();
        memcpy(loginData.m_sessionKey, ClientServices::LoginConnection()->m_sessionKey, sizeof(loginData.m_sessionKey));
        loginData.m_loginServerType = ClientServices::LoginConnection()->GetLoginServerType();

        if (!ClientServices::s_currentConnection) {
            // TODO
        }

        ClientServices::s_currentConnection->SetLoginData(&loginData);
        ClientServices::SetAccountName(loginData.m_account);

        CGlueMgr::SetCurrentAccount(loginData.m_account);

        // TODO
        // - initialize addons?
        // sub_5F9080(v7.m_account);

        // TODO CVar::DeleteAccountCVars();
    }
}
