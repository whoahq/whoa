#ifndef NET_LOGIN_GRUNT_LOGIN_HPP
#define NET_LOGIN_GRUNT_LOGIN_HPP

#include "net/grunt/Grunt.hpp"
#include "net/grunt/ClientResponse.hpp"
#include "net/login/Login.hpp"

class GruntLogin : public Login {
    public:
        // Member variables
        Grunt::ClientLink* m_clientLink = nullptr;

        // Virtual member functions
        virtual ~GruntLogin();
        virtual bool Connected(const NETADDR& addr);
        virtual void GetLogonMethod();
        virtual void GetRealmList();
        virtual void Logon(const char* a2, const char* a3);
        virtual void Logoff();
        virtual void Init(LoginResponse* loginResponse);
};

#endif
