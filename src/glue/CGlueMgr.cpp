#include "glue/CGlueMgr.hpp"
#include "client/Client.hpp"
#include "client/ClientServices.hpp"
#include "client/Util.hpp"
#include "console/CVar.hpp"
#include "db/Db.hpp"
#include "glue/CCharacterSelection.hpp"
#include "glue/Character.hpp"
#include "glue/CRealmList.hpp"
#include "glue/GlueScript.hpp"
#include "glue/Types.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
#include "gx/LoadingScreen.hpp"
#include "math/Utils.hpp"
#include "net/Connection.hpp"
#include "net/Login.hpp"
#include "sound/SI2.hpp"
#include "ui/CSimpleModelFFX.hpp"
#include "ui/CSimpleTop.hpp"
#include "ui/FrameScript.hpp"
#include "ui/FrameXML.hpp"
#include "ui/Interface.hpp"
#include "ui/ScriptFunctions.hpp"
#include "util/Filesystem.hpp"
#include "util/Locale.hpp"
#include "util/Log.hpp"
#include <common/DataStore.hpp>
#include <common/MD5.hpp>
#include <cstdio>

unsigned char InterfaceKey[256] = {
    0xC3, 0x5B, 0x50, 0x84, 0xB9, 0x3E, 0x32, 0x42, 0x8C, 0xD0, 0xC7, 0x48, 0xFA, 0x0E, 0x5D, 0x54,
    0x5A, 0xA3, 0x0E, 0x14, 0xBA, 0x9E, 0x0D, 0xB9, 0x5D, 0x8B, 0xEE, 0xB6, 0x84, 0x93, 0x45, 0x75,
    0xFF, 0x31, 0xFE, 0x2F, 0x64, 0x3F, 0x3D, 0x6D, 0x07, 0xD9, 0x44, 0x9B, 0x40, 0x85, 0x59, 0x34,
    0x4E, 0x10, 0xE1, 0xE7, 0x43, 0x69, 0xEF, 0x7C, 0x16, 0xFC, 0xB4, 0xED, 0x1B, 0x95, 0x28, 0xA8,
    0x23, 0x76, 0x51, 0x31, 0x57, 0x30, 0x2B, 0x79, 0x08, 0x50, 0x10, 0x1C, 0x4A, 0x1A, 0x2C, 0xC8,
    0x8B, 0x8F, 0x05, 0x2D, 0x22, 0x3D, 0xDB, 0x5A, 0x24, 0x7A, 0x0F, 0x13, 0x50, 0x37, 0x8F, 0x5A,
    0xCC, 0x9E, 0x04, 0x44, 0x0E, 0x87, 0x01, 0xD4, 0xA3, 0x15, 0x94, 0x16, 0x34, 0xC6, 0xC2, 0xC3,
    0xFB, 0x49, 0xFE, 0xE1, 0xF9, 0xDA, 0x8C, 0x50, 0x3C, 0xBE, 0x2C, 0xBB, 0x57, 0xED, 0x46, 0xB9,
    0xAD, 0x8B, 0xC6, 0xDF, 0x0E, 0xD6, 0x0F, 0xBE, 0x80, 0xB3, 0x8B, 0x1E, 0x77, 0xCF, 0xAD, 0x22,
    0xCF, 0xB7, 0x4B, 0xCF, 0xFB, 0xF0, 0x6B, 0x11, 0x45, 0x2D, 0x7A, 0x81, 0x18, 0xF2, 0x92, 0x7E,
    0x98, 0x56, 0x5D, 0x5E, 0x69, 0x72, 0x0A, 0x0D, 0x03, 0x0A, 0x85, 0xA2, 0x85, 0x9C, 0xCB, 0xFB,
    0x56, 0x6E, 0x8F, 0x44, 0xBB, 0x8F, 0x02, 0x22, 0x68, 0x63, 0x97, 0xBC, 0x85, 0xBA, 0xA8, 0xF7,
    0xB5, 0x40, 0x68, 0x3C, 0x77, 0x86, 0x6F, 0x4B, 0xD7, 0x88, 0xCA, 0x8A, 0xD7, 0xCE, 0x36, 0xF0,
    0x45, 0x6E, 0xD5, 0x64, 0x79, 0x0F, 0x17, 0xFC, 0x64, 0xDD, 0x10, 0x6F, 0xF3, 0xF5, 0xE0, 0xA6,
    0xC3, 0xFB, 0x1B, 0x8C, 0x29, 0xEF, 0x8E, 0xE5, 0x34, 0xCB, 0xD1, 0x2A, 0xCE, 0x79, 0xC3, 0x9A,
    0x0D, 0x36, 0xEA, 0x01, 0xE0, 0xAA, 0x91, 0x20, 0x54, 0xF0, 0x72, 0xD8, 0x1E, 0xC7, 0x89, 0xD2
};

int32_t CGlueMgr::m_acceptedEULA = 1; // TODO
int32_t CGlueMgr::m_acceptedTerminationWithoutNotice;
int32_t CGlueMgr::m_acceptedTOS = 1; // TODO
int32_t CGlueMgr::m_accountMsgAvailable;
char CGlueMgr::m_accountName[1280];
float CGlueMgr::m_aspect;
bool CGlueMgr::m_authenticated;
const CharacterSelectionDisplay* CGlueMgr::m_characterInfo;
int32_t CGlueMgr::m_clientKickReason;
char CGlueMgr::m_currentScreen[64];
int32_t CGlueMgr::m_disconnectPending;
int32_t CGlueMgr::m_displayingQueueDialog;
CGlueMgr::GLUE_IDLE_STATE CGlueMgr::m_idleState;
int32_t CGlueMgr::m_initialized;
int32_t CGlueMgr::m_lastLoginResult;
int32_t CGlueMgr::m_lastLoginState;
int32_t CGlueMgr::m_loginResult;
int32_t CGlueMgr::m_loginState;
int32_t CGlueMgr::m_matrixRemaining;
int32_t CGlueMgr::m_reconnect;
int32_t CGlueMgr::m_reload;
int32_t CGlueMgr::m_scandllOkayToLogIn = 1; // TODO
float CGlueMgr::m_screenHeight;
float CGlueMgr::m_screenWidth;
int32_t CGlueMgr::m_showedDisconnect;
CSimpleTop* CGlueMgr::m_simpleTop;
int32_t CGlueMgr::m_suspended;

struct URLERROR {
    int32_t error;
    char url[32];
};

float CalculateAspectRatio() {
    auto widescreenVar = CVar::Lookup("widescreen");
    auto resolutionVar = CVar::Lookup("gxResolution");

    int32_t width = 4;
    int32_t height = 3;

    if (widescreenVar && widescreenVar->GetInt() && resolutionVar) {
        char separator;
        sscanf(resolutionVar->GetString(), "%d%c%d", &width, &separator, &height);
    }

    return static_cast<float>(width) / static_cast<float>(height);
}

void CGlueMgr::CancelRealmListQuery() {
    if (CGlueMgr::m_idleState != IDLE_REALM_LIST) {
        return;
    }

    ClientServices::Connection()->Cancel(2);

    CGlueMgr::SetIdleState(IDLE_NONE);
}

void CGlueMgr::ChangeRealm(const REALM_INFO* realmInfo) {
    if (!realmInfo) {
        return;
    }

    ClientServices::SelectRealm(realmInfo->name);

    if (ClientServices::IsConnected()) {
        CGlueMgr::m_disconnectPending = 1;
        CGlueMgr::m_reconnect = 1;

        ClientServices::Disconnect();

        return;
    }

    CGlueMgr::SetIdleState(IDLE_ACCOUNT_LOGIN);

    auto text = FrameScript_GetText("GAME_SERVER_LOGIN", -1, GENDER_NOT_APPLICABLE);
    FrameScript_SignalEvent(3, "%s%s", "CANCEL", text);

    ClientServices::Connection()->Connect();
}

void CGlueMgr::EnterWorld() {
    if (!ClientServices::GetSelectedRealm()) {
        return;
    }

    auto character = CCharacterSelection::GetSelectedCharacter();
    CGlueMgr::m_characterInfo = character;

    if (!character) {
        return;
    }

    CCharacterSelection::s_enterWorldIndex = CCharacterSelection::s_selectionIndex;

    if (!ClientServices::IsConnected()) {
        return;
    }

    // Validate character flags

    auto flags = CGlueMgr::m_characterInfo->m_info.flags;

    if (flags & 0x4) {
        auto errorToken = ClientServices::GetErrorToken(84);
        auto errorText = FrameScript_GetText(errorToken, -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(3, "%s%s", "OKAY", errorText);

        return;
    }

    if (flags & 0x1000000) {
        auto errorToken = ClientServices::GetErrorToken(85);
        auto errorText = FrameScript_GetText(errorToken, -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(3, "%s%s", "OKAY", errorText);

        return;
    }

    if (flags & 0x4000) {
        FrameScript_SignalEvent(23, "%s", "CHAR_RENAME_DESCRIPTION");

        return;
    }

    if (!(flags & 0x2000000)) {
        if (NameNeedsDeclension(CURRENT_LANGUAGE, CGlueMgr::m_characterInfo->m_info.name)) {
            FrameScript_SignalEvent(24, nullptr);

            return;
        }
    }

    // Validate expansion

    auto raceRec = g_chrRacesDB.GetRecord(character->m_info.raceID);
    auto classRec = g_chrClassesDB.GetRecord(character->m_info.classID);

    if (
           !raceRec
        || !classRec
        || ClientServices::Connection()->m_accountExpansion < raceRec->m_requiredExpansion
        || ClientServices::Connection()->m_accountExpansion < classRec->m_requiredExpansion
    ) {
        auto errorToken = ClientServices::GetErrorToken(82);
        auto errorText = FrameScript_GetText(errorToken, -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(3, "%s%s", "OKAY", errorText);

        return;
    }

    char indexStr[32];
    SStrPrintf(indexStr, sizeof(indexStr), "%d", CCharacterSelection::s_selectionIndex);
    // TODO g_lastCharacterIndex->Set(indexStr, 1, 0, 0, 1);

    ClientServices::SetAccountName(CGlueMgr::m_accountName);
    ClientServices::SetCharacterInfo(&CGlueMgr::m_characterInfo->m_info);

    // TODO game tip
    // TODO loading screen
    // TODO save all cvars

    if (ClientServices::LoginConnection()->GetLoginServerType() == 0) {
        ClientServices::LoginConnection()->Logoff();
    }

    CGlueMgr::SetIdleState(IDLE_ENTER_WORLD);
}

void CGlueMgr::DisplayLoginStatus() {
    // No change
    if (CGlueMgr::m_lastLoginState == CGlueMgr::m_loginState && CGlueMgr::m_lastLoginResult == CGlueMgr::m_loginResult) {
        return;
    }

    CGlueMgr::m_lastLoginState = CGlueMgr::m_loginState;
    CGlueMgr::m_lastLoginResult = CGlueMgr::m_loginResult;

    // Authentication failure
    if (CGlueMgr::m_loginState == LOGIN_STATE_FAILED) {
        // Unknown result
        if (CGlueMgr::m_loginResult >= LOGIN_RESULT_MAX) {
            return;
        }

        // Parental control
        if (CGlueMgr::m_loginResult == LOGIN_PARENTALCONTROL) {
            auto resultStr = Grunt::g_LoginResultStringNames[LOGIN_PARENTALCONTROL];
            auto resultText = FrameScript_GetText(resultStr, -1, GENDER_NOT_APPLICABLE);
            FrameScript_SignalEvent(3, "%s%s%s", "PARENTAL_CONTROL", resultText, "AUTH_PARENTAL_CONTROL_URL");

            return;
        }

        // Other failures
        auto resultStr = Grunt::g_LoginResultStringNames[CGlueMgr::m_loginResult];
        auto resultText = FrameScript_GetText(resultStr, -1, GENDER_NOT_APPLICABLE);

        const char* buttonText;
        if (false /* TODO SStrStrUTF8I(resultText, "<HTML>") */) {
            buttonText = CGlueMgr::m_loginResult == LOGIN_FAILED
                             ? "CONNECTION_HELP_HTML"
                             : "OKAY_HTML";
        } else {
            buttonText = CGlueMgr::m_loginResult == LOGIN_FAILED
                             ? "CONNECTION_HELP"
                             : "OKAY";
        }

        FrameScript_SignalEvent(3, "%s%s", buttonText, resultText);

        return;
    }

    // Authentication in progress or success
    if (CGlueMgr::m_loginState != LOGIN_STATE_DISCONNECTED && CGlueMgr::m_loginState != LOGIN_STATE_SURVEY) {
        auto v3 = CGlueMgr::m_loginState < LOGIN_STATE_MAX;
        if (CGlueMgr::m_loginState == LOGIN_STATE_MATRIX || CGlueMgr::m_loginState == LOGIN_STATE_MATRIX_WAIT) {
            v3 = false;
        }

        if (CGlueMgr::m_loginState != LOGIN_STATE_TOKEN && CGlueMgr::m_loginState != LOGIN_STATE_TOKEN_WAIT && v3) {
            auto stateStr = Grunt::g_LoginStateStringNames[CGlueMgr::m_loginState];
            auto stateText = FrameScript_GetText(stateStr, -1, GENDER_NOT_APPLICABLE);
            FrameScript_SignalEvent(3, "%s%s", "CANCEL", stateText);
        }
    }
}

void CGlueMgr::GetCharacterList() {
    if (CGlueMgr::m_idleState == IDLE_WORLD_LOGIN) {
        return;
    }

    CGlueMgr::SetIdleState(IDLE_CHARACTER_LIST);

    auto retrieveingText = FrameScript_GetText("CHAR_LIST_RETRIEVING", -1, GENDER_NOT_APPLICABLE);
    FrameScript_SignalEvent(3, "%s%s", "CANCEL", retrieveingText);

    ClientServices::Connection()->GetCharacterList();
}

void CGlueMgr::GetRealmList(int32_t showProgressDialog) {
    CGlueMgr::SetIdleState(IDLE_REALM_LIST);

    if (showProgressDialog) {
        auto inProgressText = FrameScript_GetText("REALM_LIST_IN_PROGRESS", -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(OPEN_STATUS_DIALOG, "%s%s", "CANCEL", inProgressText);
    }

    ClientServices::GetRealmList();
}

int32_t CGlueMgr::HandleBattlenetDisconnect() {
    // TODO

    return 0;
}

int32_t CGlueMgr::HandleDisplaySizeChanged(const CSizeEvent& event) {
    if (
        CGlueMgr::m_screenWidth > 0
        && CGlueMgr::m_screenWidth == event.w
        && CGlueMgr::m_screenHeight > 0
        && CGlueMgr::m_screenHeight == event.h
    ) {
        return 1;
    }

    CGlueMgr::m_screenWidth = event.w;
    CGlueMgr::m_screenHeight = event.h;
    float aspect = CalculateAspectRatio();

    auto glueParent = static_cast<CSimpleFrame*>(CScriptObject::GetScriptObjectByName("GlueParent", CSimpleFrame::GetObjectType()));
    if (glueParent) {
        glueParent->SetFrameScale(1.0f, true);
        CLayoutFrame::ResizePending();
    }

    if (AreEqual(CGlueMgr::m_aspect, aspect, 0.0099999998)) {
        return 1;
    }

    CGlueMgr::m_aspect = aspect;
    CGlueMgr::m_reload = 1;

    return 1;
}

// TODO a1: const EVENT_DATA_IDLE*
int32_t CGlueMgr::Idle(const void* a1, void* a2) {
    // TODO

    if (CGlueMgr::m_idleState == IDLE_NONE) {
        if (CGlueMgr::m_reload) {
            if (!CGlueMgr::m_suspended) {
                CGlueMgr::Suspend();
                CGlueMgr::Resume();
                // TODO Sub4DA360();
                CGlueMgr::SetScreen(CGlueMgr::m_currentScreen);
            }

            CGlueMgr::m_reload = 0;
        }

        // TODO
        // if (CGlueMgr::m_accountMsgAvailable) {
        //     FrameScript_SignalEvent(0x22u, 0);
        //     CGlueMgr::m_accountMsgAvailable = 0;
        // }

        // TODO sub_4D84A0();

        return 1;
    }

    // TODO

    WOWCS_OPS op;
    const char* msg;
    int32_t result;
    int32_t errorCode;
    int32_t complete = ClientServices::Connection()->PollStatus(op, &msg, result, errorCode);

    switch (CGlueMgr::m_idleState) {
    case IDLE_LOGIN_SERVER_LOGIN: {
        CGlueMgr::PollLoginServerLogin();
        break;
    }

    case IDLE_ACCOUNT_LOGIN: {
        CGlueMgr::PollAccountLogin(errorCode, msg, complete, result, op);
        break;
    }

    case IDLE_CHARACTER_LIST: {
        CGlueMgr::PollCharacterList(msg, complete, result, errorCode);
        break;
    }

    case IDLE_REALM_LIST: {
        CGlueMgr::PollRealmList(msg, complete, result, op);
        break;
    }

    case IDLE_ENTER_WORLD: {
        CGlueMgr::PollEnterWorld();
        break;
    }

    // TODO other idle states

    default:
        break;
    }

    return 1;
}

void CGlueMgr::Initialize() {
    CGlueMgr::m_initialized = 1;

    // TODO
    // - cvar stuff (tou, etc)

    CRect windowSize;
    g_theGxDevicePtr->CapsWindowSize(windowSize);

    CSizeEvent sizeEvent;
    sizeEvent.id = -1;
    sizeEvent.w = windowSize.maxX - windowSize.minX;
    sizeEvent.h = windowSize.maxY - windowSize.minY;

    CGlueMgr::HandleDisplaySizeChanged(sizeEvent);

    FrameXML_RegisterFactory("ModelFFX", &CSimpleModelFFX::Create, 0);

    CGlueMgr::Resume();

    CRealmList::Initialize();

    EventRegisterEx(EVENT_ID_IDLE, &CGlueMgr::Idle, 0, 0.0);

    // TODO
    // CGLCD::Initialize();
    // sub_552380();

    // TODO
    // - options changed warning stuff

    // TODO
    // AccountDataInitializeBasicSystem();
}

void CGlueMgr::LoginServerLogin(const char* accountName, const char* password) {
    if (!CGlueMgr::m_scandllOkayToLogIn || !CGlueMgr::m_acceptedTOS || !CGlueMgr::m_acceptedEULA || CGlueMgr::m_idleState != IDLE_NONE) {
        return;
    }

    if (!accountName || !*accountName) {
        auto prompt = FrameScript_GetText("LOGIN_ENTER_NAME", -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(3, "%s%s", "OKAY", prompt);

        return;
    }

    if (!password || !*password) {
        auto prompt = FrameScript_GetText("LOGIN_ENTER_PASSWORD", -1, GENDER_NOT_APPLICABLE);
        FrameScript_SignalEvent(3, "%s%s", "OKAY", prompt);

        return;
    }

    CGlueMgr::m_loginResult = -1;
    CGlueMgr::m_loginState = -1;
    CGlueMgr::m_lastLoginResult = -1;
    CGlueMgr::m_lastLoginState = -1;
    CGlueMgr::m_authenticated = false;
    CGlueMgr::m_matrixRemaining = 0;

    CGlueMgr::SetIdleState(IDLE_LOGIN_SERVER_LOGIN);

    char* dest = CGlueMgr::m_accountName;
    for (const char* src = accountName; *src && dest < CGlueMgr::m_accountName + sizeof(CGlueMgr::m_accountName); src++, dest++) {
        if (src[0] == '|' && src[1] == '|') {
            src++;
        }

        *dest = *src;
    }
    *dest = '\0';

    SStrUpper(CGlueMgr::m_accountName);

    // Show prompt
    FrameScript_SignalEvent(3, "%s", "CANCEL");

    ClientServices::Logon(CGlueMgr::m_accountName, password);

    // Zero out password
    memset(const_cast<char*>(password), 0, SStrLen(password));
}

int32_t CGlueMgr::NetDisconnectHandler(const void* a1, void* a2) {
    auto notAccountLogin = CGlueMgr::m_idleState != IDLE_ACCOUNT_LOGIN;

    CGlueMgr::SetIdleState(IDLE_NONE);

    if (CGlueMgr::m_disconnectPending) {
        // TODO ConsolePrintf("CGlueMgr::NetDisconnectHandler: Disconnect pending");

        CGlueMgr::m_disconnectPending = 0;

        if (CGlueMgr::m_reconnect) {
            CGlueMgr::m_reconnect = 0;

            CGlueMgr::SetIdleState(IDLE_ACCOUNT_LOGIN);

            auto loginText = FrameScript_GetText("GAME_SERVER_LOGIN", -1, GENDER_NOT_APPLICABLE);
            FrameScript_SignalEvent(OPEN_STATUS_DIALOG, "%s%s", "CANCEL", loginText);

            ClientServices::Connection()->Connect();

            return 1;
        }

        return 1;
    }

    if (!ClientServices::ValidDisconnect(a1)) {
        // TODO ConsolePrintf("CGlueMgr::NetDisconnectHandler: Invalid disconnect");

        return 1;
    }

    // TODO ClientDestroyGame(0, 1, 0);
    // TODO EventSetMouseMode(0, 0);

    if (notAccountLogin) {
        // TODO ConsolePrintf("CGlueMgr::NetDisconnectHandler: Displaying script");

        FrameScript_SignalEvent(DISCONNECTED_FROM_SERVER, "%d", CGlueMgr::m_clientKickReason);

        ClientServices::LoginConnection()->Logoff();

        return 1;
    }

    // TODO ConsolePrintf("CGlueMgr::NetDisconnectHandler: NOT displaying script");

    WOWCS_OPS op;
    const char* msg;
    int32_t result;
    int32_t errorCode;
    int32_t complete = ClientServices::Connection()->PollStatus(op, &msg, result, errorCode);

    if (complete && result == 0) {
        FrameScript_SignalEvent(OPEN_STATUS_DIALOG, "%s%s", "OKAY", msg);
    } else {
        ClientServices::SelectRealm("");

        // TODO FrameScript_SignalEvent(DISCONNECTED_FROM_SERVER, "%d", CGlueMgr::m_clientKickReason);
    }

    ClientServices::LoginConnection()->Logoff();

    return 1;
}

int32_t CGlueMgr::OnKickReasonMsg(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // Nothing to read
    if (msg->IsRead()) {
        CGlueMgr::m_clientKickReason = 0;
        return 1;
    }

    uint8_t reason = 0;
    msg->Get(reason);

    // Message only contained reason code
    if (msg->IsRead()) {
        CGlueMgr::m_clientKickReason = reason;
        return 1;
    }

    // UI isn't currently running
    if (!CGlueMgr::m_initialized || CGlueMgr::m_suspended) {
        CGlueMgr::m_clientKickReason = reason;
        return 1;
    }

    // TODO
    //
    // char v6[16];
    // msg->GetArray(v6, sizeof(v6));
    //
    // if (!msg->IsValid()) {
    //    CGlueMgr::m_clientKickReason = reason;
    //    return 1;
    // }
    //
    // Sub4042C0(v6);

    return 1;
}

void CGlueMgr::PollAccountLogin(int32_t errorCode, const char* msg, int32_t complete, int32_t result, WOWCS_OPS op) {
    static URLERROR s_urlErrors[] = {
        { 28, "AUTH_BANNED_URL"             },
        { 31, "AUTH_DB_BUSY_URL"            },
        { 30, "AUTH_NO_TIME_URL"            },
        { 32, "AUTH_SUSPENDED_URL"          },
        { 33, "AUTH_PARENTAL_CONTROL_URL"   }
    };

    auto login = ClientServices::LoginConnection();

    if (login->GetLoginServerType() == 1 && CGlueMgr::m_loginState == LOGIN_STATE_FAILED) {
        CGlueMgr::DisplayLoginStatus();

        ClientServices::Connection()->Cancel(2);
        login->Logoff();

        CGlueMgr::SetIdleState(IDLE_NONE);

        return;
    }

    if (errorCode == 27) {
        // TODO AUTH_WAIT_QUEUE
    } else {
        if (CGlueMgr::m_displayingQueueDialog) {
            FrameScript_SignalEvent(3, "%s", "CANCEL");
            CGlueMgr::m_displayingQueueDialog = 0;
        }

        FrameScript_SignalEvent(4, "%s", msg);
    }

    if (!complete) {
        return;
    }

    // Error

    if (result == 0) {
        if (errorCode != 2) {
            URLERROR* urlError = nullptr;

            for (auto& ue : s_urlErrors) {
                if (errorCode == ue.error) {
                    urlError = &ue;
                    break;
                }
            }

            if (urlError) {
                FrameScript_SignalEvent(OPEN_STATUS_DIALOG,"%s%s%s", "OKAY_WITH_URL", urlError->url);
            } else {
                FrameScript_SignalEvent(OPEN_STATUS_DIALOG, "%s%s", "OKAY", msg);
            }
        }

        CGlueMgr::SetIdleState(IDLE_NONE);

        if (errorCode == 2) {
            CGlueMgr::m_disconnectPending = 1;
            ClientServices::Disconnect();
        }

        if (errorCode != 13) {
            CCharacterSelection::ClearCharacterList();

            if (ClientServices::GetInstance()->m_realmList.Count()) {
                FrameScript_SignalEvent(5, nullptr);
                CRealmList::UpdateList();
            } else {
                CGlueMgr::GetRealmList(1);
            }

            return;
        }

        if (!SStrCmpI(CGlueMgr::m_currentScreen, "charselect", STORM_MAX_STR)) {
            CGlueMgr::SetScreen("login");
            return;
        }

        return;
    }

    // Success

    if (op == COP_CONNECT) {
        ClientServices::Connection()->AccountLogin(CGlueMgr::m_accountName, "", CURRENT_REGION, CURRENT_LANGUAGE);
        return;
    }

    CGlueMgr::SetIdleState(IDLE_NONE);
    CCharacterSelection::ClearCharacterList();

    // TODO streaming, trial, and expansion checks

    auto realm = ClientServices::GetSelectedRealm();
    auto categoryRec = g_cfg_CategoriesDB.GetRecord(realm->category);

    if (categoryRec) {
        ValidateNameInitialize(categoryRec->m_localeMask, categoryRec->m_createCharsetMask);
    } else {
        ValidateNameInitialize(1 << CURRENT_LANGUAGE, 0);
    }

    CGlueMgr::SetScreen("charselect");
}

void CGlueMgr::PollCharacterList(const char* msg, int32_t complete, int32_t result, int32_t errorCode) {
    FrameScript_SignalEvent(4, "%s", msg);

    if (CGlueMgr::HandleBattlenetDisconnect()) {
        CGlueMgr::SetIdleState(IDLE_NONE);
    }

    if (!complete) {
        return;
    }

    // Error

    if (result == 0) {
        if (errorCode == 2) {
            CCharacterSelection::ClearCharacterList();
            CGlueMgr::GetRealmList(1);

            return;
        }

        FrameScript_SignalEvent(3, "%s%s", "OKAY", msg);

        CGlueMgr::SetIdleState(IDLE_NONE);

        return;
    }

    // Success

    CGlueMgr::SetIdleState(IDLE_NONE);

    FrameScript_SignalEvent(5, nullptr);

    CCharacterSelection::UpdateCharacterList();

    if (CGlueMgr::m_accountMsgAvailable) {
        FrameScript_SignalEvent(34, nullptr);
        CGlueMgr::m_accountMsgAvailable = 0;
    }
}

void CGlueMgr::PollEnterWorld() {
    if (!LoadingScreenDrawing()) {
        return;
    }

    if (CGlueMgr::m_suspended) {
        CGlueMgr::SetIdleState(IDLE_NONE);

        // TODO SI Logic

        // TODO TLS shenanigans with guid
        ClientServices::Connection()->CharacterLogin(CGlueMgr::m_characterInfo->m_info.guid, 0);

        return;
    }

    uint32_t mapId = CGlueMgr::m_characterInfo->m_info.mapID;
    C3Vector position = CGlueMgr::m_characterInfo->m_info.position;

    // TODO TLS shenanigans with guid
    // TODO first login logic (play intro M2?)

    CGlueMgr::Suspend();

    ClientInitializeGame(mapId, position);
}

void CGlueMgr::PollLoginServerLogin() {
    if (CGlueMgr::m_loginState != LOGIN_STATE_PIN_WAIT) {
        CGlueMgr::DisplayLoginStatus();
    }

    // Open new client connection after successful authentication
    if (CGlueMgr::m_authenticated) {
        CGlueMgr::SetIdleState(IDLE_NONE);
        CGlueMgr::Sub4D8BA0();
        CGlueMgr::m_authenticated = false;

        return;
    }

    switch (CGlueMgr::m_loginState) {
    case LOGIN_STATE_FAILED: {
        ClientServices::LoginConnection()->Logoff();

        CGlueMgr::SetIdleState(IDLE_NONE);

        break;
    }

    case LOGIN_STATE_DOWNLOADFILE: {
        // TODO

        break;
    }

    case LOGIN_STATE_PIN: {
        // TODO

        break;
    }

    case LOGIN_STATE_MATRIX: {
        // TODO

        break;
    }

    case LOGIN_STATE_TOKEN: {
        // TODO

        break;
    }

    case LOGIN_STATE_CHECKINGVERSIONS: {
        uint8_t versionChecksum[LOGIN_VERSION_CHECKSUM_LEN];
        auto versionChallenge = ClientServices::LoginConnection()->GetVersionChallenge();
        ChecksumExecutables(versionChallenge, LOGIN_VERSION_CHALLENGE_LEN, versionChecksum);

        ClientServices::LoginConnection()->ProveVersion(versionChecksum);

        break;
    }

    default:
        break;
    }
}

void CGlueMgr::PollRealmList(const char* msg, int32_t complete, int32_t result, WOWCS_OPS op) {
    FrameScript_SignalEvent(UPDATE_STATUS_DIALOG, "%s", msg);

    if (CGlueMgr::HandleBattlenetDisconnect()) {
        CGlueMgr::SetIdleState(IDLE_NONE);
    }

    if (!complete) {
        return;
    }

    // Error

    if (result == 0) {
        FrameScript_SignalEvent(OPEN_STATUS_DIALOG, "%s%s", "OKAY", msg);

        CGlueMgr::SetIdleState(IDLE_NONE);
    }

    // Success

    CGlueMgr::SetIdleState(IDLE_NONE);

    FrameScript_SignalEvent(CLOSE_STATUS_DIALOG, nullptr);

    CRealmList::UpdateList();

    if (CGlueMgr::m_accountMsgAvailable) {
        FrameScript_SignalEvent(ACCOUNT_MESSAGES_AVAILABLE, nullptr);
        CGlueMgr::m_accountMsgAvailable = 0;
    }
}

void CGlueMgr::QuitGame() {
    ClientPostClose(0);
}

void CGlueMgr::RealmListDialogCancelled() {
    if (!SStrCmpI(CGlueMgr::m_currentScreen, "login")) {
        ClientServices::LoginConnection()->Logoff();
    }
}

void CGlueMgr::Resume() {
    // TODO
    // CGlueMgr::m_disconnectPending = 0;
    // CGlueMgr::m_reconnect = 0;

    CGlueMgr::SetIdleState(IDLE_NONE);

    // TODO
    // CGlueMgr::m_characterInfo = 0;

    CGlueMgr::m_suspended = 0;
    CGlueMgr::m_reload = 0;

    CRect screenRect;
    g_theGxDevicePtr->CapsWindowSizeInScreenCoords(screenRect);
    CGlueMgr::m_screenWidth = screenRect.maxX - screenRect.minX;
    CGlueMgr::m_screenHeight = screenRect.maxY - screenRect.minY;
    CGlueMgr::m_aspect = CalculateAspectRatio();
    CoordinateSetAspectRatio(CGlueMgr::m_aspect);

    // Create CSimpleTop
    auto m = SMemAlloc(sizeof(CSimpleTop), __FILE__, __LINE__, 0x0);
    auto top = new (m) CSimpleTop();

    CGlueMgr::m_simpleTop = top;
    CGlueMgr::m_simpleTop->m_displaySizeCallback = &CGlueMgr::HandleDisplaySizeChanged;

    // TODO
    // - setting cursor texture
    // - setting mouse mode

    FrameScript_Flush();

    SystemRegisterFunctions();
    RegisterSimpleFrameScriptMethods();
    GlueScriptEventsRegisterFunctions();
    CharSelectRegisterScriptFunctions();
    CharacterCreateRegisterScriptFunctions();
    RealmListRegisterScriptFunctions();
    SI2::RegisterScriptFunctions();

    // TODO
    // AccountMsg_RegisterScriptFunctions();
    // CGVideoOptions::RegisterScriptFunctions();

    // TODO
    // FrameScript::s_scriptFunctionsLoaded = 1;

    FrameScript_CreateEvents(g_glueScriptEvents, NUM_GLUESCRIPTEVENTS);

    OsCreateDirectory("Logs", 0);

    CWOWClientStatus status;

    if (!SLogCreate("Logs\\GlueXML.log", 0, status.m_logFile)) {
        SysMsgPrintf(SYSMSG_WARNING, "Cannot create WOWClient log file \"%s\"!", "Logs\\GlueXML.log");
    }

    DeleteInterfaceFiles();

    MD5_CTX md5;
    unsigned char digest1[16];
    unsigned char digest2[16];

    int32_t v8;
    unsigned char* v9;
    unsigned char* v10;

    MD5Init(&md5);

    switch (FrameXML_CheckSignature("Interface\\GlueXML\\GlueXML.toc", 0, InterfaceKey, digest1)) {
        case 0:
            status.Add(STATUS_WARNING, "GlueXML missing signature");
            ClientPostClose(9);
            return;

        case 1:
            status.Add(STATUS_WARNING, "GlueXML has corrupt signature");
            ClientPostClose(9);
            return;

        case 2:
            status.Add(STATUS_WARNING, "GlueXML is modified or corrupt");
            ClientPostClose(9);
            return;

        case 3:
            FrameXML_FreeHashNodes();
            FrameXML_CreateFrames("Interface\\GlueXML\\GlueXML.toc", 0, &md5, &status);

            MD5Final(digest2, &md5);

            v8 = 16;
            v9 = digest2;
            v10 = digest1;

            break;

        default:
            ClientPostClose(9);
            return;
    }

    // TODO
    // - some kind of digest validation?

    FrameScript_SignalEvent(22, nullptr);

    // TODO
    // CGlueMgr::InitializeFFX();

    // TODO
    // ClientServices::SetMessageHandler(SMSG_CHARACTER_RENAME_RESULT, CGlueMgr::OnCharRenameResult, 0);
    // ClientServices::SetMessageHandler(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, CGlueMgr::OnCharDeclineResult, 0);
    // ClientServices::SetMessageHandler(SMSG_CHAR_CUSTOMIZE, CGlueMgr::OnCharCustomizeResult, 0);
    // ClientServices::SetMessageHandler(SMSG_REALM_SPLIT, CGlueMgr::OnRealmSplitMsg, 0);
    // ClientServices::SetMessageHandler(SMSG_KICK_REASON, CGlueMgr::OnKickReasonMsg, 0);
    // ClientServices::SetMessageHandler(SMSG_CHAR_FACTION_CHANGE, CGlueMgr::OnCharFactionChangeResult, 0);

    // TODO
    // CGlueMgr::m_pendingServerAlert = 1;
    // CGlueMgr::m_processServerAlert = 0;
    // CGlueMgr::m_serverAlert[0] = 0;
    // v21 = (const char *)ClientServices::GetServerAlertURL();
    // v22 = (const CHAR *)FrameScript_GetText(v21, -1, 0);
    // if (!OsURLDownload(v22, CGlueMgr::ServerAlertURLCallback, 0)) {
    //     CGlueMgr::m_pendingServerAlert = 0;
    // }
}

void CGlueMgr::SetCurrentAccount(const char* accountName) {
    SStrCopy(CGlueMgr::m_accountName, accountName, sizeof(CGlueMgr::m_accountName));
    SStrUpper(CGlueMgr::m_accountName);
}

void CGlueMgr::SetIdleState(GLUE_IDLE_STATE state) {
    CGlueMgr::m_idleState = state;
    CGlueMgr::m_showedDisconnect = 0;
}

void CGlueMgr::SetLoginStateAndResult(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, const char* stateStr, const char* resultStr, uint8_t flags) {
    // TODO
    // CGlueMgr::LogConnectionStatus("GRUNT: state: %s result: %s %s", a4);

    if (flags & 0x1) {
        CGlueMgr::m_accountMsgAvailable = 1;
    }

    if (state == LOGIN_STATE_AUTHENTICATED) {
        CGlueMgr::m_authenticated = true;
    }

    CGlueMgr::m_loginState = state;
    CGlueMgr::m_loginResult = result;
}

void CGlueMgr::SetScreen(const char* screen) {
    FrameScript_SignalEvent(0, "%s", screen);
}

void CGlueMgr::StatusDialogClick() {
    if (!SStrCmpI(CGlueMgr::m_currentScreen, "patchdownload", STORM_MAX_STR)) {
        CGlueMgr::SetScreen("login");
    }

    switch (CGlueMgr::m_idleState) {
        case IDLE_NONE: {
            ClientServices::Connection()->Cleanup();

            break;
        }

        case IDLE_LOGIN_SERVER_LOGIN: {
            ClientServices::LoginConnection()->Logoff();
            CGlueMgr::SetIdleState(IDLE_NONE);

            break;
        }

        case IDLE_ACCOUNT_LOGIN:
        case IDLE_CHARACTER_LIST: {
            ClientServices::Connection()->Cancel(2);
            CCharacterSelection::ClearCharacterList();

            break;
        }

        case IDLE_REALM_LIST:
        case IDLE_5:
        case IDLE_6:
        case IDLE_ENTER_WORLD: {
            ClientServices::Connection()->Cancel(2);
            CGlueMgr::SetIdleState(IDLE_NONE);

            break;
        }

        case IDLE_7:
        case IDLE_8:
        case IDLE_9: {
            CGlueMgr::SetIdleState(IDLE_NONE);

            break;
        }

        case IDLE_WORLD_LOGIN: {
            CGlueMgr::SetIdleState(IDLE_NONE);
            CGlueMgr::GetCharacterList();

            break;
        }

        case IDLE_12:
        case IDLE_13: {
            // TODO

            break;
        }
    }
}

void CGlueMgr::Sub4D8BA0() {
    if (CGlueMgr::m_idleState != IDLE_NONE) {
        return;
    }

    CGlueMgr::SetIdleState(IDLE_ACCOUNT_LOGIN);
    ClientServices::Connection()->Connect();
}

void CGlueMgr::Suspend() {
    CGlueMgr::m_suspended = 1;

    // TODO

    if (CGlueMgr::m_simpleTop) {
        delete CGlueMgr::m_simpleTop;
        CGlueMgr::m_simpleTop = nullptr;
    }

    // TODO

    FrameXML_FreeHashNodes();

    // TODO
}

void CGlueMgr::UpdateCurrentScreen(const char* screen) {
    // TODO

    SStrCopy(CGlueMgr::m_currentScreen, screen, sizeof(CGlueMgr::m_currentScreen));

    // TODO
}
