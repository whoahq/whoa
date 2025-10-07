#include "net/connection/ClientConnection.hpp"
#include "net/Login.hpp"
#include "client/ClientServices.hpp"
#include "ui/FrameScript.hpp"

void ClientConnection::AccountLogin(const char* name, const char* password, int32_t region, WOW_LOCALE locale) {
    // Assertion-like thing

    this->Initiate(COP_AUTHENTICATE, 11, nullptr);
}

void ClientConnection::AccountLogin_Finish(int32_t errorCode) {
    this->Complete(errorCode == 12, errorCode);
}

void ClientConnection::AccountLogin_Queued() {
    this->Initiate(COP_WAIT_QUEUE, 27, nullptr);

    // TODO CGlueMgr::UpdateWaitQueue(this->m_queuePosition);
}

void ClientConnection::Cancel(int32_t errorCode) {
    this->Complete(0, errorCode);
}

void ClientConnection::CharacterLogin(uint64_t guid, int32_t a2) {
    this->Initiate(COP_LOGIN_CHARACTER, 76, nullptr);

    if (this->m_connected) {
        this->RequestCharacterLogin(guid, a2);
    } else {
        this->Cancel(4);
    }
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

    this->Initiate(COP_CONNECT, 7, nullptr);

    if (this->m_connected) {
        this->Complete(1, 5);
    }

    ClientServices::LoginConnection()->GetRealmList();
}

int32_t ClientConnection::Disconnect() {
    this->NetClient::Disconnect();

    this->m_connected = 0;

    // TODO
    // WardenClient_Destroy();

    return 1;
}

void ClientConnection::EnumerateCharacters(ENUMERATE_CHARACTERS_CALLBACK callback, void* param) {
    // TODO Assertion-like thing

    for (uint32_t i = 0; i < this->m_characterList.Count(); i++) {
        callback(this->m_characterList[i], param);
    }
}

void ClientConnection::GetCharacterList() {
    this->Initiate(COP_GET_CHARACTERS, 43, nullptr);

    if (this->m_connected) {
        this->RequestCharacterEnum();
    } else {
        this->Cancel(4);
    }
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
            ? FrameScript_GetText(ClientServices::GetErrorToken(errorCode), -1, GENDER_NOT_APPLICABLE)
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
