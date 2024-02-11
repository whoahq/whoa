#include "console/Console.hpp"
#include "event/Context.hpp"
#include "event/Event.hpp"

static int32_t s_active;
static int32_t s_consoleAccessEnabled;
static KEY s_consoleKey = KEY_TILDE;
static float s_consoleLines = 10.0f;
static float s_fontHeight = 0.02f;
static float s_consoleHeight = s_consoleLines * s_fontHeight;
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

float ConsoleGetFontHeight() {
    return s_fontHeight;
}

float ConsoleGetHeight() {
    return s_consoleHeight;
}

float ConsoleGetLines() {
    return s_consoleLines;
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

void ConsoleSetHeight(float height) {
    s_consoleHeight = height;
}

void ConsolePostClose() {
    EventPostCloseEx(EventGetCurrentContext());
}
