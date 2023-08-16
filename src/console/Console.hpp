#ifndef CONSOLE_CONSOLE_HPP
#define CONSOLE_CONSOLE_HPP

#include "console/Types.hpp"
#include "event/Types.hpp"
#include <cstdint>

int32_t ConsoleAccessGetEnabled();

void ConsoleAccessSetEnabled(int32_t enable);

int32_t ConsoleGetActive();

float ConsoleGetFontHeight();

float ConsoleGetLines();

float ConsoleGetHeight();

KEY ConsoleGetHotKey();

CONSOLERESIZESTATE ConsoleGetResizeState();

void ConsoleSetActive(int32_t active);

void ConsoleSetHotKey(KEY hotkey);

void ConsoleSetHeight(float height);

void ConsoleSetResizeState(CONSOLERESIZESTATE state);

void ConsolePostClose();

#endif  // ifndef CONSOLE_CONSOLE_HPP
