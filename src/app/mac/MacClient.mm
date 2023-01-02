#include "app/mac/MacClient.h"
#include "event/Input.hpp"
#include "os/Compat.hpp"
#include <AppKit/AppKit.h>
#include <Carbon/Carbon.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hidsystem/event_status_driver.h>
#include <IOKit/hidsystem/IOHIDLib.h>
#include <IOKit/hidsystem/IOHIDParameter.h>

void* MacClient::s_currentKeyboardLayout = nullptr;

KEY MacClient::s_specialKeyConversion[128] = {
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_1,                  // kVK_ANSI_1 (0x12)
    KEY_2,                  // kVK_ANSI_2 (0x13)
    KEY_3,                  // kVK_ANSI_3 (0x14)
    KEY_4,                  // kVK_ANSI_4 (0x15)
    KEY_6,                  // kVK_ANSI_6 (0x16)
    KEY_5,                  // kVK_ANSI_5 (0x17)
    KEY_NONE,
    KEY_9,                  // kVK_ANSI_9 (0x19)
    KEY_7,                  // kVK_ANSI_7 (0x1A)
    KEY_NONE,
    KEY_8,                  // kVK_ANSI_8 (0x1C)
    KEY_0,                  // kVK_ANSI_0 (0x1D)
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_ENTER,              // kVK_Return (0x24)
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_TAB,                // kVK_Tab (0x30)
    KEY_SPACE,              // kVK_Space (0x31)
    KEY_NONE,
    KEY_BACKSPACE,          // kVK_Delete (0x33)
    KEY_NONE,
    KEY_ESCAPE,             // kVK_Escape (0x35)
    KEY_NONE,
    KEY_NONE,
    KEY_LSHIFT,             // kVK_Shift (0x38)
    KEY_CAPSLOCK,           // kVK_CapsLock (0x39)
    KEY_LALT,               // kVK_Option (0x3A)
    KEY_LCONTROL,           // kVK_Control (0x3B)
    KEY_RSHIFT,             // kVK_RightShift (0x3C)
    KEY_RALT,               // kVK_RightOption (0x3D)
    KEY_RCONTROL,           // kVK_RightControl (0x3E)
    KEY_NONE,
    KEY_F17,                // kVK_F17 (0x40)
    KEY_NUMPAD_DECIMAL,     // kVK_ANSI_KeypadDecimal (0x41)
    KEY_NONE,
    KEY_NUMPAD_MULTIPLY,    // kVK_ANSI_KeypadMultiply (0x43)
    KEY_NONE,
    KEY_NUMPAD_PLUS,        // kVK_ANSI_KeypadPlus (0x45)
    KEY_NONE,
    KEY_NUMLOCK,            // kVK_ANSI_KeypadClear (0x47)
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_NUMPAD_DIVIDE,      // kVK_ANSI_KeypadDivide (0x4B)
    KEY_ENTER,              // kVK_ANSI_KeypadEnter (0x4C)
    KEY_NONE,
    KEY_NUMPAD_MINUS,       // kVK_ANSI_KeypadMinus (0x4E)
    KEY_F18,                // kVK_F18 (0x4F)
    KEY_F19,                // kVK_F19 (0x50)
    KEY_NUMPAD_EQUALS,      // kVK_ANSI_KeypadEquals (0x51)
    KEY_NUMPAD0,            // kVK_ANSI_Keypad0 (0x52)
    KEY_NUMPAD1,            // kVK_ANSI_Keypad1 (0x53)
    KEY_NUMPAD2,            // kVK_ANSI_Keypad2 (0x54)
    KEY_NUMPAD3,            // kVK_ANSI_Keypad3 (0x55)
    KEY_NUMPAD4,            // kVK_ANSI_Keypad4 (0x56)
    KEY_NUMPAD5,            // kVK_ANSI_Keypad5 (0x57)
    KEY_NUMPAD6,            // kVK_ANSI_Keypad6 (0x58)
    KEY_NUMPAD7,            // kVK_ANSI_Keypad7 (0x59)
    KEY_NONE,
    KEY_NUMPAD8,            // kVK_ANSI_Keypad8 (0x5B)
    KEY_NUMPAD9,            // kVK_ANSI_Keypad9 (0x5C)
    KEY_NONE,
    KEY_NONE,
    KEY_NONE,
    KEY_F5,                 // kVK_F5 (0x60)
    KEY_F6,                 // kVK_F6 (0x61)
    KEY_F7,                 // kVK_F7 (0x62)
    KEY_F3,                 // kVK_F3 (0x63)
    KEY_F8,                 // kVK_F8 (0x64)
    KEY_F9,                 // kVK_F9 (0x65)
    KEY_NONE,
    KEY_F11,                // kVK_F11 (0x67)
    KEY_NONE,
    KEY_F13,                // kVK_F13 (0x69)
    KEY_F16,                // kVK_F16 (0x6A)
    KEY_F14,                // kVK_F14 (0x6B)
    KEY_NONE,
    KEY_F10,                // kVK_F10 (0x6D)
    KEY_NONE,
    KEY_F12,                // kVK_F12 (0x6F)
    KEY_NONE,
    KEY_F15,                // kVK_F15 (0x71)
    KEY_INSERT,             // kVK_Help (0x72)
    KEY_HOME,               // kVK_Home (0x73)
    KEY_PAGEUP,             // kVK_PageUp (0x74)
    KEY_DELETE,             // kVK_ForwardDelete (0x75)
    KEY_F4,                 // kVK_F4 (0x76)
    KEY_END,                // kVK_End (0x77)
    KEY_F2,                 // kVK_F2 (0x78)
    KEY_PAGEDOWN,           // kVK_PageDown (0x79)
    KEY_F1,                 // kVK_F1 (0x7A)
    KEY_LEFT,               // kVK_LeftArrow (0x7B)
    KEY_RIGHT,              // kVK_RightArrow (0x7C)
    KEY_DOWN,               // kVK_DownArrow (0x7D)
    KEY_UP,                 // kVK_UpArrow (0x7E)
    KEY_NONE
};

KEY MacClient::s_keyConversion[128];

void MacClient::CheckKeyboardLayout() {
    #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_6
        void* KCHR = reinterpret_cast<void*>(GetScriptManagerVariable(smKCHRCache));

        if (MacClient::s_currentKeyboardLayout != KCHR) {
            MacClient::InitializeKeyConversion();
            MacClient::s_currentKeyboardLayout = KCHR;
        }
    #endif

    #if WHOA_SYSTEM_VERSION >= WHOA_MACOS_10_6
        TISInputSourceRef inputSrc = TISCopyCurrentKeyboardLayoutInputSource();
        CFDataRef layoutData = static_cast<CFDataRef>(TISGetInputSourceProperty(inputSrc, kTISPropertyUnicodeKeyLayoutData));
        const UCKeyboardLayout* keyboardLayout = reinterpret_cast<const UCKeyboardLayout*>(CFDataGetBytePtr(layoutData));

        if (MacClient::s_currentKeyboardLayout != keyboardLayout) {
            MacClient::InitializeKeyConversion();
            MacClient::s_currentKeyboardLayout = const_cast<UCKeyboardLayout*>(keyboardLayout);
        }
    #endif
}

double MacClient::GetMouseSpeed() {
    #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_12
        double mouseSpeed = 1.0;

        NXEventHandle handle = NXOpenEventStatus();

        if (handle) {
            IOHIDGetAccelerationWithKey(handle, CFSTR(kIOHIDMouseAccelerationType), &mouseSpeed);
            NXCloseEventStatus(handle);
        }

        return mouseSpeed;
    #endif

    #if WHOA_SYSTEM_VERSION >= WHOA_MACOS_10_12
        double mouseSpeed = 1.0;

        io_service_t service = IORegistryEntryFromPath(kIOMasterPortDefault, kIOServicePlane ":/IOResources/IOHIDSystem");

        if (service != MACH_PORT_NULL) {
            CFDictionaryRef parameters = static_cast<CFDictionaryRef>(IORegistryEntryCreateCFProperty(service, CFSTR(kIOHIDParametersKey), kCFAllocatorDefault, kNilOptions));
            CFNumberRef speedParameter = static_cast<CFNumberRef>(CFDictionaryGetValue(parameters, CFSTR(kIOHIDMouseAccelerationType)));

            if (speedParameter) {
                int32_t number;

                if (CFNumberGetValue(static_cast<CFNumberRef>(speedParameter), kCFNumberSInt32Type, &number)) {
                    mouseSpeed = static_cast<double>(number) / 65536.0;
                }
            }

            CFRelease(parameters);
            IOObjectRelease(service);
        }

        return mouseSpeed;
    #endif
}

bool MacClient::GetTextInputEnabled() {
    // TODO
    return false;
}

void MacClient::InitializeKeyConversion() {
    memcpy(MacClient::s_keyConversion, MacClient::s_specialKeyConversion, sizeof(MacClient::s_specialKeyConversion));

    #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_6
        void* KCHR = reinterpret_cast<void*>(GetScriptManagerVariable(smKCHRCache));

        if (KCHR) {
            for (uint16_t i = 0; i < 128; i++) {
                if (MacClient::s_keyConversion[i] == KEY_NONE) {
                    uint16_t translate = i | (1 << 7);
                    uint32_t state = 0;

                    uint32_t value = KeyTranslate(KCHR, translate, &state);

                    if (state) {
                        value = KeyTranslate(KCHR, translate, &state);
                    }

                    if (value) {
                        value = value - 97 <= 25 ? value - 32 : value;

                        auto string = CFStringCreateWithBytes(
                            nullptr,
                            reinterpret_cast<uint8_t*>(&value),
                            1,
                            0,
                            false
                        );

                        if (string) {
                            CFIndex len;
                            CFRange range = CFRangeMake(0, 1);

                            CFStringGetBytes(
                                string,
                                range,
                                kCFStringEncodingISOLatin1,
                                0,
                                0,
                                reinterpret_cast<uint8_t*>(&value),
                                1,
                                &len
                            );

                            if (len && value) {
                                MacClient::s_keyConversion[i] = static_cast<KEY>(value);
                            }
                        }
                    }
                }
            }
        }
    #endif

    #if WHOA_SYSTEM_VERSION >= WHOA_MACOS_10_6
        TISInputSourceRef inputSrc = TISCopyCurrentKeyboardLayoutInputSource();
        CFDataRef layoutData = static_cast<CFDataRef>(TISGetInputSourceProperty(inputSrc, kTISPropertyUnicodeKeyLayoutData));
        const UCKeyboardLayout* keyboardLayout = reinterpret_cast<const UCKeyboardLayout*>(CFDataGetBytePtr(layoutData));
        const uint32_t keyboardType = LMGetKbdType();

        if (keyboardLayout) {
            for (uint16_t i = 0; i < 128; i++) {
                if (MacClient::s_keyConversion[i] == KEY_NONE) {
                    uint16_t vkey = i;
                    uint32_t state = 0;
                    UniChar buf[1];
                    UniCharCount len;

                    OSStatus res = UCKeyTranslate(
                        keyboardLayout,
                        vkey,
                        kUCKeyActionUp,
                        0,
                        keyboardType,
                        0,
                        &state,
                        1,
                        &len,
                        buf
                    );

                    if (res != noErr) {
                        continue;
                    }

                    if (state) {
                        res = UCKeyTranslate(
                            keyboardLayout,
                            vkey,
                            kUCKeyActionUp,
                            0,
                            keyboardType,
                            0,
                            &state,
                            1,
                            &len,
                            buf
                        );

                        if (res != noErr) {
                            continue;
                        }
                    }

                    uint16_t value = buf[0];

                    if (len && value) {
                        value = value >= 97 && value <= 122 ? value - 32 : value;
                        MacClient::s_keyConversion[i] = static_cast<KEY>(value);
                    }
                }
            }
        }
    #endif
}

bool MacClient::IsUsingGLLayer() {
    return true;
}

void MacClient::PostClipboardKeyEvents(MacClient::ClipboardAction action) {
    int32_t v1;

    switch (action) {
        case ClipboardUndo:
            v1 = 90;
            break;

        case ClipboardCut:
            v1 = 88;
            break;

        case ClipboardCopy:
            v1 = 67;
            break;

        case ClipboardPaste:
            v1 = 86;
            break;

        case ClipboardSelectAll:
            v1 = 65;
            break;

        default:
            return;
    }

    OsInputPostEvent(OS_INPUT_KEY_DOWN, 2, 0, 0, 0);
    OsInputPostEvent(OS_INPUT_KEY_DOWN, v1, 0, 0, 0);
    OsInputPostEvent(OS_INPUT_KEY_UP, v1, 0, 0, 0);
    OsInputPostEvent(OS_INPUT_KEY_UP, 2, 0, 0, 0);
}

void MacClient::SetMouseCoalescingEnabled(bool enabled) {
    #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_5
        bool prevEnabled = false;
        SetMouseCoalescingEnabled(true, &prevEnabled);
    #endif

    #if WHOA_SYSTEM_VERSION >= WHOA_MACOS_10_5
        [NSEvent setMouseCoalescingEnabled: enabled];
    #endif
}
