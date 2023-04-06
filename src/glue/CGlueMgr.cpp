#include "glue/CGlueMgr.hpp"
#include "glue/CRealmList.hpp"
#include "client/Client.hpp"
#include "client/ClientServices.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
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
#include "console/CVar.hpp"
#include "util/Filesystem.hpp"
#include "util/Log.hpp"
#include <cstdio>
#include <common/MD5.hpp>

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

void CGlueMgr::ChangeRealm(const REALM_INFO* realmInfo) {
    if (!realmInfo) {
        return;
    }

    ClientServices::SelectRealm(realmInfo->name);

    if (ClientServices::Connection()->IsConnected()) {
        CGlueMgr::m_disconnectPending = 1;
        CGlueMgr::m_reconnect = 1;

        ClientServices::Connection()->Disconnect();

        return;
    }

    CGlueMgr::m_idleState = IDLE_ACCOUNT_LOGIN;
    CGlueMgr::m_showedDisconnect = 0;

    auto text = FrameScript_GetText("GAME_SERVER_LOGIN", -1, GENDER_NOT_APPLICABLE);
    FrameScript_SignalEvent(3, "%s%s", "CANCEL", text);

    ClientServices::Connection()->Connect();
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
    CGlueMgr::m_idleState = IDLE_LOGIN_SERVER_LOGIN;
    CGlueMgr::m_showedDisconnect = 0;

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

void CGlueMgr::QuitGame() {
    ClientPostClose(0);
}

void CGlueMgr::PollAccountLogin(int32_t errorCode, const char* msg, int32_t complete, int32_t result, WOWCS_OPS op) {
    auto login = ClientServices::LoginConnection();

    if (login->GetLoginServerType() == 1 && CGlueMgr::m_loginState == LOGIN_STATE_FAILED) {
        CGlueMgr::DisplayLoginStatus();

        ClientServices::Connection()->Cancel(2);
        login->Logoff();

        CGlueMgr::m_idleState = IDLE_NONE;
        CGlueMgr::m_showedDisconnect = 0;

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

    if (complete) {
        if (result == 0) {
            if (errorCode != 2) {
                // TODO
            }

            CGlueMgr::m_idleState = IDLE_NONE;
            CGlueMgr::m_showedDisconnect = 0;

            if (errorCode == 2) {
                // TODO CGlueMgr::m_disconnectPending = 1;
                // TODO ClientServices::Connection()->Disconnect();
            }

            if (errorCode != 13) {
                // TODO CCharacterSelection::ClearCharacterList();

                if (ClientServices::GetInstance()->m_realmList.Count()) {
                    FrameScript_SignalEvent(5, nullptr);
                    CRealmList::UpdateList();
                } else {
                    // TODO
                }

                return;
            }

            if (!SStrCmpI(CGlueMgr::m_currentScreen, "charselect", STORM_MAX_STR)) {
                CGlueMgr::SetScreen("login");
                return;
            }

            return;
        }

        if (op == COP_CONNECT) {
            // TODO

            return;
        }
    }
}

void CGlueMgr::PollLoginServerLogin() {
    if (CGlueMgr::m_loginState != LOGIN_STATE_PIN_WAIT) {
        CGlueMgr::DisplayLoginStatus();
    }

    // Open new client connection after successful authentication
    if (CGlueMgr::m_authenticated) {
        CGlueMgr::m_idleState = IDLE_NONE;
        CGlueMgr::m_showedDisconnect = 0;
        CGlueMgr::Sub4D8BA0();
        CGlueMgr::m_authenticated = false;

        return;
    }

    switch (CGlueMgr::m_loginState) {
    case LOGIN_STATE_FAILED: {
        // TODO

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
        uint8_t versionChecksum[20];
        // TODO
        // uint8_t* versionChallenge = ClientServices::LoginConnection()->GetVersionChallenge();
        // ChecksumExecutables(versionChallenge, 16, versionChecksum);
        ClientServices::LoginConnection()->ProveVersion(versionChecksum);

        break;
    }

    default:
        break;
    }
}

void CGlueMgr::Resume() {
    // TODO
    // CGlueMgr::m_disconnectPending = 0;
    // CGlueMgr::m_reconnect = 0;

    CGlueMgr::m_idleState = IDLE_NONE;

    // TODO
    // CGlueMgr::m_showedDisconnect = 0;
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

    FrameScript_CreateEvents(g_glueScriptEvents, NUM_GLUE_SCRIPT_EVENTS);

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

            CGlueMgr::m_showedDisconnect = 0;
            CGlueMgr::m_idleState = IDLE_NONE;

            break;
        }

        case IDLE_ACCOUNT_LOGIN:
        case IDLE_3: {
            ClientServices::Connection()->Cancel(2);

            break;
        }

        case IDLE_4:
        case IDLE_5:
        case IDLE_6:
        case IDLE_10: {
            ClientServices::Connection()->Cancel(2);

            CGlueMgr::m_showedDisconnect = 0;
            CGlueMgr::m_idleState = IDLE_NONE;

            break;
        }

        case IDLE_7:
        case IDLE_8:
        case IDLE_9: {
            CGlueMgr::m_showedDisconnect = 0;
            CGlueMgr::m_idleState = IDLE_NONE;

            break;
        }

        case IDLE_11: {
            CGlueMgr::m_showedDisconnect = 0;
            CGlueMgr::m_idleState = IDLE_NONE;

            // TODO
            // CGlueMgr::GetCharacterList();

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

    CGlueMgr::m_idleState = IDLE_ACCOUNT_LOGIN;
    CGlueMgr::m_showedDisconnect = 0;
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
