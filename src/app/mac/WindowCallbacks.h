#ifndef APP_MAC_WINDOW_CALLBACKS_H
#define APP_MAC_WINDOW_CALLBACKS_H

#include <cstdint>
#include <AppKit/AppKit.h>

void MacOnKeyDown(NSEvent* event);

void MacOnKeyUp(NSEvent* event);

void MacOnMouseDown(int16_t button, int32_t x, int32_t y);

void MacOnMouseMoved(int32_t x, int32_t y);

void MacOnMouseUp(int16_t button, int32_t x, int32_t y);

void MacOnResized(int32_t width, int32_t height, bool a3);

#endif
