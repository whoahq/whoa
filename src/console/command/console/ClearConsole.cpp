#include "console/Command.hpp"

int32_t ConsoleCommand_ClearConsole(const char* command, const char* arguments) {
    ConsoleClear();
    return 1;
}
