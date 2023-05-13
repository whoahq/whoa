#include "console/Console.hpp"

static int32_t s_active;
static int32_t s_consoleAccessEnabled;
static KEY s_consoleKey = KEY_TILDE;
static CONSOLERESIZESTATE s_consoleResizeState = CS_NONE;

int32_t ConsoleAccessGetEnabled() {
    return s_consoleAccessEnabled;
}

void ConsoleAccessSetEnabled(int32_t enable) {
    s_consoleAccessEnabled = enable;
}

int32_t ConsoleGetActive() {
    return s_active;
}

KEY ConsoleGetHotKey() {
    return s_consoleKey;
}

CONSOLERESIZESTATE ConsoleGetResizeState() {
    return s_consoleResizeState;
}

void ConsoleSetActive(int32_t active) {
    s_active = active;
}

void ConsoleSetHotKey(KEY hotkey) {
    s_consoleKey = hotkey;
}

void ConsoleSetResizeState(CONSOLERESIZESTATE state) {
    s_consoleResizeState = state;
}
