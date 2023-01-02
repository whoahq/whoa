#include "app/mac/WoWApplication.h"
#include "gx/gll/GLLayerView.h"

@implementation WoWApplication

- (void)sendEvent:(NSEvent*)event {
    auto responder = [[NSApp keyWindow] firstResponder];

    if (responder && [responder isKindOfClass: [GLLayerView class]]) {
        NSEventType type = [event type];

        if (type == NSKeyDown && (event.keyCode == 114 || (event.keyCode == 48 && event.modifierFlags & NSControlKeyMask))) {
            [responder keyDown: event];
            return;
        } else if (type == NSKeyUp) {
            [responder keyUp: event];
            return;
        }
    }

    [super sendEvent:event];
}

@end
