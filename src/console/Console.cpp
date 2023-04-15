#include "console/Console.hpp"

static int32_t s_active;
static int32_t s_consoleAccessEnabled;

int32_t ConsoleAccessGetEnabled() {
    return s_consoleAccessEnabled;
}

void ConsoleAccessSetEnabled(int32_t enable) {
    s_consoleAccessEnabled = enable;
}

int32_t ConsoleGetActive() {
    return s_active;
}

void ConsoleSetActive(int32_t active) {
    s_active = active;
}
