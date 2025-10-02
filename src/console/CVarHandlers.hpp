#ifndef CONSOLE_C_VAR_HANDLERS_HPP
#define CONSOLE_C_VAR_HANDLERS_HPP

#include <cstdint>

int32_t CVarDefaultCommandHandler(const char* command, const char* arguments);

int32_t CVarListCommandHandler(const char* command, const char* arguments);

int32_t CVarSetCommandHandler(const char* command, const char* arguments);

int32_t CVarResetCommandHandler(const char* command, const char* arguments);

#endif
