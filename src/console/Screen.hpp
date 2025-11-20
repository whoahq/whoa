#ifndef CONSOLE_SCREEN_HPP
#define CONSOLE_SCREEN_HPP

#include "console/Types.hpp"
#include <storm/List.hpp>

class CGxString;

class CONSOLELINE : public TSLinkedNode<CONSOLELINE> {
    public:
        // Member variables
        char* buffer;
        uint32_t chars;
        uint32_t charsalloc;
        uint32_t inputpos;
        uint32_t inputstart;
        COLOR_T colorType;
        CGxString* fontPointer;

        // Member functions
        ~CONSOLELINE();
};

void ConsoleScreenAnimate(float elapsedSec);

void ConsoleScreenInitialize(const char* title);

#endif
