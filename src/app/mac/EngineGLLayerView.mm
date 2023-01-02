#include "app/mac/EngineGLLayerView.h"
#include "app/mac/MacClient.h"
#include "event/Input.hpp"
#include <Carbon/Carbon.h>

@implementation EngineGLLayerView

- (void)insertText:(id)string {
    // TODO
}

- (void)keyDown:(NSEvent*)event {
    uint32_t keyCode = event.keyCode;

    MacClient::CheckKeyboardLayout();

    if (keyCode <= 0x7F) {
        uint32_t key = MacClient::s_keyConversion[keyCode];

        if (key != KEY_NONE) {
            OsQueuePut(OS_INPUT_KEY_DOWN, key, 0, 0, 0);
        }
    }

    if (MacClient::GetTextInputEnabled()) {
        auto events = [NSArray arrayWithObject:event];
        [self interpretKeyEvents:events];
    } else {
        EventRef eventRef = static_cast<EventRef>(const_cast<void*>([event eventRef]));

        uint8_t chr;

        if (GetEventParameter(eventRef, 'kchr', 'TEXT', 0, 1, 0, &chr) == noErr) {
            if (chr > 0x1F && chr <= 0x7E) {
                OsQueuePut(OS_INPUT_CHAR, chr, 1, 0, 0);
            }
        }
    }
}

@end
