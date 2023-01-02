#ifndef CLIENT_CONSOLE_HPP
#define CLIENT_CONSOLE_HPP

#include "gx/CGxFormat.hpp"

struct DefaultSettings {
    CGxFormat format;
};

void ConsoleInitializeClientCommand();

void ConsoleInitializeClientCVar(const char* a1);

void ConsoleDeviceInitialize(const char* title);

#endif
