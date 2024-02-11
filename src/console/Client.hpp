#ifndef CONSOLE_CLIENT_HPP
#define CONSOLE_CLIENT_HPP

#include <cstdint>

void ConsoleInitializeClientCommand();

void ConsoleInitializeClientCVar(const char* a1);

int32_t ConsoleLoadClientCVar(const char* a1);

#endif
