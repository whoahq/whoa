#include "client/Client.hpp"
#include "async/AsyncFile.hpp"
#include "client/ClientHandlers.hpp"
#include "client/ClientServices.hpp"
#include "component/CCharacterComponent.hpp"
#include "console/CVar.hpp"
#include "console/Device.hpp"
#include "console/Initialize.hpp"
#include "console/Screen.hpp"
#include "db/Db.hpp"
#include "glue/CGlueMgr.hpp"
#include "glue/GlueScript.hpp"
#include "gx/Screen.hpp"
#include "gx/Texture.hpp"
#include "model/Model2.hpp"
#include "net/Poll.hpp"
#include "sound/Interface.hpp"
#include "ui/FrameScript.hpp"
#include "ui/FrameXML.hpp"
#include "world/World.hpp"
#include <bc/Debug.hpp>
#include <common/Prop.hpp>
#include <storm/Error.hpp>

CVar* Client::g_accountNameVar;
CVar* Client::g_accountListVar;
HEVENTCONTEXT Client::g_clientEventContext;

static CVar* s_desktopGammaCvar;
static CVar* s_gammaCvar;
static CVar* s_textureCacheSizeCvar;
static CVar* s_textureFilteringModeCvar;
static CVar* s_uiFasterCvar;

bool DesktopGammaCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool GammaCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool TextureCacheSizeCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool TextureFilteringCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool UIFasterCalllback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

void AsyncFileInitialize() {
    // TODO
    AsyncFileReadInitialize(0, 100);
}

void BaseInitializeGlobal() {
    PropInitialize();
}

int32_t ClientIdle(const void* data, void* param) {
    // TODO
    // ClientGameTimeTickHandler(data, param);
    // Player_C_ZoneUpdateHandler(data, param);

    return 1;
}

void ClientInitializeGame(uint32_t mapId, C3Vector position) {
    // TODO

    EventRegister(EVENT_ID_IDLE, ClientIdle);

    // TODO

    ClientServices::SetMessageHandler(SMSG_NOTIFICATION, NotifyHandler, nullptr);
    ClientServices::SetMessageHandler(SMSG_PLAYED_TIME, PlayedTimeHandler, nullptr);
    ClientServices::SetMessageHandler(SMSG_NEW_WORLD, NewWorldHandler, nullptr);
    ClientServices::SetMessageHandler(SMSG_TRANSFER_PENDING, TransferPendingHandler, nullptr);
    ClientServices::SetMessageHandler(SMSG_TRANSFER_ABORTED, TransferAbortedHandler, nullptr);
    ClientServices::SetMessageHandler(SMSG_LOGIN_VERIFY_WORLD, LoginVerifyWorldHandler, nullptr);

    ClientServices::SetMessageHandler(SMSG_KICK_REASON, CGlueMgr::OnKickReasonMsg, nullptr);

    // TODO
}

void ClientRegisterConsoleCommands() {
    // TODO

    Client::g_accountNameVar = CVar::Register(
        "accountName",
        "Saved account name",
        0x40,
        "",
        nullptr,
        GAME,
        false,
        nullptr,
        false
    );

    Client::g_accountListVar = CVar::Register(
        "accountList",
        "List of wow accounts for saved Blizzard account",
        0,
        "",
        nullptr,
        GAME,
        false,
        nullptr,
        false
    );

    // TODO

    s_desktopGammaCvar = CVar::Register(
        "DesktopGamma",
        nullptr,
        0x0,
        "0",
        &DesktopGammaCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    s_gammaCvar = CVar::Register(
        "Gamma",
        nullptr,
        0x0,
        "1.0",
        &GammaCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    // TODO

    auto showToolsVar = CVar::Register(
        "showToolsUI",
        "Display the launcher when starting the game",
        0x0,
        "-1",
        nullptr,
        GAME,
        false,
        nullptr,
        false
    );

    if (showToolsVar->GetInt() >= 2) {
        showToolsVar->Set("1", true, false, false, true);
    }

    // TODO
}

void ClientPostClose(int32_t a1) {
    // TODO s_finalDialog = a1;
    EventPostCloseEx(nullptr);
}

int32_t DestroyEngineCallback(const void* a1, void* a2) {
    // TODO

    return 1;
}

int32_t InitializeEngineCallback(const void* a1, void* a2) {
    // TODO
    // sub_4D2A30();

    AsyncFileInitialize();
    TextureInitialize();

    // ModelBlobLoad("world\\model.blob");

    // if (SFile::IsStreamingMode()) {
    //     TextureLoadBlob("world\\liquid.tex");
    // }

    ScrnInitialize(0);
    ConsoleScreenInitialize(nullptr); // TODO argument

    s_textureFilteringModeCvar = CVar::Register(
        "textureFilteringMode",
        "Texture filtering mode",
        0x1,
        "1",
        &TextureFilteringCallback,
        GRAPHICS
    );

    s_uiFasterCvar = CVar::Register(
        "UIFaster",
        "UI acceleration option",
        0x0,
        "3",
        &UIFasterCalllback,
        GRAPHICS
    );

    s_textureCacheSizeCvar = CVar::Register(
        "textureCacheSize",
        "Texture cache size in bytes",
        0x1,
        "32",
        &TextureCacheSizeCallback,
        GRAPHICS
    );

    // sub_4B6580(*(_DWORD *)(dword_B2F9FC + 48) << 20);

    // AddConsoleDeviceDefaultCallback(SetDefaults);

    // if (ConsoleDeviceHardwareChanged()) {
    //     v3 = 0;

    //     do {
    //         SetDefaults(v3++);
    //     } while (v3 < 3);
    // }

    auto m2Flags = M2RegisterCVars();
    M2Initialize(m2Flags, 0);

    // v4 = *(_DWORD *)(dword_B2FA00 + 48);
    // sub_4B61C0(dword_AB6128[v4]);
    // sub_4B6230(dword_AB6140[v4]);

    WowClientInit();

    return 1;
}

// TODO name this (maybe something like InitializeLocale?)
void Sub405DD0() {
    // TODO

    // TODO get this from the soupy mess of locale checks above
    auto locale = "enUS";

    ClientServices::InitLoginServerCVars(1, locale);

    // TODO
}

bool LocaleChangedCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO

    return true;
}

int32_t InitializeGlobal() {
    // TODO

    // SCmdRegisterArgList(&ProcessCommandLine(void)::s_wowArgList, 17u);

    // CmdLineProcess();

    // sub_403600("WoW.mfil");

    // if (dword_B2FA10 != 2) {
    //     sub_403560();
    // }

    // LOBYTE(v24) = 0;

    // if (sub_422140()) {
    //     LOBYTE(v24) = OsDirectoryExists((int)"WTF/Account") == 0;
    // }

    // ClientServices::LoadCDKey();

    ConsoleInitializeClientCommand();

    ConsoleInitializeClientCVar("Config.wtf");

    // sub_7663F0();

    // v18 = 0;
    // v19 = 0;
    // ptr = 0;
    // v21 = 0;

    // sub_406740(&v18, &CVar::Load);

    // if (ptr) {
    //     SMemFree(ptr, a_pad, -2, 0);
    // }

    // CVar::Register("dbCompress", "Database compression", 0, "-1", 0, 5, 0, 0, 0);

    auto localeVar = CVar::Register(
        "locale",
        "Set the game locale",
        0x0,
        "****",
        &LocaleChangedCallback,
        DEFAULT,
        false,
        nullptr,
        false
    );

    if (SStrCmp(localeVar->GetString(), "****") == 0) {
        localeVar->Set("enUS", true, false, false, true);
    }

    CVar::Register(
        "useEnglishAudio",
        "override the locale and use English audio",
        0x0,
        "0",
        nullptr,
        DEFAULT,
        false,
        nullptr,
        false
    );

    // if (sub_422140()) {
    //     sub_4036B0(v24, 0, a2, (int)v2, (char)v24);
    // }

    // SStrCopy(&a1a, v2->m_stringValue.m_str, 5);

    // sub_402D50(&a1a);

    // CVar::Set(v2, &a1a, 1, 0, 0, 1);

    // SStrPrintf(dest, 260, "%s%s", *(_DWORD *)off_AB6158, v2->m_stringValue.m_str);

    // sub_421B50(dest);

    // sub_423D70();

    Sub405DD0();

    // CVar* v3 = CVar::Register(
    //     "processAffinityMask",
    //     "Sets which core(s) WoW may execute on - changes require restart to take effect",
    //     2,
    //     "0",
    //     &sub_4022E0,
    //     0,
    //     0,
    //     0,
    //     0
    // );

    // CVar* v4 = CVar::Lookup("videoOptionsVersion");

    // if (!v4 || v4->m_intValue < 3) {
    //     SStrPrintf(v23, 8, "%u", 0);
    //     CVar::Set(v3, v23, 1, 0, 0, 1);
    //     CVar::Update((int)v3);
    // }

    // v5 = v3->m_intValue;

    // if (v5) {
    //     SSetCurrentProcessAffinityMask(v5);
    // }

    BaseInitializeGlobal();

    EventInitialize(1, 0);

    // CVar* v6 = CVar::Register(
    //     "timingTestError",
    //     "Error reported by the timing validation system",
    //     6,
    //     "0",
    //     0,
    //     5,
    //     0,
    //     0,
    //     0
    // );
    // v7 = v6;

    // CVar* v8 = CVar::Register(
    //     "timingMethod",
    //     "Desired method for game timing",
    //     2,
    //     "0",
    //     &sub_403200,
    //     5,
    //     0,
    //     v6,
    //     0
    // );

    // sub_86D430(v8->m_intValue);

    // ConsoleCommandRegister("timingInfo", (int)sub_4032A0, 0, 0);

    // v9 = sub_86AD50();

    // v10 = v9 == v7->m_intValue;

    // dword_B2F9D8 = v9;

    // if (!v10) {
    //     sprintf(&v17, "%d", v9);
    //     CVar::SetReadOnly((int)v7, 0);
    //     CVar::Set(v7, &v17, 1, 0, 0, 1);
    //     CVar::Update((int)v7);
    //     CVar::SetReadOnly((int)v7, 1);
    //     ConsolePrintf("Timing test error: %d", (int)v7);
    // }

    // WowClientDB<Startup_StringsRec>::Load(&g_Startup_StringsDB, 0, ".\\Client.cpp", 0x12E3u);
    // Startup_StringsRec* v11 = g_Startup_StringsDB.GetRecordByIndex(1);
    // const char* v12;

    // if (v11) {
    //     v12 = v11->m_text;
    // } else {
    //     v12 = "World of Warcraft";
    // }

    // TODO
    // - replace with above logic for loading from Startup_Strings.dbc
    const char* v12 = "World of Warcraft";

    char v15[260];

    SStrCopy(v15, v12, 0x7FFFFFFF);

    ConsoleDeviceInitialize(v15);

    // OsIMEInitialize();

    // uint32_t v13 = OsGetAsyncTimeMs();
    // g_rndSeed.SetSeed(v13);

    Client::g_clientEventContext = EventCreateContextEx(
        1,
        &InitializeEngineCallback,
        &DestroyEngineCallback,
        0,
        0
    );

    return 1;
}

void CommonMain() {
    StormInitialize();

    // TODO
    // - error log setup
    // - misc other setup

    if (InitializeGlobal()) {
        EventDoMessageLoop();

        // TODO
        // sub_406B70();
    }

    // TODO
    // - misc cleanup
}

void BlizzardAssertCallback(const char* a1, const char* a2, const char* a3, uint32_t a4) {
    if (*a2) {
        SErrDisplayError(0, a3, a4, a2, 0, 1, 0x11111111);
    } else {
        SErrDisplayError(0, a3, a4, a1, 0, 1, 0x11111111);
    }
}

void StormInitialize() {
    // TODO
    // SStrInitialize();
    // SErrInitialize();
    // SLogInitialize();
    // SFile::Initialize();

    Blizzard::Debug::SetAssertHandler(BlizzardAssertCallback);
}

void WowClientInit() {
    // TODO
    // EventRegister(EVENT_ID_5, (int)sub_4020E0);
    // _cfltcvt_init_0();

    ClientRegisterConsoleCommands();
    ClientDBInitialize();

    // LoadingScreenInitialize();

    FrameScript_Initialize(0);
    SI2::Init(0);

    // TODO
    // sub_6F66B0();

    FrameXML_RegisterDefault();
    GlueScriptEventsInitialize();
    ScriptEventsInitialize();

    // TODO
    // sub_6F75E0();

    CCharacterComponent::Initialize();

    ClientServices::Initialize();
    // TODO ClientServices::SetMessageHandler(SMSG_TUTORIAL_FLAGS, (int)sub_530920, 0);

    // TODO
    // v2 = CVar::Lookup("EnableVoiceChat");
    // if (v2 && *(_DWORD *)(v2 + 48)) {
    //     ComSatClient_Init();
    // }

    // TODO
    // DBCache_RegisterHandlers();
    // DBCache_Initialize(a1);

    CWorldParam::Initialize();
    CWorld::Initialize();

    // TODO
    // ShadowInit();
    // GxuLightInitialize();
    // GxuLightBucketSizeSet(16.665001);
    // InputControlInitialize();

    CGlueMgr::Initialize();

    // TODO
    // if (GetConsoleMessage()) {
    //     v3 = (const char *)GetConsoleMessage();
    //     CGlueMgr::AddChangedOptionWarning(v3);
    //     SetConsoleMessage(0);
    // }

    // TODO
    // if (sub_422140()) {
    //     sub_421630();
    // }

    // TODO
    // if (byte_B2F9E1 != 1) {
    //     if ((g_playIntroMovie + 48) == 1) {
    //         CVar::Set(g_playIntroMovie, "0", 1, 0, 0, 1);
    //         CGlueMgr::SetScreen("movie");
    //     } else {
    //         CGlueMgr::SetScreen("login");
    //     }
    // } else {
    //     if ((dword_B2F980 + 48) == 1) {
    //         CVar::Set(dword_B2F980, "0", 1, 0, 0, 1);
    //         CVar::Set(g_playIntroMovie, "0", 1, 0, 0, 1);
    //         CGlueMgr::SetScreen("movie");
    //     } else {
    //         CGlueMgr::SetScreen("login");
    //     }
    // }

    // TODO
    // - temporary until above logic is implemented
    CGlueMgr::SetScreen("login");

    // TODO
    // CGlueMgr::m_pendingTimerAlert = dword_B2F9D8;
    // sub_7FC5A0();

    EventRegister(EVENT_ID_POLL, &PollNet);
}
