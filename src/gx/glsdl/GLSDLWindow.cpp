#include "gx/glsdl/GLSDLWindow.hpp"
#include "event/Types.hpp"
#include "event/Input.hpp"
#include "event/Event.hpp"

#include <bc/Debug.hpp>
#include <storm/Unicode.hpp>
#include <map>

static bool s_GLSDL_Initialized = false;

static const std::map<SDL_Scancode, KEY> s_keyConversion = {
    {SDL_SCANCODE_LSHIFT,       KEY_LSHIFT},
    {SDL_SCANCODE_RSHIFT,       KEY_RSHIFT},
    {SDL_SCANCODE_LCTRL,        KEY_LCONTROL},
    {SDL_SCANCODE_RCTRL,        KEY_RCONTROL},
    {SDL_SCANCODE_LALT,         KEY_LALT},
    {SDL_SCANCODE_RALT,         KEY_RALT},
    {SDL_SCANCODE_SPACE,        KEY_SPACE},
    {SDL_SCANCODE_0,            KEY_0},
    {SDL_SCANCODE_1,            KEY_1},
    {SDL_SCANCODE_2,            KEY_2},
    {SDL_SCANCODE_3,            KEY_3},
    {SDL_SCANCODE_4,            KEY_4},
    {SDL_SCANCODE_5,            KEY_5},
    {SDL_SCANCODE_6,            KEY_6},
    {SDL_SCANCODE_7,            KEY_7},
    {SDL_SCANCODE_8,            KEY_8},
    {SDL_SCANCODE_9,            KEY_9},
    {SDL_SCANCODE_A,            KEY_A},
    {SDL_SCANCODE_B,            KEY_B},
    {SDL_SCANCODE_C,            KEY_C},
    {SDL_SCANCODE_D,            KEY_D},
    {SDL_SCANCODE_E,            KEY_E},
    {SDL_SCANCODE_F,            KEY_F},
    {SDL_SCANCODE_G,            KEY_G},
    {SDL_SCANCODE_H,            KEY_H},
    {SDL_SCANCODE_I,            KEY_I},
    {SDL_SCANCODE_J,            KEY_J},
    {SDL_SCANCODE_K,            KEY_K},
    {SDL_SCANCODE_L,            KEY_L},
    {SDL_SCANCODE_M,            KEY_M},
    {SDL_SCANCODE_N,            KEY_N},
    {SDL_SCANCODE_O,            KEY_O},
    {SDL_SCANCODE_P,            KEY_P},
    {SDL_SCANCODE_Q,            KEY_Q},
    {SDL_SCANCODE_R,            KEY_R},
    {SDL_SCANCODE_S,            KEY_S},
    {SDL_SCANCODE_T,            KEY_T},
    {SDL_SCANCODE_U,            KEY_U},
    {SDL_SCANCODE_V,            KEY_V},
    {SDL_SCANCODE_W,            KEY_W},
    {SDL_SCANCODE_X,            KEY_X},
    {SDL_SCANCODE_Y,            KEY_Y},
    {SDL_SCANCODE_Z,            KEY_Z},
    {SDL_SCANCODE_GRAVE,        KEY_TILDE},
    {SDL_SCANCODE_KP_0,         KEY_NUMPAD0},
    {SDL_SCANCODE_KP_1,         KEY_NUMPAD1},
    {SDL_SCANCODE_KP_2,         KEY_NUMPAD2},
    {SDL_SCANCODE_KP_3,         KEY_NUMPAD3},
    {SDL_SCANCODE_KP_4,         KEY_NUMPAD4},
    {SDL_SCANCODE_KP_5,         KEY_NUMPAD5},
    {SDL_SCANCODE_KP_6,         KEY_NUMPAD6},
    {SDL_SCANCODE_KP_7,         KEY_NUMPAD7},
    {SDL_SCANCODE_KP_8,         KEY_NUMPAD8},
    {SDL_SCANCODE_KP_9,         KEY_NUMPAD9},
    {SDL_SCANCODE_KP_PLUS,      KEY_NUMPAD_PLUS},
    {SDL_SCANCODE_KP_MINUS,     KEY_NUMPAD_MINUS},
    {SDL_SCANCODE_KP_MULTIPLY,  KEY_NUMPAD_MULTIPLY},
    {SDL_SCANCODE_KP_DIVIDE,    KEY_NUMPAD_DIVIDE},
    {SDL_SCANCODE_KP_DECIMAL,   KEY_NUMPAD_DECIMAL},
    {SDL_SCANCODE_KP_EQUALS,    KEY_NUMPAD_EQUALS},
    {SDL_SCANCODE_EQUALS,       KEY_PLUS},
    {SDL_SCANCODE_MINUS,        KEY_MINUS},
    {SDL_SCANCODE_LEFTBRACKET,  KEY_BRACKET_OPEN},
    {SDL_SCANCODE_RIGHTBRACKET, KEY_BRACKET_CLOSE},
    {SDL_SCANCODE_SLASH,        KEY_SLASH},
    {SDL_SCANCODE_BACKSLASH,    KEY_BACKSLASH},
    {SDL_SCANCODE_SEMICOLON,    KEY_SEMICOLON},
    {SDL_SCANCODE_APOSTROPHE,   KEY_APOSTROPHE},
    {SDL_SCANCODE_COMMA,        KEY_COMMA},
    {SDL_SCANCODE_PERIOD,       KEY_PERIOD},
    {SDL_SCANCODE_ESCAPE,       KEY_ESCAPE},
    {SDL_SCANCODE_RETURN,       KEY_ENTER},
    {SDL_SCANCODE_BACKSPACE,    KEY_BACKSPACE},
    {SDL_SCANCODE_TAB,          KEY_TAB},
    {SDL_SCANCODE_LEFT,         KEY_LEFT},
    {SDL_SCANCODE_UP,           KEY_UP},
    {SDL_SCANCODE_RIGHT,        KEY_RIGHT},
    {SDL_SCANCODE_DOWN,         KEY_DOWN},
    {SDL_SCANCODE_INSERT,       KEY_INSERT},
    {SDL_SCANCODE_DELETE,       KEY_DELETE},
    {SDL_SCANCODE_HOME,         KEY_HOME},
    {SDL_SCANCODE_END,          KEY_END},
    {SDL_SCANCODE_PAGEUP,       KEY_PAGEUP},
    {SDL_SCANCODE_PAGEDOWN,     KEY_PAGEDOWN},
    {SDL_SCANCODE_CAPSLOCK,     KEY_CAPSLOCK},
    {SDL_SCANCODE_NUMLOCKCLEAR, KEY_NUMLOCK},
    {SDL_SCANCODE_SCROLLLOCK,   KEY_SCROLLLOCK},
    {SDL_SCANCODE_PAUSE,        KEY_PAUSE},
    {SDL_SCANCODE_PRINTSCREEN,  KEY_PRINTSCREEN},
    {SDL_SCANCODE_F1,           KEY_F1},
    {SDL_SCANCODE_F2,           KEY_F2},
    {SDL_SCANCODE_F3,           KEY_F3},
    {SDL_SCANCODE_F4,           KEY_F4},
    {SDL_SCANCODE_F5,           KEY_F5},
    {SDL_SCANCODE_F6,           KEY_F6},
    {SDL_SCANCODE_F7,           KEY_F7},
    {SDL_SCANCODE_F8,           KEY_F8},
    {SDL_SCANCODE_F9,           KEY_F9},
    {SDL_SCANCODE_F10,          KEY_F10},
    {SDL_SCANCODE_F11,          KEY_F11},
    {SDL_SCANCODE_F12,          KEY_F12},
    {SDL_SCANCODE_F13,          KEY_F13},
    {SDL_SCANCODE_F14,          KEY_F14},
    {SDL_SCANCODE_F15,          KEY_F15},
    {SDL_SCANCODE_F16,          KEY_F16},
    {SDL_SCANCODE_F17,          KEY_F17},
    {SDL_SCANCODE_F18,          KEY_F18},
    {SDL_SCANCODE_F19,          KEY_F19}
};

static MOUSEBUTTON s_buttonConversion[16] = {
    MOUSE_BUTTON_NONE,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_XBUTTON1,
    MOUSE_BUTTON_XBUTTON2,
    MOUSE_BUTTON_XBUTTON3,
    MOUSE_BUTTON_XBUTTON4,
    MOUSE_BUTTON_XBUTTON5,
    MOUSE_BUTTON_XBUTTON6,
    MOUSE_BUTTON_XBUTTON7,
    MOUSE_BUTTON_XBUTTON8,
    MOUSE_BUTTON_XBUTTON9,
    MOUSE_BUTTON_XBUTTON10,
    MOUSE_BUTTON_XBUTTON11,
    MOUSE_BUTTON_XBUTTON12
};

void GLSDLWindow::Create(const char* title, const GLSDLWindowRect& rect, GLTextureFormat depthFormat, uint32_t sampleCount) {
    BLIZZARD_ASSERT(this->m_sdlWindow == nullptr);

    if (!s_GLSDL_Initialized) {
        // Initialize SDL video context
        SDL_Init(SDL_INIT_VIDEO);

        // Set GL version profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        s_GLSDL_Initialized = true;
    }

    // Set depth and stencil size according to format
    uint32_t depthSize = 0;
    uint32_t stencilSize = 0;

    switch (depthFormat) {
    case GLTF_INVALID:
        break;
    case GLTF_D32:
        depthSize = 32;
        break;
    case GLTF_D24:
        depthSize = 24;
        break;
    case GLTF_D16:
        depthSize = 16;
        break;
    case GLTF_D24S8:
        depthSize = 24;
        stencilSize = 8;
        break;
    default:
        BLIZZARD_ASSERT(false);
        break;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthSize);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilSize);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Set multisampling
    if (sampleCount >= 1) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sampleCount);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    }

    this->m_sdlWindow = SDL_CreateWindow(
        title,
        static_cast<int>(rect.size.width), static_cast<int>(rect.size.height),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    BLIZZARD_ASSERT(this->m_sdlWindow != nullptr);

    SDL_StartTextInput();
}

void GLSDLWindow::Swap() {
    SDL_GL_SwapWindow(this->m_sdlWindow);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        this->DispatchSDLEvent(event);
    }
}

void GLSDLWindow::Destroy() {
    SDL_DestroyWindow(this->m_sdlWindow);
    this->m_sdlWindow = nullptr;
}

GLSDLWindowRect GLSDLWindow::GetRect() {
    // Default rectangle
    GLSDLWindowRect rect;

    int origin_x = 0;
    int origin_y = 0;
    if (SDL_GetWindowPosition(this->m_sdlWindow, &origin_x, &origin_y) == 0) {
        rect.origin.x = static_cast<int32_t>(origin_x);
        rect.origin.y = static_cast<int32_t>(origin_y);
    }

    int width  = 0;
    int height = 0;
    if (SDL_GetWindowSize(this->m_sdlWindow, &width, &height) == 0) {
        rect.size.width  = static_cast<int32_t>(width);
        rect.size.height = static_cast<int32_t>(height);
    }

    return rect;
}

GLSDLWindowRect GLSDLWindow::GetBackingRect() {
    // Default rectangle
    GLSDLWindowRect rect;

    // Query backing width/height
    int width  = 0;
    int height = 0;
    if (SDL_GetWindowSizeInPixels(this->m_sdlWindow, &width, &height) == 0) {
        rect.size.width  = static_cast<int32_t>(width);
        rect.size.height = static_cast<int32_t>(height);
    }

    return rect;
}

void GLSDLWindow::Resize(const GLSDLWindowRect& rect) {
    auto current = this->GetBackingRect();

    if (current.size.width != rect.size.width || current.size.height != rect.size.width) {
        auto status = SDL_SetWindowSize(this->m_sdlWindow, rect.size.width, rect.size.height);
        BLIZZARD_ASSERT(status == 0);
    }
}

int32_t GLSDLWindow::GetWidth() {
    return this->GetBackingRect().size.width;
}

int32_t GLSDLWindow::GetHeight() {
    return this->GetBackingRect().size.height;
}

void GLSDLWindow::DispatchSDLEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        this->DispatchSDLKeyboardEvent(event);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        this->DispatchSDLMouseButtonEvent(event);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        this->DispatchSDLMouseMotionEvent(event);
        break;
    case SDL_EVENT_TEXT_INPUT:
        this->DispatchSDLTextInputEvent(event);
        break;
    case SDL_EVENT_QUIT:
        EventPostClose();
        break;
    default:
        break;
    }
}

void GLSDLWindow::DispatchSDLKeyboardEvent(const SDL_Event& event) {
    // Is this an up or down keypress?
    OSINPUT inputclass = event.type == SDL_EVENT_KEY_UP ? OS_INPUT_KEY_UP : OS_INPUT_KEY_DOWN;

    // What key does this SDL scancode correspond to?
    auto lookup = s_keyConversion.find(event.key.keysym.scancode);
    if (lookup != s_keyConversion.end()) {
        // Scancode was found
        KEY key = lookup->second;

        // Push key event into input queue
        OsQueuePut(inputclass, key, 0, 0, 0);
        return;
    }

}

void GLSDLWindow::DispatchSDLMouseMotionEvent(const SDL_Event& event) {
    auto x = static_cast<int32_t>(event.motion.x);
    auto y = static_cast<int32_t>(event.motion.y);

    OsQueuePut(OS_INPUT_MOUSE_MOVE, 0, x, y, 0);
}

void GLSDLWindow::DispatchSDLMouseButtonEvent(const SDL_Event& event) {
    // Is this an up or down mouse click?
    OSINPUT inputclass = event.type == SDL_EVENT_MOUSE_BUTTON_UP ? OS_INPUT_MOUSE_UP : OS_INPUT_MOUSE_DOWN;

    // XY click coordinates
    auto x = static_cast<int32_t>(event.button.x);
    auto y = static_cast<int32_t>(event.button.y);

    // Convert SDL button index into internal MOUSEBUTTON ID
    auto buttonIndex = event.button.button;
    if (buttonIndex > 15) {
        return;
    }
    auto button = s_buttonConversion[buttonIndex];

    // Push mousebutton event into input queue
    OsQueuePut(inputclass, button, x, y, 0);
}

void GLSDLWindow::DispatchSDLTextInputEvent(const SDL_Event& event) {
    // text input string holding one or more UTF-8 characters
    auto text = reinterpret_cast<const uint8_t*>(event.text.text);

    // Because SDL_TextInputEvent can hold multiple UTF-8 characters
    // explode variable number of these characters into
    // individual OS_INPUT_CHAR events
    while (*text != '\0') {
        // byte size of current UTF-8 character
        uint32_t charactersize = 0;

        // Read UTF-8 character
        auto character = static_cast<int32_t>(SUniSGetUTF8(text, &charactersize));
        if (character < 0) {
            // Cancel in case of invalid input
            break;
        }

        // Push character to input queue
        OsQueuePut(OS_INPUT_CHAR, character, 1, 0, 0);

        // Advance text pointer
        text += charactersize;
    }
}
