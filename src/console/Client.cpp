#include "console/Client.hpp"
#include "console/Command.hpp"
#include "console/CVar.hpp"

void ConsoleInitializeClientCommand() {
    ConsoleCommandInitialize();
    ConsoleInitializeCommonCommand();
    ConsoleInitializeDebugCommand();
}

void ConsoleInitializeClientCVar(const char* a1) {
    CVar::Initialize(a1);
}

int32_t ConsoleLoadClientCVar(const char* a1) {
    return CVar::Load(a1);
}
