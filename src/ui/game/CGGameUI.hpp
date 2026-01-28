#ifndef UI_GAME_C_G_GAME_UI_HPP
#define UI_GAME_C_G_GAME_UI_HPP

class CScriptObject;
class CSimpleTop;

class CGGameUI {
    public:
        // Static variables
        static CScriptObject* s_gameTooltip;
        static CSimpleTop* s_simpleTop;

        // Static functions
        static void Initialize();
        static void InitializeGame();
        static void RegisterFrameFactories();
};

#endif
