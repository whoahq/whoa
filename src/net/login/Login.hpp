#ifndef NET_LOGIN_LOGIN_HPP
#define NET_LOGIN_LOGIN_HPP

#include "net/grunt/ClientResponse.hpp"

class CDataStore;
class LoginResponse;

class Login : public Grunt::ClientResponse {
    public:
        // Member variables
        bool m_loggedOn = false;
        bool m_reconnect = false;
        char m_accountName[1280] = {};
        char m_rawAccountName[1280] = {};
        char* m_password = nullptr;
        uint8_t m_sessionKey[40] = {};
        LoginResponse* m_loginResponse = nullptr;

        // Virtual member functions
        virtual ~Login();
        virtual bool OnlineIdle();
        virtual void RealmListResult(CDataStore* msg);
        virtual bool IsReconnect();
        virtual bool IsLoggedOn();

        // Member functions
        void SetLogonCreds(const char* accountName, const char* password);
};

#endif
