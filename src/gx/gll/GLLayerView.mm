#include "gx/gll/GLLayerView.h"
#include "gx/gll/GLContext.h"
#include "gx/gll/GLDevice.h"
#include "gx/gll/GLWindow.h"
#include <cmath>

@implementation GLLayerView

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (GLWindowCallbacks*)callbacks {
    if (self.m_GLWindow) {
        return self.m_GLWindow->m_ActiveCallbacks;
    } else {
        // TODO
    }
}

- (void)drawRect:(NSRect)dirtyRect {
    GLContext* context = self.m_GLWindow->m_Context;

    if (context && context->m_Device) {
        context->m_Device->DrawRect();
    }
}

- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)window {
    self = [super initWithFrame:frame];
    [self setWantsBestResolutionOpenGLSurface:YES];

    if (self) {
        [self setGLWindow:window];
    }

    CGSetLocalEventsSuppressionInterval(0.0);

    self.m_display = [self findMyDisplay];

    return self;
}

- (void)setGLWindow:(GLWindow*)window {
    self.m_GLWindow = window;
}

- (CGDirectDisplayID)findMyDisplay {
    NSRect frame = self.frame;

    uint32_t displayCount;
    CGDirectDisplayID displays[1];

    CGGetDisplaysWithPoint(frame.origin, 1, displays, &displayCount);

    if (displayCount) {
        return displays[0];
    } else {
        return CGMainDisplayID();
    }
}

- (void)keyDown:(NSEvent*)event {
    [self callbacks]->OnKeyDown(event);
}

- (void)keyUp:(NSEvent*)event {
    [self callbacks]->OnKeyUp(event);
}

- (void)mouseDown:(NSEvent*)event {
    NSPoint location = event.locationInWindow;
    NSRect frame = self.frame;

    float x = location.x;
    float y = frame.size.height - location.y;

    NSInteger buttonNumber = event.buttonNumber;

    [self callbacks]->OnMouseDown(
        buttonNumber,
        static_cast<int32_t>(floor(x)),
        static_cast<int32_t>(floor(y))
    );
}

- (void)mouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)mouseMoved:(NSEvent*)event {
    float v20 = 0.0;
    float v21 = 0.0;

    // TODO
    // - logic to handle fullscreen mouse movement

    NSPoint location = event.locationInWindow;
    NSRect frame = self.frame;

    float x = location.x + v21;
    float y = (frame.size.height - location.y) + v20;

    [self callbacks]->OnMouseMoved(
        static_cast<int32_t>(floor(x)),
        static_cast<int32_t>(floor(y))
    );
}

- (void)mouseUp:(NSEvent*)event {
    NSPoint location = event.locationInWindow;
    NSRect frame = self.frame;

    float x = location.x;
    float y = frame.size.height - location.y;

    NSInteger buttonNumber = event.buttonNumber;

    [self callbacks]->OnMouseUp(
        buttonNumber,
        static_cast<int32_t>(floor(x)),
        static_cast<int32_t>(floor(y))
    );
}

- (void)otherMouseDown:(NSEvent*)event {
    [self mouseDown:event];
}

- (void)otherMouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent*)event {
    [self mouseUp:event];
}

- (void)rightMouseDown:(NSEvent*)event {
    [self mouseDown:event];
}

- (void)rightMouseDragged:(NSEvent*)event {
    [self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent*)event {
    [self mouseUp:event];
}

- (void)scrollWheel:(NSEvent*)event {
    // TODO
}

- (void)viewDidChangeBackingProperties {
    [self update];
}

- (void)viewDidEndLiveResize {
    int32_t width = std::floor(self.frame.size.width);
    int32_t height = std::floor(self.frame.size.height);

    [self update];

    [self callbacks]->OnResized(
        width,
        height,
        false
    );
}

- (void)update {
    self.m_display = [self findMyDisplay];

    if (self.m_GLWindow->m_Context) {
        self.m_GLWindow->m_Context->Update();
    }
}

@end
