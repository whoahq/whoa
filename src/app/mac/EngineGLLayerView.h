#ifndef APP_MAC_ENGINE_GL_LAYER_VIEW_H
#define APP_MAC_ENGINE_GL_LAYER_VIEW_H

#include "gx/gll/GLLayerView.h"
#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>

@interface EngineGLLayerView : GLLayerView

- (void)insertText:(id)string;

- (void)keyDown:(NSEvent*)event;

@end

#endif
