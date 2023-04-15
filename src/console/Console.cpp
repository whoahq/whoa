#include "console/Console.hpp"

static int32_t s_active;

int32_t ConsoleGetActive() {
    return s_active;
}

void ConsoleSetActive(int32_t active) {
    s_active = active;
}
