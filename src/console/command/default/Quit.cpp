#include "console/Command.hpp"
#include "console/Console.hpp"

int32_t ConsoleCommand_Quit(const char* command, const char* arguments) {
    ConsolePostClose();
    return 1;
}
