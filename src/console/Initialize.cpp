#include "console/Initialize.hpp"
#include "console/Command.hpp"
#include "console/CommandHandlers.hpp"
#include "console/CVar.hpp"

static const char* s_filename;

void ConsoleInitializeCommonCommand() {
    ConsoleCommandRegister("quit",      ConsoleCommand_Quit,    DEFAULT,    NOHELP);
    ConsoleCommandRegister("ver",       ConsoleCommand_Ver,     DEFAULT,    NOHELP);
    ConsoleCommandRegister("setmap",    ConsoleCommand_SetMap,  DEFAULT,    NOHELP);
}

void ConsoleInitializeDebugCommand() {
    // Unknown without debug build
}

void ConsoleInitializeClientCommand() {
    ConsoleCommandInitialize();
    ConsoleInitializeCommonCommand();
    ConsoleInitializeDebugCommand();
}

void ConsoleInitializeClientCVar(const char* filename) {
    s_filename = filename;

    CVar::Initialize();

    CVar::Load(s_filename);
}
