#ifndef UI_SCRIPT_FUNCTIONS_HPP
#define UI_SCRIPT_FUNCTIONS_HPP

#include "ui/Types.hpp"
#include <cstdint>

struct lua_State;

#define NUM_SCRIPT_FUNCTIONS_CHAR_CREATE 32
#define NUM_SCRIPT_FUNCTIONS_CHAR_SELECT 13
#define NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS 113
#define NUM_SCRIPT_FUNCTIONS_REALM_LIST 14
#define NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME 7
#define NUM_SCRIPT_FUNCTIONS_SYSTEM 7

namespace FrameScript {
    extern FrameScript_Method s_ScriptFunctions_CharCreate[NUM_SCRIPT_FUNCTIONS_CHAR_CREATE];
    extern FrameScript_Method s_ScriptFunctions_CharSelect[NUM_SCRIPT_FUNCTIONS_CHAR_SELECT];
    extern FrameScript_Method s_ScriptFunctions_GlueScriptEvents[NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS];
    extern FrameScript_Method s_ScriptFunctions_RealmList[NUM_SCRIPT_FUNCTIONS_REALM_LIST];
    extern FrameScript_Method s_ScriptFunctions_SimpleFrame[NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME];
    extern FrameScript_Method s_ScriptFunctions_System[NUM_SCRIPT_FUNCTIONS_SYSTEM];
}

void CharacterCreateRegisterScriptFunctions();

void CharSelectRegisterScriptFunctions();

void GlueScriptEventsRegisterFunctions();

void RealmListRegisterScriptFunctions();

void RegisterSimpleFrameScriptMethods();

void SystemRegisterFunctions();

#endif
