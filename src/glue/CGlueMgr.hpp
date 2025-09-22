#ifndef GLUE_C_GLUE_MGR_HPP
#define GLUE_C_GLUE_MGR_HPP

#include "event/Event.hpp"
#include "net/Types.hpp"
#include <cstdint>

class CSimpleTop;

class CGlueMgr {
    public:
        // Types
        enum GLUE_IDLE_STATE {
            IDLE_NONE = 0,
            IDLE_LOGIN_SERVER_LOGIN = 1,
            IDLE_ACCOUNT_LOGIN = 2,
            IDLE_3 = 3,
            IDLE_4 = 4,
            IDLE_5 = 5,
            IDLE_6 = 6,
            IDLE_7 = 7,
            IDLE_8 = 8,
            IDLE_9 = 9,
            IDLE_ENTER_WORLD = 10,
            IDLE_11 = 11,
            IDLE_12 = 12,
            IDLE_13 = 13
        };

        // Static variables
        static int32_t m_acceptedEULA;
        static int32_t m_acceptedTerminationWithoutNotice;
        static int32_t m_acceptedTOS;
        static int32_t m_accountMsgAvailable;
        static char m_accountName[];
        static float m_aspect;
        static bool m_authenticated;
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
        // TODO a1: const EVENT_DATA_IDLE*
        static int32_t HandleDisplaySizeChanged(const CSizeEvent& event);
        static int32_t Idle(const void* a1, void* a2);
        static void Initialize();
        static void LoginServerLogin(const char* accountName, const char* password);
        static void QuitGame();
        static void PollAccountLogin(int32_t errorCode, const char* msg, int32_t complete, int32_t result, WOWCS_OPS op);
        static void PollEnterWorld();
        static void PollLoginServerLogin();
        static void Resume();
        static void SetCurrentAccount(const char* accountName);
        static void SetLoginStateAndResult(LOGIN_STATE state, LOGIN_RESULT result, char const* addrStr, char const* stateStr, char const* resultStr, uint8_t flags);
        static void SetScreen(const char* screen);
        static void StatusDialogClick();
        static void Sub4D8BA0();
        static void Suspend();
        static void UpdateCurrentScreen(const char* screen);
};

#endif
