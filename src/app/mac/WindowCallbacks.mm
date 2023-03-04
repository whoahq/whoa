#include "app/mac/WindowCallbacks.h"
#include "app/mac/MacClient.h"
#include "event/Input.hpp"
#include "gx/gll/CGxDeviceGLL.hpp"
#include "gx/Device.hpp"
#include "gx/Window.hpp"
#include <bc/Debug.hpp>

void MacOnKeyDown(NSEvent* event) {
    BLIZZARD_ASSERT(false);
}

void MacOnKeyUp(NSEvent* event) {
    uint32_t keyCode = event.keyCode;

    MacClient::CheckKeyboardLayout();

    if (keyCode <= 0x7F) {
        uint32_t key = MacClient::s_keyConversion[keyCode];

        if (key != KEY_NONE) {
            OsQueuePut(OS_INPUT_KEY_UP, key, 0, 0, 0);
        }
    }
}

void MacOnMouseDown(int16_t button, int32_t x, int32_t y) {
    NSEvent* event = [NSApp currentEvent];
    uint32_t modifierFlags = event.modifierFlags;

    uint32_t v8 = 0;

    if (modifierFlags & NSShiftKeyMask) {
        v8 |= 0x2;
    }

    if (modifierFlags & NSAlternateKeyMask) {
        v8 |= 0x8;
    }

    if (modifierFlags & NSCommandKeyMask) {
        v8 |= 0x1;
    }

    if (modifierFlags & NSControlKeyMask) {
        v8 |= 0x10;
    }

    if (modifierFlags & NSAlphaShiftKeyMask) {
        v8 |= 0x4;
    }

    int16_t buttonNumber = button + 1;
    int16_t buttonIndex = button;

    if (buttonNumber > 0xF) {
        return;
    }

    if (Input::s_buttonDown[buttonIndex]) {
        return;
    }

    int16_t effectiveButtonNumber = buttonNumber;

    Input::s_buttonDown[buttonIndex] = 1;

    if (buttonNumber == 1) {
        effectiveButtonNumber = 1;

        if (v8 & 0x1) {
            Input::s_simulatedRightButtonClick = 1;

            // TODO
            // if (Input::byte_12B94E2) {
            //     return;
            // }

            effectiveButtonNumber = 2;
        }
    }

    if (buttonNumber == 2) {
        if (Input::s_simulatedRightButtonClick) {
            return;
        } else {
            effectiveButtonNumber = 2;
        }
    }

    MOUSEBUTTON mouseButton = ConvertButtonNumberToMOUSEBUTTON(effectiveButtonNumber);

    if (mouseButton) {
        if (Input::s_mouseMode == 0) {
            Input::s_currentMouse = { x, y };
        }

        OsQueuePut(OS_INPUT_MOUSE_DOWN, mouseButton, x, y, 0);
    }
}

void MacOnMouseMoved(int32_t x, int32_t y) {
    if (Input::s_mouseMode == 1) {
        NSEvent* event = [NSApp currentEvent];

        int32_t deltaX = static_cast<int32_t>(floor(event.deltaX));
        int32_t deltaY = static_cast<int32_t>(floor(event.deltaY));

        OsQueuePut(OS_INPUT_MOUSE_MOVE_RELATIVE, 0, deltaX, deltaY, 0);
    } else {
        Input::s_currentMouse = { x, y };

        OsQueuePut(OS_INPUT_MOUSE_MOVE, 0, x, y, 0);
    }
}

void MacOnMouseUp(int16_t button, int32_t x, int32_t y) {
    NSEvent* event = [NSApp currentEvent];
    uint32_t modifierFlags = event.modifierFlags;

    int16_t buttonNumber = button + 1;
    int16_t buttonIndex = button;

    if (buttonNumber > 0xF) {
        return;
    }

    if (!Input::s_buttonDown[buttonIndex]) {
        return;
    }

    int16_t effectiveButtonNumber = buttonNumber;

    Input::s_buttonDown[buttonIndex] = 0;

    if (buttonNumber == 1) {
        effectiveButtonNumber = 1;

        if (Input::s_simulatedRightButtonClick) {
            effectiveButtonNumber = 2;
            Input::s_simulatedRightButtonClick = 0;

            // TODO
            // if (Input::byte_12B94E2) {
            //     return;
            // }
        }
    }

    if (buttonNumber == 2) {
        if (Input::s_simulatedRightButtonClick) {
            return;
        } else {
            effectiveButtonNumber = 2;
        }
    }

    MOUSEBUTTON mouseButton = ConvertButtonNumberToMOUSEBUTTON(effectiveButtonNumber);

    if (mouseButton) {
        if (Input::s_mouseMode != 0) {
            Input::s_currentMouse = { x, y };
        }

        OsQueuePut(OS_INPUT_MOUSE_UP, mouseButton, x, y, 0);
    }
}

void MacOnResized(int32_t width, int32_t height, bool a3) {
    if (a3) {
        return;
    }

    static_cast<CGxDeviceGLL*>(g_theGxDevicePtr)->Resize(width, height);

    OsQueuePut(OS_INPUT_SIZE, width, height, 0, 0);

    // TODO SubA61E60(width, height)

    auto bounds = GetSavedWindowBounds();
    Rect newBounds = {
        bounds->top,
        bounds->left,
        static_cast<int16_t>(bounds->top + height),
        static_cast<int16_t>(bounds->left + width)
    };
    SetSavedWindowBounds(newBounds);
}
