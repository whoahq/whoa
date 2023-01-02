#ifndef GX_GLL_GL_LAYER_VIEW_H
#define GX_GLL_GL_LAYER_VIEW_H

#include <AppKit/AppKit.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Foundation/Foundation.h>

class GLWindow;

@interface GLLayerView : NSView <NSWindowDelegate>

@property CGDirectDisplayID m_display;
@property GLWindow* m_GLWindow;
@property (retain) NSOpenGLContext* m_savedContext;
@property (retain) NSCursor* m_cursor;

- (BOOL)acceptsFirstResponder;
- (void)drawRect:(NSRect)dirtyRect;
- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)window;
- (void)keyDown:(NSEvent*)event;
- (void)keyUp:(NSEvent*)event;
- (void)mouseDown:(NSEvent*)event;
- (void)mouseDragged:(NSEvent*)event;
- (void)mouseMoved:(NSEvent*)event;
- (void)mouseUp:(NSEvent*)event;
- (void)otherMouseDown:(NSEvent*)event;
- (void)otherMouseDragged:(NSEvent*)event;
- (void)otherMouseUp:(NSEvent*)event;
- (void)rightMouseDown:(NSEvent*)event;
- (void)rightMouseDragged:(NSEvent*)event;
- (void)rightMouseUp:(NSEvent*)event;
- (void)scrollWheel:(NSEvent*)event;
- (void)viewDidChangeBackingProperties;
- (void)viewDidEndLiveResize;
- (void)update;

@end

#endif
