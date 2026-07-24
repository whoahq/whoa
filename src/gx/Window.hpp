#ifndef GX_WINDOW_HPP
#define GX_WINDOW_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC)
    #include <ApplicationServices/ApplicationServices.h>
#endif

#if defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WIN) || defined(WHOA_SYSTEM_WEB)
struct Rect {
    int16_t top;
    int16_t left;
    int16_t bottom;
    int16_t right;
};
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
typedef struct tagRECT {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} RECT;
#endif

int32_t OsGetDefaultWindowRect(RECT* rect);

Rect* GetSavedWindowBounds();

Rect* GetSavedZoomedWindowBounds();

void SetSavedWindowBounds(Rect rect);

void SetSavedZoomedWindowBounds(Rect rect);

#endif
