#ifndef CONSOLE_CONSOLE_HPP
#define CONSOLE_CONSOLE_HPP

#include "event/Types.hpp"
#include <cstdint>

int32_t ConsoleAccessGetEnabled();

void ConsoleAccessSetEnabled(int32_t enable);

int32_t ConsoleGetActive();

KEY ConsoleGetHotKey();

void ConsoleSetActive(int32_t active);

void ConsoleSetHotKey(KEY hotkey);

#endif  // ifndef CONSOLE_CONSOLE_HPP
