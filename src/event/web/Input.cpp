#include "event/Input.hpp"
#include <emscripten/html5.h>
#include <cstring>

// Key code translation from DOM key codes to engine KEY values
static KEY TranslateKeyCode(const char* code) {
    // Letters
    if (strcmp(code, "KeyA") == 0) return KEY_A;
    if (strcmp(code, "KeyB") == 0) return KEY_B;
    if (strcmp(code, "KeyC") == 0) return KEY_C;
    if (strcmp(code, "KeyD") == 0) return KEY_D;
    if (strcmp(code, "KeyE") == 0) return KEY_E;
    if (strcmp(code, "KeyF") == 0) return KEY_F;
    if (strcmp(code, "KeyG") == 0) return KEY_G;
    if (strcmp(code, "KeyH") == 0) return KEY_H;
    if (strcmp(code, "KeyI") == 0) return KEY_I;
    if (strcmp(code, "KeyJ") == 0) return KEY_J;
    if (strcmp(code, "KeyK") == 0) return KEY_K;
    if (strcmp(code, "KeyL") == 0) return KEY_L;
    if (strcmp(code, "KeyM") == 0) return KEY_M;
    if (strcmp(code, "KeyN") == 0) return KEY_N;
    if (strcmp(code, "KeyO") == 0) return KEY_O;
    if (strcmp(code, "KeyP") == 0) return KEY_P;
    if (strcmp(code, "KeyQ") == 0) return KEY_Q;
    if (strcmp(code, "KeyR") == 0) return KEY_R;
    if (strcmp(code, "KeyS") == 0) return KEY_S;
    if (strcmp(code, "KeyT") == 0) return KEY_T;
    if (strcmp(code, "KeyU") == 0) return KEY_U;
    if (strcmp(code, "KeyV") == 0) return KEY_V;
    if (strcmp(code, "KeyW") == 0) return KEY_W;
    if (strcmp(code, "KeyX") == 0) return KEY_X;
    if (strcmp(code, "KeyY") == 0) return KEY_Y;
    if (strcmp(code, "KeyZ") == 0) return KEY_Z;

    // Numbers
    if (strcmp(code, "Digit0") == 0) return KEY_0;
    if (strcmp(code, "Digit1") == 0) return KEY_1;
    if (strcmp(code, "Digit2") == 0) return KEY_2;
    if (strcmp(code, "Digit3") == 0) return KEY_3;
    if (strcmp(code, "Digit4") == 0) return KEY_4;
    if (strcmp(code, "Digit5") == 0) return KEY_5;
    if (strcmp(code, "Digit6") == 0) return KEY_6;
    if (strcmp(code, "Digit7") == 0) return KEY_7;
    if (strcmp(code, "Digit8") == 0) return KEY_8;
    if (strcmp(code, "Digit9") == 0) return KEY_9;

    // Function keys
    if (strcmp(code, "F1") == 0) return KEY_F1;
    if (strcmp(code, "F2") == 0) return KEY_F2;
    if (strcmp(code, "F3") == 0) return KEY_F3;
    if (strcmp(code, "F4") == 0) return KEY_F4;
    if (strcmp(code, "F5") == 0) return KEY_F5;
    if (strcmp(code, "F6") == 0) return KEY_F6;
    if (strcmp(code, "F7") == 0) return KEY_F7;
    if (strcmp(code, "F8") == 0) return KEY_F8;
    if (strcmp(code, "F9") == 0) return KEY_F9;
    if (strcmp(code, "F10") == 0) return KEY_F10;
    if (strcmp(code, "F11") == 0) return KEY_F11;
    if (strcmp(code, "F12") == 0) return KEY_F12;

    // Special keys
    if (strcmp(code, "Escape") == 0) return KEY_ESCAPE;
    if (strcmp(code, "Enter") == 0) return KEY_ENTER;
    if (strcmp(code, "NumpadEnter") == 0) return KEY_ENTER;
    if (strcmp(code, "Backspace") == 0) return KEY_BACKSPACE;
    if (strcmp(code, "Tab") == 0) return KEY_TAB;
    if (strcmp(code, "Space") == 0) return KEY_SPACE;

    // Arrow keys
    if (strcmp(code, "ArrowLeft") == 0) return KEY_LEFT;
    if (strcmp(code, "ArrowUp") == 0) return KEY_UP;
    if (strcmp(code, "ArrowRight") == 0) return KEY_RIGHT;
    if (strcmp(code, "ArrowDown") == 0) return KEY_DOWN;

    // Navigation keys
    if (strcmp(code, "Insert") == 0) return KEY_INSERT;
    if (strcmp(code, "Delete") == 0) return KEY_DELETE;
    if (strcmp(code, "Home") == 0) return KEY_HOME;
    if (strcmp(code, "End") == 0) return KEY_END;
    if (strcmp(code, "PageUp") == 0) return KEY_PAGEUP;
    if (strcmp(code, "PageDown") == 0) return KEY_PAGEDOWN;

    // Modifier keys
    if (strcmp(code, "ShiftLeft") == 0) return KEY_LSHIFT;
    if (strcmp(code, "ShiftRight") == 0) return KEY_RSHIFT;
    if (strcmp(code, "ControlLeft") == 0) return KEY_LCONTROL;
    if (strcmp(code, "ControlRight") == 0) return KEY_RCONTROL;
    if (strcmp(code, "AltLeft") == 0) return KEY_LALT;
    if (strcmp(code, "AltRight") == 0) return KEY_RALT;

    // Lock keys
    if (strcmp(code, "CapsLock") == 0) return KEY_CAPSLOCK;
    if (strcmp(code, "NumLock") == 0) return KEY_NUMLOCK;
    if (strcmp(code, "ScrollLock") == 0) return KEY_SCROLLLOCK;

    // Numpad
    if (strcmp(code, "Numpad0") == 0) return KEY_NUMPAD0;
    if (strcmp(code, "Numpad1") == 0) return KEY_NUMPAD1;
    if (strcmp(code, "Numpad2") == 0) return KEY_NUMPAD2;
    if (strcmp(code, "Numpad3") == 0) return KEY_NUMPAD3;
    if (strcmp(code, "Numpad4") == 0) return KEY_NUMPAD4;
    if (strcmp(code, "Numpad5") == 0) return KEY_NUMPAD5;
    if (strcmp(code, "Numpad6") == 0) return KEY_NUMPAD6;
    if (strcmp(code, "Numpad7") == 0) return KEY_NUMPAD7;
    if (strcmp(code, "Numpad8") == 0) return KEY_NUMPAD8;
    if (strcmp(code, "Numpad9") == 0) return KEY_NUMPAD9;
    if (strcmp(code, "NumpadAdd") == 0) return KEY_NUMPAD_PLUS;
    if (strcmp(code, "NumpadSubtract") == 0) return KEY_NUMPAD_MINUS;
    if (strcmp(code, "NumpadMultiply") == 0) return KEY_NUMPAD_MULTIPLY;
    if (strcmp(code, "NumpadDivide") == 0) return KEY_NUMPAD_DIVIDE;
    if (strcmp(code, "NumpadDecimal") == 0) return KEY_NUMPAD_DECIMAL;

    // Punctuation
    if (strcmp(code, "Minus") == 0) return KEY_MINUS;
    if (strcmp(code, "Equal") == 0) return KEY_PLUS;
    if (strcmp(code, "BracketLeft") == 0) return KEY_BRACKET_OPEN;
    if (strcmp(code, "BracketRight") == 0) return KEY_BRACKET_CLOSE;
    if (strcmp(code, "Semicolon") == 0) return KEY_SEMICOLON;
    if (strcmp(code, "Quote") == 0) return KEY_APOSTROPHE;
    if (strcmp(code, "Backquote") == 0) return KEY_TILDE;
    if (strcmp(code, "Backslash") == 0) return KEY_BACKSLASH;
    if (strcmp(code, "Comma") == 0) return KEY_COMMA;
    if (strcmp(code, "Period") == 0) return KEY_PERIOD;
    if (strcmp(code, "Slash") == 0) return KEY_SLASH;

    return KEY_NONE;
}

static uint32_t GetMetaKeyState(const EmscriptenKeyboardEvent* e) {
    uint32_t state = 0;
    if (e->shiftKey) {
        state |= (1 << KEY_LSHIFT);
    }
    if (e->ctrlKey) {
        state |= (1 << KEY_LCONTROL);
    }
    if (e->altKey) {
        state |= (1 << KEY_LALT);
    }
    return state;
}

static MOUSEBUTTON TranslateMouseButton(int button) {
    switch (button) {
        case 0: return MOUSE_BUTTON_LEFT;
        case 1: return MOUSE_BUTTON_MIDDLE;
        case 2: return MOUSE_BUTTON_RIGHT;
        case 3: return MOUSE_BUTTON_XBUTTON1;
        case 4: return MOUSE_BUTTON_XBUTTON2;
        default: return MOUSE_BUTTON_NONE;
    }
}

// Keyboard event callback
static EM_BOOL OnKeyDown(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    KEY key = TranslateKeyCode(e->code);
    if (key != KEY_NONE) {
        uint32_t metaState = GetMetaKeyState(e);
        OsQueuePut(OS_INPUT_KEY_DOWN, static_cast<int32_t>(key), metaState, e->repeat ? 1 : 0, 0);
    }

    // Also send char event for printable characters
    if (e->key[0] != '\0' && e->key[1] == '\0') {
        uint32_t metaState = GetMetaKeyState(e);
        OsQueuePut(OS_INPUT_CHAR, static_cast<int32_t>(e->key[0]), metaState, e->repeat ? 1 : 0, 0);
    }

    return EM_TRUE;
}

static EM_BOOL OnKeyUp(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    KEY key = TranslateKeyCode(e->code);
    if (key != KEY_NONE) {
        uint32_t metaState = GetMetaKeyState(e);
        OsQueuePut(OS_INPUT_KEY_UP, static_cast<int32_t>(key), metaState, 0, 0);
    }
    return EM_TRUE;
}

// Mouse event callbacks
static EM_BOOL OnMouseDown(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    MOUSEBUTTON button = TranslateMouseButton(e->button);
    int32_t x = e->targetX;
    int32_t y = e->targetY;
    OsQueuePut(OS_INPUT_MOUSE_DOWN, static_cast<int32_t>(button), x, y, 0);
    return EM_TRUE;
}

static EM_BOOL OnMouseUp(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    MOUSEBUTTON button = TranslateMouseButton(e->button);
    int32_t x = e->targetX;
    int32_t y = e->targetY;
    OsQueuePut(OS_INPUT_MOUSE_UP, static_cast<int32_t>(button), x, y, 0);
    return EM_TRUE;
}

static EM_BOOL OnMouseMove(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    if (Input::s_osMouseMode == OS_MOUSE_MODE_RELATIVE) {
        // Relative mouse movement (pointer locked)
        int32_t dx = e->movementX;
        int32_t dy = e->movementY;
        OsQueuePut(OS_INPUT_MOUSE_MOVE_RELATIVE, dx, dy, 0, 0);
    } else {
        // Absolute mouse position
        int32_t x = e->targetX;
        int32_t y = e->targetY;
        OsQueuePut(OS_INPUT_MOUSE_MOVE, 0, x, y, 0);
    }
    return EM_TRUE;
}

static EM_BOOL OnWheel(int eventType, const EmscriptenWheelEvent* e, void* userData) {
    // Normalize wheel delta
    int32_t delta = static_cast<int32_t>(-e->deltaY);
    if (e->deltaMode == DOM_DELTA_LINE) {
        delta *= 40; // Approximate line height
    } else if (e->deltaMode == DOM_DELTA_PAGE) {
        delta *= 400; // Approximate page height
    }
    OsQueuePut(OS_INPUT_MOUSE_WHEEL, delta, 0, 0, 0);
    return EM_TRUE;
}

static EM_BOOL OnFocus(int eventType, const EmscriptenFocusEvent* e, void* userData) {
    OsQueuePut(OS_INPUT_FOCUS, 1, 0, 0, 0);
    return EM_TRUE;
}

static EM_BOOL OnBlur(int eventType, const EmscriptenFocusEvent* e, void* userData) {
    OsQueuePut(OS_INPUT_FOCUS, 0, 0, 0, 0);
    return EM_TRUE;
}

static EM_BOOL OnPointerLockChange(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData) {
    if (!e->isActive && Input::s_osMouseMode == OS_MOUSE_MODE_RELATIVE) {
        // Pointer lock was released externally
        Input::s_osMouseMode = OS_MOUSE_MODE_NORMAL;
    }
    return EM_TRUE;
}

void OsInputInitialize() {
    // Register keyboard events on document
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE, OnKeyDown);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE, OnKeyUp);

    // Register mouse events on canvas
    emscripten_set_mousedown_callback("#canvas", nullptr, EM_TRUE, OnMouseDown);
    emscripten_set_mouseup_callback("#canvas", nullptr, EM_TRUE, OnMouseUp);
    emscripten_set_mousemove_callback("#canvas", nullptr, EM_TRUE, OnMouseMove);
    emscripten_set_wheel_callback("#canvas", nullptr, EM_TRUE, OnWheel);

    // Register focus events
    emscripten_set_focus_callback("#canvas", nullptr, EM_TRUE, OnFocus);
    emscripten_set_blur_callback("#canvas", nullptr, EM_TRUE, OnBlur);

    // Register pointer lock change event
    emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE, OnPointerLockChange);
}

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    return OsQueueGet(id, param0, param1, param2, param3);
}

void OsInputSetMouseMode(OS_MOUSE_MODE mode) {
    if (mode == Input::s_osMouseMode) {
        return;
    }

    Input::s_osMouseMode = mode;

    if (mode == OS_MOUSE_MODE_RELATIVE) {
        // Request pointer lock
        emscripten_request_pointerlock("#canvas", EM_TRUE);
    } else {
        // Exit pointer lock
        emscripten_exit_pointerlock();
    }
}

int32_t OsWindowProc(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam) {
    // Not used on web - events come through Emscripten callbacks
    return 0;
}

bool OsInputIsUsingCocoaEventLoop() {
    return false;
}
