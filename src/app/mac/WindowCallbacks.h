#ifndef APP_MAC_WINDOW_CALLBACKS_H
#define APP_MAC_WINDOW_CALLBACKS_H

#include <cstdint>
#include <AppKit/AppKit.h>

void MacOnKeyDown(NSEvent*);

void MacOnKeyUp(NSEvent*);

void MacOnMouseDown(int16_t, int32_t, int32_t);

void MacOnMouseMoved(int32_t, int32_t);

void MacOnMouseUp(int16_t, int32_t, int32_t);

void MacOnResized(int32_t width, int32_t height, bool a3);

#endif
