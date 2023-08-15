#include "client/CmdLine.hpp"

#include <storm/Command.hpp>

int32_t CmdLineProcess() {
    // engine-specific flags.
    static ARGLIST s_argList[] = {
        {0x0,                       CMD_D3D,         "d3d",        nullptr},
        {0x0,                       CMD_D3D9EX,      "d3d9ex",     nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_DATADIR,     "datadir",    nullptr},
        {0x0,                       CMD_NO_LAG_FIX,  "nolagfix",   nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_LOADFILE,    "loadfile",   nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_GAMETYPE,    "gametype",   nullptr},
        {0x0,                       CMD_OPENGL,      "opengl",     nullptr},
        {0x0,                       CMD_SWTNL,       "swtnl",      nullptr},
        {0x0,                       CMD_TIMEDEMO,    "timedemo",   nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_DEMOREZ,     "rez",        nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_DEMODEPTH,   "depth",      nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_DEMODETAIL,  "detail",     nullptr},
        {STORM_COMMAND_TYPE_STRING, CMD_DEMOSOUND,   "sound",      nullptr},
        {0x0,                       CMD_FULLSCREEN,  "fullscreen", nullptr},
        {0x0,                       CMD_22050HZ,     "22050",      nullptr},
        {0x0,                       CMD_NO_WARNINGS, "nowarnings", nullptr}
    };

    // load engine-specific launch flags
    SCmdRegisterArgList(s_argList, std::size(s_argList));

    // parse command line
    return SCmdProcessCommandLine(0, 0);
}

void ProcessCommandLine() {
    static ARGLIST s_wowArgList[] = {
        {0x0,                       WOWCMD_RES_800x600,   "800x600",    nullptr},
        {0x0,                       WOWCMD_RES_1024x768,  "1024x768",   nullptr},
        {0x0,                       WOWCMD_RES_1280x960,  "1280x960",   nullptr},
        {0x0,                       WOWCMD_RES_1280x1024, "1280x1024",  nullptr},
        {0x0,                       WOWCMD_RES_1600x1200, "1600x1200",  nullptr},
        {0x0,                       WOWCMD_16_BIT,        "16bit",      nullptr},
        {0x0,                       WOWCMD_UP_TO_DATE,    "uptodate",   nullptr},
        {0x0,                       WOWCMD_NO_SOUND,      "nosound",    nullptr},
        {0x0,                       WOWCMD_SOUND_CHAOS,   "soundchaos", nullptr},
        {0x0,                       WOWCMD_NO_FIX_LAG,    "nofixlag",   nullptr},
        {0x0,                       WOWCMD_DEPTH_16,      "d16",        nullptr},
        {0x0,                       WOWCMD_DEPTH_24,      "d24",        nullptr},
        {0x0,                       WOWCMD_DEPTH_32,      "d32",        nullptr},
        {0x0,                       WOWCMD_WINDOWED,      "windowed",   nullptr},
        {0x0,                       WOWCMD_HW_DETECT,     "hwdetect",   nullptr},
        {0x0,                       WOWCMD_CONSOLE,       "console",    nullptr},
        {STORM_COMMAND_TYPE_STRING, WOWCMD_GX_OVERRIDE,   "gxoverride", nullptr}
    };

    // Load wow-specific launch flags
    SCmdRegisterArgList(s_wowArgList, std::size(s_wowArgList));

    CmdLineProcess();
}

const char* CmdLineGetString(uint32_t opt) {
    static char buffer[260] = {0};

    SCmdGetString(opt, buffer, 260);

    return buffer;
}

uint32_t CmdLineGetUint(uint32_t opt) {
    return SCmdGetNum(opt);
}

int32_t CmdLineGetBool(uint32_t opt) {
    return SCmdGetBool(opt);
}
