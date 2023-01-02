#ifndef NET_GRUNT_CLIENT_RESPONSE_HPP
#define NET_GRUNT_CLIENT_RESPONSE_HPP

#include "net/grunt/Grunt.hpp"
#include "net/Types.hpp"

class LoginResponse;

class Grunt::ClientResponse {
    public:
        virtual bool Connected(const NETADDR& addr) = 0;
        virtual bool OnlineIdle() = 0;
        virtual void GetLogonMethod() = 0;
        virtual void GetRealmList() = 0;
        virtual void Logon(const char* a2, const char* a3) = 0;
        virtual void Logoff() = 0;
        virtual void Init(LoginResponse* loginResponse) = 0;
};

#endif
