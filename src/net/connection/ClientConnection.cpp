#include "net/connection/ClientConnection.hpp"
#include "net/Login.hpp"
#include "client/ClientServices.hpp"
#include "ui/FrameScript.hpp"

const char* s_errorCodeTokens[] = {
    "RESPONSE_SUCCESS",
    "RESPONSE_FAILURE",
    "RESPONSE_CANCELLED",
    "RESPONSE_DISCONNECTED",
    "RESPONSE_FAILED_TO_CONNECT",
    "RESPONSE_CONNECTED",
    "RESPONSE_VERSION_MISMATCH",
    "CSTATUS_CONNECTING",
    "CSTATUS_NEGOTIATING_SECURITY",
    "CSTATUS_NEGOTIATION_COMPLETE",
    "CSTATUS_NEGOTIATION_FAILED",
    "CSTATUS_AUTHENTICATING",
    "AUTH_OK",
    "AUTH_FAILED",
    "AUTH_REJECT",
    "AUTH_BAD_SERVER_PROOF",
    "AUTH_UNAVAILABLE",
    "AUTH_SYSTEM_ERROR",
    "AUTH_BILLING_ERROR",
    "AUTH_BILLING_EXPIRED",
    "AUTH_VERSION_MISMATCH",
    "AUTH_UNKNOWN_ACCOUNT",
    "AUTH_INCORRECT_PASSWORD",
    "AUTH_SESSION_EXPIRED",
    "AUTH_SERVER_SHUTTING_DOWN",
    "AUTH_ALREADY_LOGGING_IN",
    "AUTH_LOGIN_SERVER_NOT_FOUND",
    "AUTH_WAIT_QUEUE",
    "AUTH_BANNED",
    "AUTH_ALREADY_ONLINE",
    "AUTH_NO_TIME",
    "AUTH_DB_BUSY",
    "AUTH_SUSPENDED",
    "AUTH_PARENTAL_CONTROL",
    "AUTH_LOCKED_ENFORCED",
    "REALM_LIST_IN_PROGRESS",
    "REALM_LIST_SUCCESS",
    "REALM_LIST_FAILED",
    "REALM_LIST_INVALID",
    "REALM_LIST_REALM_NOT_FOUND",
    "ACCOUNT_CREATE_IN_PROGRESS",
    "ACCOUNT_CREATE_SUCCESS",
    "ACCOUNT_CREATE_FAILED",
    "CHAR_LIST_RETRIEVING",
    "CHAR_LIST_RETRIEVED",
    "CHAR_LIST_FAILED",
    "CHAR_CREATE_IN_PROGRESS",
    "CHAR_CREATE_SUCCESS",
    "CHAR_CREATE_ERROR",
    "CHAR_CREATE_FAILED",
    "CHAR_CREATE_NAME_IN_USE",
    "CHAR_CREATE_DISABLED",
    "CHAR_CREATE_PVP_TEAMS_VIOLATION",
    "CHAR_CREATE_SERVER_LIMIT",
    "CHAR_CREATE_ACCOUNT_LIMIT",
    "CHAR_CREATE_SERVER_QUEUE",
    "CHAR_CREATE_ONLY_EXISTING",
    "CHAR_CREATE_EXPANSION",
    "CHAR_CREATE_EXPANSION_CLASS",
    "CHAR_CREATE_LEVEL_REQUIREMENT",
    "CHAR_CREATE_UNIQUE_CLASS_LIMIT",
    "CHAR_CREATE_CHARACTER_IN_GUILD",
    "CHAR_CREATE_RESTRICTED_RACECLASS",
    "CHAR_CREATE_CHARACTER_CHOOSE_RACE",
    "CHAR_CREATE_CHARACTER_ARENA_LEADER",
    "CHAR_CREATE_CHARACTER_DELETE_MAIL",
    "CHAR_CREATE_CHARACTER_SWAP_FACTION",
    "CHAR_CREATE_CHARACTER_RACE_ONLY",
    "CHAR_CREATE_CHARACTER_GOLD_LIMIT",
    "CHAR_CREATE_FORCE_LOGIN",
    "CHAR_DELETE_IN_PROGRESS",
    "CHAR_DELETE_SUCCESS",
    "CHAR_DELETE_FAILED",
    "CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER",
    "CHAR_DELETE_FAILED_GUILD_LEADER",
    "CHAR_DELETE_FAILED_ARENA_CAPTAIN",
    "CHAR_LOGIN_IN_PROGRESS",
    "CHAR_LOGIN_SUCCESS",
    "CHAR_LOGIN_NO_WORLD",
    "CHAR_LOGIN_DUPLICATE_CHARACTER",
    "CHAR_LOGIN_NO_INSTANCES",
    "CHAR_LOGIN_FAILED",
    "CHAR_LOGIN_DISABLED",
    "CHAR_LOGIN_NO_CHARACTER",
    "CHAR_LOGIN_LOCKED_FOR_TRANSFER",
    "CHAR_LOGIN_LOCKED_BY_BILLING",
    "CHAR_LOGIN_LOCKED_BY_MOBILE_AH",
    "CHAR_NAME_SUCCESS",
    "CHAR_NAME_FAILURE",
    "CHAR_NAME_NO_NAME",
    "CHAR_NAME_TOO_SHORT",
    "CHAR_NAME_TOO_LONG",
    "CHAR_NAME_INVALID_CHARACTER",
    "CHAR_NAME_MIXED_LANGUAGES",
    "CHAR_NAME_PROFANE",
    "CHAR_NAME_RESERVED",
    "CHAR_NAME_INVALID_APOSTROPHE",
    "CHAR_NAME_MULTIPLE_APOSTROPHES",
    "CHAR_NAME_THREE_CONSECUTIVE",
    "CHAR_NAME_INVALID_SPACE",
    "CHAR_NAME_CONSECUTIVE_SPACES",
    "CHAR_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS",
    "CHAR_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END",
    "CHAR_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME",
};

void ClientConnection::AccountLogin(const char* name, const char* password, int32_t region, WOW_LOCALE locale) {
    // Assertion-like thing

    this->Initiate(COP_AUTHENTICATE, 11, nullptr);
}

void ClientConnection::AccountLogin_Finish(int32_t errorCode) {
    this->Complete(errorCode == 12, errorCode);
}

void ClientConnection::AccountLogin_Queued() {
    this->m_statusCop = COP_WAIT_QUEUE;
    this->m_errorCode = 27;
    this->m_statusComplete = 0;

    // TODO LogConnectionStatus(this->m_statusCop, 27, 1);

    // TODO CGlueMgr::UpdateWaitQueue(this->m_queuePosition);
}

void ClientConnection::Cancel(int32_t errorCode) {
    this->Complete(0, errorCode);
}

void ClientConnection::Cleanup() {
    if (this->m_cleanup) {
        this->m_cleanup();
        this->m_cleanup = nullptr;
    }
}

void ClientConnection::Complete(int32_t result, int32_t errorCode) {
    this->Cleanup();

    this->m_statusResult = result;
    this->m_errorCode = errorCode;
    this->m_statusComplete = 1;

    // TODO LogConnectionStatus(this->m_statusCop, errorCode, 0);
}

void ClientConnection::Connect() {
    // TODO

    this->m_cleanup = nullptr;
    this->m_statusCop = COP_CONNECT;
    this->m_errorCode = 7;
    this->m_statusComplete = 0;

    // TODO LogConnectionStatus(this->m_statusCop, this->m_errorCode, 1)

    if (this->m_connected) {
        this->Complete(1, 5);
    }

    ClientServices::LoginConnection()->GetRealmList();
}

int32_t ClientConnection::Disconnect() {
    // TODO
    return 0;
}

int32_t ClientConnection::HandleConnect() {
    this->Complete(1, 5);

    this->m_connected = 1;

    // TODO WardenClient_Initialize();

    return this->NetClient::HandleConnect();
}

void ClientConnection::Initiate(WOWCS_OPS op, int32_t errorCode, void (*cleanup)()) {
    this->m_cleanup = cleanup;
    this->m_statusCop = op;
    this->m_errorCode = errorCode;
    this->m_statusComplete = 0;

    // TODO LogConnectionStatus(this->m_statusCop, errorCode, 1);
}

int32_t ClientConnection::IsConnected() {
    return this->m_connected;
}

int32_t ClientConnection::PollStatus(WOWCS_OPS& op, const char** msg, int32_t& result, int32_t& errorCode) {
    op = this->m_statusCop;
    errorCode = this->m_errorCode;
    result = this->m_statusResult;

    static char altText[256];

    if (this->m_statusComplete) {
        auto text = errorCode >= 0 && errorCode < 104
            ? FrameScript_GetText(s_errorCodeTokens[errorCode], -1, GENDER_NOT_APPLICABLE)
            : nullptr;

        if (!text) {
            SStrPrintf(altText, sizeof(altText), "(%i)", errorCode);
            *msg = altText;
        } else if (errorCode == 27) {
            // TODO
            *msg = "TODO";
        } else {
            *msg = text;
        }

        return this->m_statusComplete;
    }

    *msg = "";

    return this->m_statusComplete;
}
