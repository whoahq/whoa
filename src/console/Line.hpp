#ifndef CONSOLE_LINE_HPP
#define CONSOLE_LINE_HPP

#include "console/Types.hpp"

#include <storm/List.hpp>

#define CONSOLE_LINES_MAX 256
#define CONSOLE_LINE_LENGTH 1024
#define CONSOLE_LINE_PREALLOC 16

void ConsoleWrite(const char* str, COLOR_T color);
void ConsoleWriteA(const char* str, COLOR_T color, ...);

void PasteInInputLine(char* characters);

void MoveLinePtr(int32_t direction, int32_t modifier);

void BackspaceLine(CONSOLELINE* line);

void ReserveInputSpace(CONSOLELINE* line, size_t len);

CONSOLELINE* GetInputLine();
CONSOLELINE* GetCurrentLine();

CONSOLELINE* GetLineAtMousePosition(float y);

void ConsoleClear();

#endif
