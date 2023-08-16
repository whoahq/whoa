#ifndef CONSOLE_LINE_HPP
#define CONSOLE_LINE_HPP

#include "console/Types.hpp"
#include "gx/Font.hpp"

#include <storm/List.hpp>

#define CONSOLE_LINES_MAX 256
#define CONSOLE_LINE_LENGTH 1024
#define CONSOLE_LINE_PREALLOC 16

class CONSOLELINE : public TSLinkedNode<CONSOLELINE> {
    public:
        char* buffer;
        uint32_t chars;
        uint32_t charsalloc;
        uint32_t inputpos;
        uint32_t inputstart;
        COLOR_T colorType;
        CGxString* fontPointer;

        ~CONSOLELINE();
};

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
