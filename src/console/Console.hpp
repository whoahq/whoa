#ifndef CONSOLE_CONSOLE_HPP
#define CONSOLE_CONSOLE_HPP

#include "console/Types.hpp"
#include "event/Types.hpp"
#include <cstdint>

int32_t ConsoleAccessGetEnabled();

void ConsoleAccessSetEnabled(int32_t enable);

int32_t ConsoleGetActive();

float ConsoleGetFontHeight();

float ConsoleGetHeight();

char* ConsoleGetRepeatBuffer();

uint32_t ConsoleGetRepeatCount();

CONSOLERESIZESTATE ConsoleGetResizeState();

KEY ConsoleGetHotKey();

void ConsoleSetActive(int32_t active);

void ConsoleSetFontHeight(float fontHeight);

void ConsoleSetHeight(float height);

void ConsoleSetHotKey(KEY hotkey);

void ConsoleSetRepeatCount(uint32_t repeatCount);

void ConsoleSetResizeState(CONSOLERESIZESTATE resizeState);

#endif  // ifndef CONSOLE_CONSOLE_HPP
