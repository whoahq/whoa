#include "event/mac/Event.h"
#include "event/Event.hpp"
#include <AppKit/AppKit.h>

void RunCocoaEventLoop() {
    if (!Event::s_shouldLoopTerminate) {
        [NSApp run];
    }
}
