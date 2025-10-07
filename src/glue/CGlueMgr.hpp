#ifndef GLUE_C_GLUE_MGR_HPP
#define GLUE_C_GLUE_MGR_HPP

#include "event/Event.hpp"
#include "glue/Types.hpp"
#include "net/Login.hpp"
#include "net/Types.hpp"

class CDataStore;
class CSimpleTop;
struct CharacterSelectionDisplay;

class CGlueMgr {
    public:
        // Types
        enum GLUE_IDLE_STATE {
            IDLE_NONE               = 0,
            IDLE_LOGIN_SERVER_LOGIN = 1,
            IDLE_ACCOUNT_LOGIN      = 2,
            IDLE_CHARACTER_LIST     = 3,
            IDLE_REALM_LIST         = 4,
            IDLE_5                  = 5,
            IDLE_6                  = 6,
            IDLE_7                  = 7,
            IDLE_8                  = 8,
            IDLE_9                  = 9,
            IDLE_ENTER_WORLD        = 10,
            IDLE_WORLD_LOGIN        = 11,
            IDLE_12                 = 12,
            IDLE_13                 = 13
        };

        // Static variables
        static int32_t m_acceptedEULA;
        static int32_t m_acceptedTerminationWithoutNotice;
        static int32_t m_acceptedTOS;
        static int32_t m_accountMsgAvailable;
        static char m_accountName[];
        static float m_aspect;
        static bool m_authenticated;
        static const CharacterSelectionDisplay* m_characterInfo;
        static char m_currentScreen[];
        static int32_t m_disconnectPending;
        static int32_t m_displayingQueueDialog;
        static GLUE_IDLE_STATE m_idleState;
        static int32_t m_initialized;
        static int32_t m_lastLoginResult;
        static int32_t m_lastLoginState;
        static int32_t m_loginResult;
        static int32_t m_loginState;
        static int32_t m_matrixRemaining;
        static int32_t m_reconnect;
        static int32_t m_reload;
        static int32_t m_scandllOkayToLogIn;
        static float m_screenHeight;
        static float m_screenWidth;
        static int32_t m_showedDisconnect;
        static CSimpleTop* m_simpleTop;
        static int32_t m_suspended;

        // Static functions
        static void ChangeRealm(const REALM_INFO* realmInfo);
        static void DisplayLoginStatus();
        static void EnterWorld();
        static void GetCharacterList();
        static void GetRealmList(int32_t a1);
        static int32_t HandleBattlenetDisconnect();
        static int32_t HandleDisplaySizeChanged(const CSizeEvent& event);
        static int32_t Idle(const void* a1, void* a2); // TODO a1: const EVENT_DATA_IDLE*
        static void Initialize();
        static void LoginServerLogin(const char* accountName, const char* password);
        static int32_t OnKickReasonMsg(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);
        static void PollAccountLogin(int32_t errorCode, const char* msg, int32_t complete, int32_t result, WOWCS_OPS op);
        static void PollCharacterList(const char* msg, int32_t complete, int32_t result, int32_t errorCode);
        static void PollEnterWorld();
        static void PollLoginServerLogin();
        static void QuitGame();
        static void Resume();
        static void SetCurrentAccount(const char* accountName);
        static void SetIdleState(GLUE_IDLE_STATE state);
        static void SetLoginStateAndResult(LOGIN_STATE state, LOGIN_RESULT result, char const* addrStr, char const* stateStr, char const* resultStr, uint8_t flags);
        static void SetScreen(const char* screen);
        static void StatusDialogClick();
        static void Sub4D8BA0();
        static void Suspend();
        static void UpdateCurrentScreen(const char* screen);
};

#endif
