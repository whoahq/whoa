#include "console/Client.hpp"
#include "event/Event.hpp"

void ConsoleInitializeClientCommand() {
    // TODO
}

void ConsoleInitializeClientCVar(const char* a1) {
    // TODO
}

void ConsolePostClose() {
    EventPostCloseEx(EventGetCurrentContext());
}
