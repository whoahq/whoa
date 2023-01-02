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

        static void CheckKeyboardLayout(void);
        static double GetMouseSpeed(void);
        static bool GetTextInputEnabled(void);
        static void InitializeKeyConversion(void);
        static bool IsUsingGLLayer(void);
        static void PostClipboardKeyEvents(MacClient::ClipboardAction);
        static void SetMouseCoalescingEnabled(bool);
};

#endif
