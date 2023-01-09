#ifndef NET_LOGIN_LOGIN_RESPONSE_HPP
#define NET_LOGIN_LOGIN_RESPONSE_HPP

#include "net/Types.hpp"

class LoginResponse {
    public:
        // Member variables
        LOGIN_STATE m_loginState;
        LOGIN_RESULT m_loginResult;

        // Virtual member functions
        virtual void LoginServerStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint16_t a7) = 0;

        // Member functions
        void UpdateLoginStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* a4, uint16_t a5);
};

#endif
