#include "app/mac/MainApp.h"
#include "event/Event.hpp"
#include "event/Input.hpp"
#include "event/Scheduler.hpp"
#include "os/Compat.hpp"

@implementation MainApp

+ (void)initialize {
    [[NSUserDefaults standardUserDefaults]
        registerDefaults: [NSDictionary
            dictionaryWithObject: @"YES"
            forKey: @"NSDisabledCharacterPaletteMenuItem"]];

    [NSApp
        setActivationPolicy: NSApplicationActivationPolicyRegular];
}

- (void)applicationDidFinishLaunching:(id)a1 {
    self.m_pollTimer = [NSTimer
        timerWithTimeInterval: 0.0001
        target: self
        selector: @selector(poll:)
        userInfo: nil
        repeats: true];

    [[NSRunLoop currentRunLoop]
        addTimer: self.m_pollTimer
        forMode: NSDefaultRunLoopMode];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    OsQueuePut(OS_INPUT_CLOSE, 0, 0, 0, 0);

    return NSTerminateCancel;
}

- (void)captureFrame:(id)a1 {
}

- (void)copy:(id)a3 {
    MacClient::PostClipboardKeyEvents(MacClient::ClipboardCopy);
}

- (void)cut:(id)a3 {
    MacClient::PostClipboardKeyEvents(MacClient::ClipboardCut);
}

- (void)paste:(id)a3 {
    MacClient::PostClipboardKeyEvents(MacClient::ClipboardPaste);
}

- (void)poll:(id)a1 {
    if (!Event::s_shouldLoopTerminate) {
        Event::s_shouldLoopTerminate = SchedulerMainProcess();

        if (Event::s_shouldLoopTerminate) {
            [self.m_pollTimer invalidate];
            self.m_pollTimer = nil;

            [NSApp stop:self];

            [NSApp
                postEvent:
                    [NSEvent
                        otherEventWithType: NSEventTypeApplicationDefined
                        location: NSMakePoint(0, 0)
                        modifierFlags: 0
                        timestamp: 0
                        windowNumber: 0
                        context: 0
                        subtype: 0
                        data1: 0
                        data2: 0]
                atStart: 0];
        }
    }
}

- (void)showBatchViewer:(id)a1 {
}

- (void)showGLLayerSetup:(id)a1 {
}

- (void)toggleFullscreenMode:(id)a1 {
    // TODO
}

@end
