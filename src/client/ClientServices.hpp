#ifndef CLIENT_CLIENT_SERVICES_HPP
#define CLIENT_CLIENT_SERVICES_HPP

#include "net/login/LoginResponse.hpp"

class ClientConnection;
class Login;
class RealmResponse;

class ClientServices : public LoginResponse {
    public:
        // Static variables
        static char s_accountName[1280];
        static RealmResponse* s_clientRealmResponse;
        static ClientConnection* s_currentConnection;
        static ClientServices* s_instance;
        static Login* s_loginObj;
        static bool s_newLogin;

        // Static functions
        static ClientConnection* Connection();
        static ClientServices* GetInstance();
        static void Initialize();
        static Login* LoginConnection();
        static void Logon(const char* accountName, const char* password);
        static void SetAccountName(const char* accountName);

        // Virtual member functions
        virtual void LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags);
};

#endif
