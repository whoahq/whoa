#ifndef UI_GAME_C_G_GAME_UI_HPP
#define UI_GAME_C_G_GAME_UI_HPP

#include "util/guid/Types.hpp"

class CScriptObject;
class CSimpleTop;

class CGGameUI {
    public:
        // Static variables
        static CScriptObject* s_gameTooltip;
        static CSimpleTop* s_simpleTop;

        // Static functions
        static void EnterWorld();
        static WOWGUID& GetCurrentObjectTrack();
        static uint32_t GetCursorMoney();
        static WOWGUID& GetLockedTarget();
        static void Initialize();
        static void InitializeGame();
        static bool IsLoggingIn();
        static int32_t IsRaidMember(const WOWGUID& guid);
        static int32_t IsRaidMemberOrPet(const WOWGUID& guid);
        static void RegisterFrameFactories();

    private:
        static WOWGUID s_currentObjectTrack;
        static uint32_t s_cursorMoney;
        static bool s_inWorld;
        static WOWGUID s_lockedTarget;
        static bool s_loggingIn;
};

#endif
