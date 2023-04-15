#ifndef CONSOLE_CONSOLE_HPP
#define CONSOLE_CONSOLE_HPP

#include <cstdint>

int32_t ConsoleAccessGetEnabled();

void ConsoleAccessSetEnabled(int32_t enable);

int32_t ConsoleGetActive();

void ConsoleSetActive(int32_t active);

#endif  // ifndef CONSOLE_CONSOLE_HPP
