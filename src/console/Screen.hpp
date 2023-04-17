#ifndef CONSOLE_SCREEN_HPP
#define CONSOLE_SCREEN_HPP

#include "gx/Screen.hpp"

HLAYER ConsoleScreenGetBackgroundLayer();

HLAYER ConsoleScreenGetTextLayer();

RECTF* ConsoleScreenGetRect();

void ConsoleScreenInitialize(const char* title);

#endif
