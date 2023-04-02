#ifndef CONSOLE_CLIENT_HPP
#define CONSOLE_CLIENT_HPP

#include "gx/CGxFormat.hpp"

enum HIGHLIGHTSTATE
{
    HS_NONE = 0x0,
    HS_HIGHLIGHTING = 0x1,
    HS_ENDHIGHLIGHT = 0x2,
    NUM_HIGHLIGHTSTATES
};

struct DefaultSettings {
    CGxFormat format;
};

void ConsoleInitializeClientCommand();

void ConsoleInitializeClientCVar(const char* a1);

void ConsoleDeviceInitialize(const char* title);

#endif
