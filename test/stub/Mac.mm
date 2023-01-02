#include "gx/gll/GLLayerView.h"
#include "app/mac/MacClient.h"
#include "app/mac/View.h"

void AssignEngineViewCallbacks(GLWindowCallbacks* callbacks) {
}

Class GetEngineViewClass() {
    return nullptr;
}

double MacClient::GetMouseSpeed() {
    return 0.0;
}

void MacClient::SetMouseCoalescingEnabled(bool enabled) {
}

@implementation GLLayerView

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (GLWindowCallbacks*)callbacks {
    return nullptr;
}

- (void)drawRect:(NSRect)dirtyRect {
}

- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)window {
    return self;
}

- (void)setGLWindow:(GLWindow*)window {
}

- (CGDirectDisplayID)findMyDisplay {
    return 0;
}

- (void)keyDown:(NSEvent*)event {
}

- (void)keyUp:(NSEvent*)event {
}

- (void)mouseDown:(NSEvent*)event {
}

- (void)mouseDragged:(NSEvent*)event {
}

- (void)mouseMoved:(NSEvent*)event {
}

- (void)mouseUp:(NSEvent*)event {
}

- (void)otherMouseDown:(NSEvent*)event {
}

- (void)otherMouseDragged:(NSEvent*)event {
}

- (void)otherMouseUp:(NSEvent*)event {
}

- (void)rightMouseDown:(NSEvent*)event {
}

- (void)rightMouseDragged:(NSEvent*)event {
}

- (void)rightMouseUp:(NSEvent*)event {
}

- (void)scrollWheel:(NSEvent*)event {
}

- (void)update {
}

@end
