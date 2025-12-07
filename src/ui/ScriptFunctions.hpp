#ifndef UI_SCRIPT_FUNCTIONS_HPP
#define UI_SCRIPT_FUNCTIONS_HPP

#include "ui/Types.hpp"
#include <cstdint>

struct lua_State;

#define NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME 7
#define NUM_SCRIPT_FUNCTIONS_SYSTEM 7

namespace FrameScript {
    extern FrameScript_Method s_ScriptFunctions_SimpleFrame[NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME];
    extern FrameScript_Method s_ScriptFunctions_System[NUM_SCRIPT_FUNCTIONS_SYSTEM];
}

void RegisterSimpleFrameScriptMethods();

void SystemRegisterFunctions();

#endif
