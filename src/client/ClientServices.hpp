#ifndef CLIENT_CLIENT_SERVICES_HPP
#define CLIENT_CLIENT_SERVICES_HPP

#include "net/connection/NetClient.hpp"
#include "net/login/LoginResponse.hpp"

class ClientConnection;
class CVar;
class Login;
class RealmResponse;

class ClientServices : public LoginResponse {
    public:
        // Static variables
        static char s_accountName[1280];
        static RealmResponse* s_clientRealmResponse;
        static ClientConnection* s_currentConnection;
        static CVar* s_darkPortalVar;
        static CVar* s_decorateAccountName;
        static ClientServices* s_instance;
        static Login* s_loginObj;
        static bool s_newLogin;
        static CVar* s_realmListVar;
        static CVar* s_realmListBNVar;
        static CVar* s_realmNameVar;
        static REALM_INFO s_selectRealmInfo;
        static bool s_selectRealmInfoValid;
        static CVar* s_serverAlertVar;

        // Static functions
        static void ConnectToSelectedServer();
        static ClientConnection* Connection();
        static ClientServices* GetInstance();
        static REALM_INFO* GetRealmInfoByIndex(int32_t index);
        static const char* GetSelectedRealmName();
        static const REALM_INFO* GetSelectedRealm();
        static void Initialize();
        static void InitLoginServerCVars(int32_t force, const char* locale);
        static Login* LoginConnection();
        static void Logon(const char* accountName, const char* password);
        static void SelectRealm(const char* realmName);
        static void SetAccountName(const char* accountName);
        static void SetMessageHandler(NETMESSAGE msgId, MESSAGE_HANDLER handler, void* param);
        static int32_t SetSelectedRealmInfo(int32_t a1);

        // Virtual member functions
        virtual int32_t GetLoginServerType();
        virtual void LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags);
        virtual void RealmEnumCallback(uint32_t a2);
};

#endif
