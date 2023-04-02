#ifndef CONSOLE_DEVICE_HPP
#define CONSOLE_DEVICE_HPP

#include "gx/CGxFormat.hpp"

struct DefaultSettings {
    CGxFormat format;
};

void ConsoleDeviceInitialize(const char* title);

#endif
