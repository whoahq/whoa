#ifndef CONSOLE_CONSOLE_HPP
#define CONSOLE_CONSOLE_HPP

#include "console/Types.hpp"
#include "event/Types.hpp"
#include <cstdint>

int32_t ConsoleAccessGetEnabled();

void ConsoleAccessSetEnabled(int32_t enable);

int32_t ConsoleGetActive();

KEY ConsoleGetHotKey();

CONSOLERESIZESTATE ConsoleGetResizeState();

void ConsoleSetActive(int32_t active);

void ConsoleSetHotKey(KEY hotkey);

void ConsoleSetResizeState(CONSOLERESIZESTATE state);

void ConsoleWrite(const char* text, COLOR_T color);

#endif  // ifndef CONSOLE_CONSOLE_HPP
