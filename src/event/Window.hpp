#ifndef EVENT_WINDOW_HPP
#define EVENT_WINDOW_HPP

#include <cstdint>

int32_t OsGuiProcessMessage(void* message);

int32_t OsWindowProc(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam);

#endif
