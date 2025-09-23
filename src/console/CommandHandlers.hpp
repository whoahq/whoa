#ifndef CONSOLE_COMMAND_HANDLERS_HPP
#define CONSOLE_COMMAND_HANDLERS_HPP

#include <cstdint>

int32_t ConsoleCommand_Help(const char* command, const char* arguments);

int32_t ConsoleCommand_Quit(const char* command, const char* arguments);

int32_t ConsoleCommand_SetMap(const char* command, const char* arguments);

int32_t ConsoleCommand_Ver(const char* command, const char* arguments);

#endif
