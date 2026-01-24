#ifndef UI_GAME_C_G_GAME_UI_HPP
#define UI_GAME_C_G_GAME_UI_HPP

class CSimpleTop;

class CGGameUI {
    public:
        // Static variables
        static CSimpleTop* s_simpleTop;

        // Static functions
        static void Initialize();
        static void InitializeGame();
        static void RegisterFrameFactories();
};

#endif
