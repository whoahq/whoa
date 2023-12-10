#include "console/Device.hpp"
#include "client/CmdLine.hpp"
#include "client/Gui.hpp"
#include "console/Console.hpp"
#include "console/CVar.hpp"
#include "event/Input.hpp"
#include "gx/Device.hpp"
#include <cstring>

CVar* s_cvGxMaximize;
CVar* s_cvGxResolution;
CVar* s_cvGxWidescreen;
CVar* s_cvGxWindow;
CVar* s_cvGxApi;
DefaultSettings s_defaults;
bool s_hwDetect;
bool s_hwChanged;
CGxFormat s_requestedFormat;

const char* g_gxApiNames[GxApis_Last] = {
    "OpenGL", // GxApi_OpenGl
    "D3D9",   // GxApi_D3d9
    "D3D9Ex", // GxApi_D3d9Ex
    "D3D10",  // GxApi_D3d10
    "D3D11",  // GxApi_D3d11
    "GLL",    // GxApi_GLL
    "GLSDL"   // GxApi_GLSDL
};

EGxApi g_gxApiSupported[] = {
#if defined(WHOA_SYSTEM_WIN)
    GxApi_D3d9,
    GxApi_GLSDL
#endif

#if defined(WHOA_SYSTEM_MAC)
    GxApi_GLL
#endif

#if defined(WHOA_SYSTEM_LINUX)
    GxApi_GLSDL
#endif
};

size_t g_numGxApiSupported = sizeof(g_gxApiSupported) / sizeof(EGxApi);

bool CVGxMaximizeCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxResolutionCallback(CVar*, const char*, const char*, void*) {
    // static C2iVector legalSizes[] = {
    //     {}
    // }

    // TODO
    return true;
}

bool CVGxWindowCallback(CVar*, const char*, const char*, void*) {
    // TODO
    return true;
}

bool CVGxApiCallback(CVar* h, const char* oldValue, const char* newValue, void* arg) {
    for (size_t api = 0; api < g_numGxApiSupported; api++) {
        auto apiString = g_gxApiNames[g_gxApiSupported[api]];
        if (SStrCmpI(newValue, apiString, STORM_MAX_STR) == 0) {
            // New gxApi is supported, pass
            ConsoleWrite("GxApi set pending gxRestart", DEFAULT_COLOR);
            return true;
        }
    }

    // Supported gxApi not supplied, show available apis
    constexpr size_t msgsize = 1024;
    char msg[msgsize] = {0};
    SStrPack(msg, "unsupported api, must be one of ", msgsize);

    for (size_t api = 0; api < g_numGxApiSupported; api++) {
        if (api > 0) {
            SStrPack(msg, ", ", msgsize);
        }
        auto apiString = g_gxApiNames[g_gxApiSupported[api]];
        SStrPack(msg, "'", msgsize);
        SStrPack(msg, apiString, msgsize);
        SStrPack(msg, "'", msgsize);
    }

    ConsoleWrite(msg, DEFAULT_COLOR);
    return false;
}

EGxApi GxApiDefault() {
#if defined(WHOA_SYSTEM_WIN)
    return GxApi_D3d9;
#endif

#if defined(WHOA_SYSTEM_MAC)
    return GxApi_GLL;
#endif

#if defined(WHOA_SYSTEM_LINUX)
    return GxApi_GLSDL;
#endif
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
        1,
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
        1,
        0,
        0,
        0
    );

    s_cvGxMaximize = CVar::Register(
        "gxMaximize",
        "maximize game window",
        0x1 | 0x2,
        v1 ? "1" : "0",
        &CVGxMaximizeCallback,
        1,
        0,
        0,
        0
    );

    // TODO s_cvGxColorBits
    // TODO s_cvGxDepthBits

    char resolution[260];
    SStrPrintf(resolution, 260, "%dx%d", format.size.x, format.size.y);
    s_cvGxResolution = CVar::Register(
        "gxResolution",
        "resolution",
        0x1 | 0x2,
        resolution,
        &CVGxResolutionCallback,
        1,
        false,
        nullptr,
        false
    );

    // TODO s_cvGxRefresh
    // TODO s_cvGxTripleBuffer
    // TODO s_cvGxApi

    s_cvGxApi = CVar::Register(
        "gxApi",
        "graphics api",
        0x1 | 0x2,
        g_gxApiNames[GxApiDefault()],
        CVGxApiCallback,
        1,
        false,
        nullptr,
        false
    );

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

    // TODO s_cvGxColorBits
    // TODO s_cvGxDepthBits

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

    ConsoleAccessSetEnabled(CmdLineGetBool(WOWCMD_CONSOLE));

    // TODO

    RegisterGxCVars();

    // TODO ConsoleCommandRegister("gxRestart", &CCGxRestart, 1, nullptr);

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

    // Select gxApi based on user CVars and command-line parameters
    EGxApi api = GxApiDefault();

    auto gxApiName = s_cvGxApi->GetString();

    auto gxOverride = CmdLineGetString(WOWCMD_GX_OVERRIDE);
    if (*gxOverride != '\0') {
        gxApiName = gxOverride;
    }

    // Sanitize chosen gxApi against list of supported gxApis
    for (size_t i = 0; i < g_numGxApiSupported; i++) {
        EGxApi supportedApi = g_gxApiSupported[i];
        if (SStrCmp(gxApiName, g_gxApiNames[supported], STORM_MAX_STR) == 0) {
            api = supportedApi;
            break;
        }
    }

    CGxDevice* device = GxDevCreate(api, OsWindowProc, format);

    // TODO

    auto gxWindow = GxDevWindow();
    OsGuiSetGxWindow(gxWindow);

    // TODO
}
