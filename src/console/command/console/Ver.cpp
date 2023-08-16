#include "console/Command.hpp"
#include "console/Line.hpp"

int32_t ConsoleCommand_Ver(const char* command, const char* arguments) {
    ConsoleWrite("Whoa <https://github.com/whoahq/whoa>", DEFAULT_COLOR);
    return 1;
}
