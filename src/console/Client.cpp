#include "console/Client.hpp"
#include "console/Command.hpp"

void ConsoleInitializeClientCommand() {
    ConsoleCommandInitialize();
    ConsoleInitializeCommonCommand();
    ConsoleInitializeDebugCommand();
}

void ConsoleInitializeClientCVar(const char* a1) {
    // TODO
}
