#include "console/Command.hpp"
#include "console/Console.hpp"

int32_t ConsoleCommand_CloseConsole(const char* command, const char* arguments) {
    ConsoleSetActive(false);
    return 1;
}
