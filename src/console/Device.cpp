#include "console/Device.hpp"
#include "client/Gui.hpp"
#include "console/CVar.hpp"
#include "console/Console.hpp"
#include "event/Input.hpp"
#include "gx/Adapter.hpp"
#include "gx/Device.hpp"
#include <storm/Array.hpp>
#include <cstring>

static CGxDevice* s_device;
static CVar* s_cvGxColorBits;
static CVar* s_cvGxDepthBits;
static CVar* s_cvGxMaximize;
static CVar* s_cvGxResolution;
static CVar* s_cvGxWidescreen;
static CVar* s_cvGxWindow;
static DefaultSettings s_defaults;
static TSGrowableArray<CGxMonitorMode> s_gxMonitorModes;
static bool s_hwDetect;
static bool s_hwChanged;
static CGxFormat s_requestedFormat;

bool CVGxColorBitsCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxDepthBitsCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxMaximizeCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxResolutionCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxWindowCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

void RegisterGxCVars() {
    auto& format = s_defaults.format;

    // TODO CURRENT_LANGUAGE check?
    auto v1 = true;

    s_cvGxWidescreen = CVar::Register(
        "widescreen",
        "Allow widescreen support",
        0x0,
        "1",
        nullptr,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    s_cvGxWindow = CVar::Register(
        "gxWindow",
        "toggle fullscreen/window",
        0x1 | 0x2,
        v1 ? "1" : "0",
        &CVGxWindowCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    s_cvGxMaximize = CVar::Register(
        "gxMaximize",
        "maximize game window",
        0x1 | 0x2,
        v1 ? "1" : "0",
        &CVGxMaximizeCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    char colorBits[260];
    SStrPrintf(colorBits, sizeof(colorBits), "%s", CGxFormat::formatToBitsString[format.colorFormat]);
    s_cvGxColorBits = CVar::Register(
        "gxColorBits",
        "color bits",
        0x1 | 0x2,
        colorBits,
        &CVGxColorBitsCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    char depthBits[260];
    SStrPrintf(depthBits, sizeof(depthBits), "%s", CGxFormat::formatToBitsString[format.depthFormat]);
    s_cvGxDepthBits = CVar::Register(
        "gxDepthBits",
        "depth bits",
        0x1 | 0x2,
        depthBits,
        &CVGxDepthBitsCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    char resolution[260];
    SStrPrintf(resolution, 260, "%dx%d", format.size.x, format.size.y);
    s_cvGxResolution = CVar::Register(
        "gxResolution",
        "resolution",
        0x1 | 0x2,
        resolution,
        &CVGxResolutionCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    // TODO s_cvGxRefresh
    // TODO s_cvGxTripleBuffer
    // TODO s_cvGxApi
    // TODO s_cvGxVSync
    // TODO s_cvGxAspect
    // TODO s_cvGxCursor
    // TODO s_cvGxMultisample
    // TODO s_cvGxFixLag
    // TODO s_cvGxStereoEnabled
    // TODO s_cvGxOverride
    // TODO s_cvGxAspect
    // TODO s_cvGxMaxFPS
    // TODO s_cvGxMaxFPSBk
    // TODO s_cvWindowResizeLock
    // TODO s_cvFixedFunction
}

void UpdateGxCVars() {
    // TODO others

    s_cvGxWindow->Update();
    s_cvGxResolution->Update();

    // TODO others

    s_cvGxMaximize->Update();

    // TODO others
}

void SetGxCVars(const CGxFormat& format) {
    char value[1024];

    s_cvGxColorBits->Set(CGxFormat::formatToBitsString[format.colorFormat], true, false, false, true);

    s_cvGxDepthBits->Set(CGxFormat::formatToBitsString[format.depthFormat], true, false, false, true);

    SStrPrintf(value, sizeof(value), "%d", format.window);
    s_cvGxWindow->Set(value, true, false, false, true);

    SStrPrintf(value, sizeof(value), "%dx%d", format.size.x, format.size.y);
    s_cvGxResolution->Set(value, true, false, false, true);

    // TODO s_cvGxRefresh
    // TODO others

    SStrPrintf(value, sizeof(value), "%d", format.maximize);
    s_cvGxMaximize->Set(value, true, false, false, true);

    // TODO others

    UpdateGxCVars();
}

void ConsoleDeviceInitialize(const char* title) {
    // TODO

    // TODO proper logic
    s_hwDetect = true;

    // TODO ConsoleAccessSetEnabled(CmdLineGetBool(35));
    ConsoleAccessSetEnabled(1);

    // TODO

    RegisterGxCVars();

    // TODO ConsoleCommandRegister("gxRestart", &CCGxRestart, 1, nullptr);

    GxAdapterMonitorModes(s_gxMonitorModes);

    // TODO

    // TODO
    // - source the size values correctly
    s_requestedFormat.size.x = 1024;
    s_requestedFormat.size.y = 768;
    s_requestedFormat.colorFormat = CGxFormat::Fmt_Argb8888;
    s_requestedFormat.depthFormat = CGxFormat::Fmt_Ds248;

    if (s_hwDetect || s_hwChanged) {
        // TODO Sub76B3F0(&UnkCABAF0, &UnkCABB38);
        // TODO s_cvFixedFunction->Set("0", 1, 0, 0, 1);
        // TODO memcpy(&s_requestedFormat, &s_defaults.format, sizeof(s_requestedFormat));

        s_requestedFormat.window = s_cvGxWindow->GetInt() != 0;
        s_requestedFormat.maximize = s_cvGxMaximize->GetInt() != 0;

        // TODO temporary override
        s_requestedFormat.maximize = 0;

        SetGxCVars(s_requestedFormat);
    }

    // TODO

    // TODO s_requestedFormat.hwTnL = !CmdLineGetBool(CMD_SW_TNL);
    s_requestedFormat.hwTnL = true;

    // TODO

    CGxFormat format;
    memcpy(&format, &s_requestedFormat, sizeof(s_requestedFormat));

    // TODO proper api selection
    EGxApi api = GxApi_OpenGl;
#if defined(WHOA_SYSTEM_WIN)
    api = GxApi_D3d9;
#elif defined(WHOA_SYSTEM_MAC)
    api = GxApi_GLL;
#endif

    s_device = GxDevCreate(api, OsWindowProc, format);

    // TODO

    auto gxWindow = GxDevWindow();
    OsGuiSetGxWindow(gxWindow);

    // TODO
}

int32_t ConsoleDeviceExists() {
    return s_device != nullptr;
}
