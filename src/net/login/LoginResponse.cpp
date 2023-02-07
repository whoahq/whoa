#include "net/login/LoginResponse.hpp"
#include "net/grunt/Grunt.hpp"
#include <storm/String.hpp>

void LoginResponse::UpdateLoginStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, uint16_t flags) {
    this->m_loginState = state;
    this->m_loginResult = result;

    char stateStr[64];
    SStrCopy(stateStr, Grunt::g_LoginStateStringNames[state], sizeof(stateStr));

    char resultStr[64];
    SStrCopy(resultStr, Grunt::g_LoginResultStringNames[result], sizeof(resultStr));

    this->LoginServerStatus(state, result, addrStr, stateStr, resultStr, flags);
}
