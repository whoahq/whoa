#include "console/Detect.hpp"
#include "gx/Adapter.hpp"

void AddResolution(TSGrowableArray<C2iVector>& resolutions, const C2iVector& resolution) {
    resolutions.Add(1, &resolution);
}

// TODO sometimes 640x480 is included, sometimes its ignored -- more than one function?
void ConsoleDetectGetResolutions(TSGrowableArray<C2iVector>& resolutions, int32_t widescreen) {
    // "Widescreen" resolutions
    //
    // This branch is just dynamically enumerating resolutions above a minimum size and aspect
    // ratio as opposed to using a fixed list of common 1.33:1 resolutions. The name widescreen
    // comes from the CVar that feeds the argument (see: SetupResolutions).

    if (widescreen) {
        TSGrowableArray<CGxMonitorMode> monitorModes;
        GxAdapterMonitorModes(monitorModes);

        C2iVector previousResolution = { 0, 0 };

        for (uint32_t i = 0; i < monitorModes.Count(); i++) {
            auto& monitorMode = monitorModes[i];
            auto& resolution = monitorMode.size;

            // Minimum aspect ratio
            if (static_cast<float>(resolution.x) / static_cast<float>(resolution.y) < 1.248f) {
                continue;
            }

            // Minimum resolution
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
