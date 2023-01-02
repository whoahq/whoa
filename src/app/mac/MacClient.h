#ifndef APP_MAC_MAC_CLIENT_H
#define APP_MAC_MAC_CLIENT_H

#include "event/Event.hpp"
#include <cstdint>

class MacClient {
    public:
        enum ClipboardAction {
            ClipboardUndo = 1,
            ClipboardCut = 2,
            ClipboardCopy = 3,
            ClipboardPaste = 4,
            ClipboardSelectAll = 5
        };

        static void* s_currentKeyboardLayout;
        static KEY s_specialKeyConversion[128];
        static KEY s_keyConversion[128];

        static void CheckKeyboardLayout();
        static double GetMouseSpeed();
        static bool GetTextInputEnabled();
        static void InitializeKeyConversion();
        static bool IsUsingGLLayer();
        static void PostClipboardKeyEvents(ClipboardAction action);
        static void SetMouseCoalescingEnabled(bool enabled);
};

#endif
