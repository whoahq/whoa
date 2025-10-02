#ifndef NET_GRUNT_CLIENT_RESPONSE_HPP
#define NET_GRUNT_CLIENT_RESPONSE_HPP

#include "net/grunt/Grunt.hpp"
#include "net/login/Types.hpp"
#include <cstdint>

class CDataStore;
class LoginResponse;
struct NETADDR;

class Grunt::ClientResponse {
    public:
        virtual bool Connected(const NETADDR& addr) = 0;
        virtual bool OnlineIdle() = 0;
        virtual void GetLogonMethod() = 0;
        virtual void GetVersionProof(const uint8_t* versionChallenge) = 0;
        virtual void SetPinInfo(bool enabled, uint32_t gridSeed, const uint8_t* salt) = 0;
        virtual void SetMatrixInfo(bool enabled, uint8_t width, uint8_t height, uint8_t a5, uint8_t a6, bool a7, uint8_t challengeCount, uint64_t seed, const uint8_t* sessionKey, uint32_t a11) = 0;
        virtual void SetTokenInfo(bool enabled, uint8_t required) = 0;
        virtual void LogonResult(Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags) = 0;
        virtual void ReconnectResult(Result result, const uint8_t* sessionKey, uint32_t sessionKeyLen, uint16_t flags) = 0;
        virtual void RealmListResult(CDataStore* msg) = 0;
        virtual LOGIN_STATE NextSecurityState(LOGIN_STATE state) = 0;
        virtual int32_t GetServerId() = 0;
        virtual const uint8_t* GetVersionChallenge() = 0;
        virtual void GetRealmList() = 0;
        virtual void Reconnect() = 0;
        virtual void Logon(const char* loginServer, const char* loginPortal) = 0;
        virtual void ProveVersion(const uint8_t* versionChecksum) = 0;
        virtual void Logoff() = 0;
        virtual void Init(LoginResponse* loginResponse) = 0;
        virtual int32_t GetLoginServerType() = 0;
};

#endif
