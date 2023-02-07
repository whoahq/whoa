#ifndef NET_LOGIN_GRUNT_LOGIN_HPP
#define NET_LOGIN_GRUNT_LOGIN_HPP

#include "net/grunt/Grunt.hpp"
#include "net/grunt/ClientResponse.hpp"
#include "net/login/Login.hpp"

class GruntLogin : public Login {
    public:
        // Member variables
        uint8_t m_versionChallenge[16];
        Grunt::ClientLink* m_clientLink = nullptr;

        // Virtual member functions
        virtual ~GruntLogin();
        virtual bool Connected(const NETADDR& addr);
        virtual void GetLogonMethod();
        virtual void GetVersionProof(const uint8_t* versionChallenge);
        virtual void SetPinInfo(bool enabled, uint32_t a3, const uint8_t* a4);
        virtual void SetMatrixInfo(bool enabled, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6, bool a7, uint8_t a8, uint64_t a9, const uint8_t* a10, uint32_t a11);
        virtual void SetTokenInfo(bool enabled, uint8_t tokenRequired);
        virtual void LogonResult(Grunt::Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags);
        virtual LOGIN_STATE NextSecurityState(LOGIN_STATE state);
        virtual int32_t GetServerId();
        virtual void GetRealmList();
        virtual void Logon(const char* a2, const char* a3);
        virtual void ProveVersion(const uint8_t* versionChecksum);
        virtual void Logoff();
        virtual void Init(LoginResponse* loginResponse);
        virtual int32_t GetLoginServerType();
};

#endif
