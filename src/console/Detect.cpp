#include "console/Detect.hpp"
#include "gx/Adapter.hpp"

void AddResolution(TSGrowableArray<C2iVector>& resolutions, const C2iVector& resolution) {
    resolutions.Add(1, &resolution);
}

void ConsoleDetectGetResolutions(TSGrowableArray<C2iVector>& resolutions, int32_t widescreen) {
    // Widescreen resolutions

    if (widescreen) {
        TSGrowableArray<CGxMonitorMode> monitorModes;
        GxAdapterMonitorModes(monitorModes);

        C2iVector previousResolution = { 0, 0 };

        for (uint32_t i = 0; i < monitorModes.Count(); i++) {
            auto& monitorMode = monitorModes[i];
            auto& resolution = monitorMode.size;

            // "Widescreen" aspect ratio
            if (static_cast<float>(resolution.x) / static_cast<float>(resolution.y) < 1.248f) {
                continue;
            }

            // At least 640x480
            if (resolution.x < 640 || resolution.y < 480) {
                continue;
            }

            // Not already present
            if (resolution.x == previousResolution.x && resolution.y == previousResolution.y) {
                continue;
            }

            AddResolution(resolutions, resolution);
            previousResolution = resolution;
        }
    }

    // Fallback resolutions

    if (!widescreen || resolutions.Count() == 0) {
        AddResolution(resolutions, {  640,  480 });
        AddResolution(resolutions, {  800,  600 });
        AddResolution(resolutions, { 1024,  768 });
        AddResolution(resolutions, { 1152,  864 });
        AddResolution(resolutions, { 1280,  960 });
        AddResolution(resolutions, { 1280, 1024 });
        AddResolution(resolutions, { 1600, 1200 });
    }
}
