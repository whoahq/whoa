#ifndef EVENT_TYPES_HPP
#define EVENT_TYPES_HPP

#include <cstdint>

typedef void* HEVENTCONTEXT;
typedef int32_t (*EVENTHANDLERFUNC)(const void*, void*);

enum EVENTID {
    EVENT_ID_0                  = 0,
    EVENT_ID_CHAR               = 1,
    EVENT_ID_FOCUS              = 2,
    EVENT_ID_3                  = 3,
    EVENT_ID_DESTROY            = 4,
    EVENT_ID_5                  = 5,
    EVENT_ID_IDLE               = 6,
    EVENT_ID_POLL               = 7,
    EVENT_ID_INITIALIZE         = 8,
    EVENT_ID_KEYDOWN            = 9,
    EVENT_ID_KEYUP              = 10,
    EVENT_ID_KEYDOWN_REPEATING  = 11,
    EVENT_ID_MOUSEDOWN          = 12,
    EVENT_ID_MOUSEMOVE          = 13,
    EVENT_ID_MOUSEMOVE_RELATIVE = 14,
    EVENT_ID_MOUSEUP            = 15,
    EVENT_ID_MOUSEMODE_CHANGED  = 16,
    EVENT_ID_MOUSEWHEEL         = 17,
    EVENT_ID_18                 = 18,
    EVENT_ID_19                 = 19,
    EVENT_ID_20                 = 20,
    EVENT_ID_21                 = 21,
    EVENT_ID_22                 = 22,
    EVENT_ID_PAINT              = 23,
    EVENT_ID_NET_DATA           = 24,
    EVENT_ID_NET_CONNECT        = 25,
    EVENT_ID_NET_DISCONNECT     = 26,
    EVENT_ID_NET_CANTCONNECT    = 27,
    EVENT_ID_NET_DESTROY        = 28,
    EVENT_ID_NET_AUTH_CHALLENGE = 29,
    EVENT_ID_30                 = 30,
    EVENT_ID_31                 = 31,
    EVENT_ID_32                 = 32,
    EVENT_ID_33                 = 33,
    EVENT_ID_IME                = 34,
    EVENT_ID_SIZE               = 35,
    EVENTIDS                    = 36
};

enum KEY {
    KEY_NONE = 0xFFFFFFFF,
    KEY_LSHIFT = 0x0,
    KEY_RSHIFT = 0x1,
    KEY_LCONTROL = 0x2,
    KEY_RCONTROL = 0x3,
    KEY_LALT = 0x4,
    KEY_RALT = 0x5,
    KEY_LASTMETAKEY = 0x5,
    KEY_SPACE = 0x20,
    KEY_0 = 0x30,
    KEY_1 = 0x31,
    KEY_2 = 0x32,
    KEY_3 = 0x33,
    KEY_4 = 0x34,
    KEY_5 = 0x35,
    KEY_6 = 0x36,
    KEY_7 = 0x37,
    KEY_8 = 0x38,
    KEY_9 = 0x39,
    KEY_A = 0x41,
    KEY_B = 0x42,
    KEY_C = 0x43,
    KEY_D = 0x44,
    KEY_E = 0x45,
    KEY_F = 0x46,
    KEY_G = 0x47,
    KEY_H = 0x48,
    KEY_I = 0x49,
    KEY_J = 0x4A,
    KEY_K = 0x4B,
    KEY_L = 0x4C,
    KEY_M = 0x4D,
    KEY_N = 0x4E,
    KEY_O = 0x4F,
    KEY_P = 0x50,
    KEY_Q = 0x51,
    KEY_R = 0x52,
    KEY_S = 0x53,
    KEY_T = 0x54,
    KEY_U = 0x55,
    KEY_V = 0x56,
    KEY_W = 0x57,
    KEY_X = 0x58,
    KEY_Y = 0x59,
    KEY_Z = 0x5A,
    KEY_TILDE = 0x60,
    KEY_NUMPAD0 = 0x100,
    KEY_NUMPAD1 = 0x101,
    KEY_NUMPAD2 = 0x102,
    KEY_NUMPAD3 = 0x103,
    KEY_NUMPAD4 = 0x104,
    KEY_NUMPAD5 = 0x105,
    KEY_NUMPAD6 = 0x106,
    KEY_NUMPAD7 = 0x107,
    KEY_NUMPAD8 = 0x108,
    KEY_NUMPAD9 = 0x109,
    KEY_NUMPAD_PLUS = 0x10A,
    KEY_NUMPAD_MINUS = 0x10B,
    KEY_NUMPAD_MULTIPLY = 0x10C,
    KEY_NUMPAD_DIVIDE = 0x10D,
    KEY_NUMPAD_DECIMAL = 0x10E,
    KEY_NUMPAD_EQUALS = 0x30C,
    KEY_PLUS = 0x3D,
    KEY_MINUS = 0x2D,
    KEY_BRACKET_OPEN = 0x5B,
    KEY_BRACKET_CLOSE = 0x5D,
    KEY_SLASH = 0x2F,
    KEY_BACKSLASH = 0x5C,
    KEY_SEMICOLON = 0x3B,
    KEY_APOSTROPHE = 0x27,
    KEY_COMMA = 0x2C,
    KEY_PERIOD = 0x2E,
    KEY_ESCAPE = 0x200,
    KEY_ENTER = 0x201,
    KEY_BACKSPACE = 0x202,
    KEY_TAB = 0x203,
    KEY_LEFT = 0x204,
    KEY_UP = 0x205,
    KEY_RIGHT = 0x206,
    KEY_DOWN = 0x207,
    KEY_INSERT = 0x208,
    KEY_DELETE = 0x209,
    KEY_HOME = 0x20A,
    KEY_END = 0x20B,
    KEY_PAGEUP = 0x20C,
    KEY_PAGEDOWN = 0x20D,
    KEY_CAPSLOCK = 0x20E,
    KEY_NUMLOCK = 0x20F,
    KEY_SCROLLLOCK = 0x210,
    KEY_PAUSE = 0x211,
    KEY_PRINTSCREEN = 0x212,
    KEY_F1 = 0x300,
    KEY_F2 = 0x301,
    KEY_F3 = 0x302,
    KEY_F4 = 0x303,
    KEY_F5 = 0x304,
    KEY_F6 = 0x305,
    KEY_F7 = 0x306,
    KEY_F8 = 0x307,
    KEY_F9 = 0x308,
    KEY_F10 = 0x309,
    KEY_F11 = 0x30A,
    KEY_F12 = 0x30B,
    KEY_F13 = 0x212,
    KEY_F14 = 0x30D,
    KEY_F15 = 0x30E,
    KEY_F16 = 0x30F,
    KEY_F17 = 0x310,
    KEY_F18 = 0x311,
    KEY_F19 = 0x312,
    KEY_LAST = 0x313
};

enum MOUSEBUTTON {
    MOUSE_BUTTON_NONE               = 0x0,
    MOUSE_BUTTON_LEFT               = 0x1,
    MOUSE_BUTTON_MIDDLE             = 0x2,
    MOUSE_BUTTON_RIGHT              = 0x4,
    MOUSE_BUTTON_XBUTTON1           = 0x8,
    MOUSE_BUTTON_XBUTTON2           = 0x10,
    MOUSE_BUTTON_XBUTTON3           = 0x20,
    MOUSE_BUTTON_XBUTTON4           = 0x40,
    MOUSE_BUTTON_XBUTTON5           = 0x80,
    MOUSE_BUTTON_XBUTTON6           = 0x100,
    MOUSE_BUTTON_XBUTTON7           = 0x200,
    MOUSE_BUTTON_XBUTTON8           = 0x400,
    MOUSE_BUTTON_XBUTTON9           = 0x800,
    MOUSE_BUTTON_XBUTTON10          = 0x1000,
    MOUSE_BUTTON_XBUTTON11          = 0x2000,
    MOUSE_BUTTON_XBUTTON12          = 0x4000,
    MOUSE_BUTTON_ALL                = 0xFFFFFFFF
};

enum MOUSEMODE {
    MOUSE_MODE_NORMAL               = 0x0,
    MOUSE_MODE_RELATIVE             = 0x1,
    MOUSE_MODES                     = 0x2
};

enum OSINPUT {
    OS_INPUT_CAPTURE_CHANGED        = 0,
    OS_INPUT_CHAR                   = 1,
    OS_INPUT_STRING                 = 2,
    OS_INPUT_IME                    = 3,
    OS_INPUT_SIZE                   = 4,
    OS_INPUT_CLOSE                  = 5,
    OS_INPUT_FOCUS                  = 6,
    OS_INPUT_KEY_DOWN               = 7,
    OS_INPUT_KEY_UP                 = 8,
    OS_INPUT_MOUSE_DOWN             = 9,
    OS_INPUT_MOUSE_MOVE             = 10,
    OS_INPUT_MOUSE_WHEEL            = 11,
    OS_INPUT_MOUSE_MOVE_RELATIVE    = 12,
    OS_INPUT_MOUSE_UP               = 13,
    OS_INPUT_14                     = 14,
    OS_INPUT_15                     = 15,
    OS_INPUT_16                     = 16,
    OS_INPUT_17                     = 17,
    OS_INPUT_18                     = 18,
    OS_INPUT_SHUTDOWN               = 19
};

struct OSEVENT {
    OSINPUT id;
    int32_t param[4];
};

struct EVENT_DATA_CHAR {
    int32_t ch;
    uint32_t metaKeyState;
    uint32_t repeat;
};

struct EVENT_DATA_IDLE {
    float elapsedSec;
    uint32_t time;
};

struct EVENT_DATA_KEY {
    KEY key;
    uint32_t metaKeyState;
    uint32_t repeat;
    uint32_t time;
};

struct EVENT_DATA_MOUSE {
    MOUSEMODE mode;
    MOUSEBUTTON button;
    uint32_t buttonState;
    uint32_t metaKeyState;
    uint32_t flags;
    float x;
    float y;
    int32_t wheelDistance;
    uint32_t time;
};

struct EVENT_DATA_SIZE {
    int32_t w;
    int32_t h;
};

#endif
