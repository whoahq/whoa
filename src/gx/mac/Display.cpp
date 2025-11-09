#include "gx/CGxMonitorMode.hpp"
#include <ApplicationServices/ApplicationServices.h>

void ConvertDisplayMode(CGxMonitorMode& monitorMode, CFDictionaryRef displayMode) {
    monitorMode.size.x = 0;
    monitorMode.size.y = 0;
    monitorMode.bpp = 0;
    monitorMode.refreshRate = 0;

    int32_t width = 0;
    auto widthValue = static_cast<CFNumberRef>(CFDictionaryGetValue(displayMode, kCGDisplayWidth));
    CFNumberGetValue(widthValue, kCFNumberIntType, &width);

    int32_t height = 0;
    auto heightValue = static_cast<CFNumberRef>(CFDictionaryGetValue(displayMode, kCGDisplayHeight));
    CFNumberGetValue(heightValue, kCFNumberIntType, &height);

    uint32_t refreshRate = 0;
    auto refreshRateValue = static_cast<CFNumberRef>(CFDictionaryGetValue(displayMode, kCGDisplayRefreshRate));
    CFNumberGetValue(refreshRateValue, kCFNumberIntType, &refreshRate);

    uint32_t bitsPerPixel = 0;
    auto bppValue = static_cast<CFNumberRef>(CFDictionaryGetValue(displayMode, kCGDisplayBitsPerPixel));
    CFNumberGetValue(bppValue, kCFNumberIntType, &bitsPerPixel);

    monitorMode.size.x = width;
    monitorMode.size.y = height;
    monitorMode.bpp = bitsPerPixel;
    monitorMode.refreshRate = refreshRate ? refreshRate : 60;
}
