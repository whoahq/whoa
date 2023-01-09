#include "net/login/LoginResponse.hpp"

void LoginResponse::UpdateLoginStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* a4, uint16_t a5) {
    this->m_loginState = state;
    this->m_loginResult = result;

    // TODO string lookups
    const char* stateStr = nullptr;
    const char* resultStr = nullptr;

    this->LoginServerStatus(state, result, a4, stateStr, resultStr, a5);
}
