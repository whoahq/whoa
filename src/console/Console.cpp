#include "console/Console.hpp"

const int32_t HISTORY_DEPTH = 32;
const int32_t BUFFER_SIZE = 1024;

static int32_t s_active;
static int32_t s_consoleAccessEnabled;
static KEY s_consoleKey = KEY_TILDE;
static float s_fontHeight = 0.02f;
static float s_consoleLines = 10.0f;
static float s_consoleHeight = s_fontHeight * s_consoleLines;
static CONSOLERESIZESTATE s_consoleResizeState;
static uint32_t s_repeatCount;
static char s_repeatBuffer[HISTORY_DEPTH][BUFFER_SIZE];

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

KEY ConsoleGetHotKey() {
    return s_consoleKey;
}

char* ConsoleGetRepeatBuffer() {
    return *s_repeatBuffer;
}

uint32_t ConsoleGetRepeatCount() {
    return s_repeatCount;
}

CONSOLERESIZESTATE ConsoleGetResizeState() {
    return s_consoleResizeState;
}

void ConsoleSetActive(int32_t active) {
    s_active = active;
}

void ConsoleSetFontHeight(float fontHeight) {
    s_fontHeight = fontHeight;
}

void ConsoleSetHotKey(KEY hotkey) {
    s_consoleKey = hotkey;
}

void ConsoleSetRepeatCount(uint32_t repeatCount) {
    s_repeatCount = repeatCount;
}

void ConsoleSetResizeState(CONSOLERESIZESTATE resizeState) {
    s_consoleResizeState = resizeState;
}
