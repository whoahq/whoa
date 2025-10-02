#ifndef NET_LOGIN_LOGIN_RESPONSE_HPP
#define NET_LOGIN_LOGIN_RESPONSE_HPP

#include "net/login/Types.hpp"
#include "net/Types.hpp"
#include <storm/Array.hpp>

class CDataStore;

class LoginResponse {
    public:
        // Member variables
        TSFixedArray<REALM_INFO> m_realmList;
        uint32_t uint10;
        LOGIN_STATE m_loginState;
        LOGIN_RESULT m_loginResult;

        // Virtual member functions
        virtual const char* GetLoginServer() = 0;
        virtual int32_t GetLoginServerType() = 0;
        virtual void JoinRealmResult(uint32_t addr, int32_t port, int32_t a3, int32_t a4) = 0;
        virtual void LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags) = 0;
        virtual void RealmEnumCallback(uint32_t a2) = 0;

        // Member functions
        void HandleRealmData(uint32_t a2, CDataStore* msg);
        void UpdateLoginStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, uint16_t flags);
};

#endif
