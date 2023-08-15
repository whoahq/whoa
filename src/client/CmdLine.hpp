#ifndef CLIENT_CMD_LINE_HPP
#define CLIENT_CMD_LINE_HPP

#include <cstdint>

enum CMDOPT {
    CMD_D3D = 0,
    CMD_D3D9EX = 1,
    CMD_DATA_DIR = 2,
    CMD_NO_LAG_FIX = 3,
    CMD_LOADFILE = 4,
    CMD_GAMETYPE = 5,
    CMD_OPENGL = 6,
    CMD_SW_TNL = 7,
    CMD_TIMEDEMO = 8,
    CMD_DEMOREZ = 9,
    CMD_DEMODEPTH = 10,
    CMD_DEMODETAIL = 11,
    CMD_DEMOSOUND = 12,
    CMD_FULL_SCREEN = 13,
    CMD_22050HZ = 14,
    CMD_NO_WARNINGS = 15,
    CMDOPTS
};

enum WOWCMDOPT {
    WOWCMD_RES_800x600 = 16,
    WOWCMD_RES_1024x768 = 17,
    WOWCMD_RES_1280x960 = 18,
    WOWCMD_RES_1280x1024 = 19,
    WOWCMD_RES_1600x1200 = 20,
    WOWCMD_UP_TO_DATE = 21,
    WOWCMD_16_BIT = 22,
    WOWCMD_NO_FIX_LAG = 24,
    WOWCMD_NO_SOUND = 26,
    WOWCMD_SOUND_CHAOS = 27,
    WOWCMD_DEPTH_16 = 29,
    WOWCMD_DEPTH_24 = 30,
    WOWCMD_DEPTH_32 = 31,
    WOWCMD_WINDOWED = 32,
    WOWCMD_CONSOLE = 35,
    WOWCMD_HW_DETECT = 36,
    WOWCMD_GX_OVERRIDE = 39,
    WOWCMD_OPTS
};

static int32_t CmdLineProcess();

static void ProcessCommandLine();

const char* CmdLineGetString(uint32_t opt);

uint32_t    CmdLineGetUint(uint32_t opt);

int32_t     CmdLineGetBool(uint32_t opt);

#endif
