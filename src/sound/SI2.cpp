#include "sound/SI2.hpp"
#include "console/CVar.hpp"
#include "sound/CVarHandlers.hpp"
#include "sound/SESound.hpp"
#include "ui/FrameScript.hpp"

CVar* SI2::s_pCVar_Sound_EnableReverb;
CVar* SI2::s_pCVar_Sound_EnableSoftwareHRTF;
CVar* SI2::s_pCVar_Sound_NumChannels;
CVar* SI2::s_pCVar_Sound_OutputDriverIndex;
CVar* SI2::s_pCVar_Sound_OutputDriverName;

static int32_t s_initFlags;

int32_t SI2::Init(int32_t a1) {
    // TODO
    // if (CmdLineGetBool(26)) {
    //     SI2::RegisterCVars();
    //
    //     // TODO
    //     // sub_4D1600();
    //
    //     return 17;
    // }

    if (!a1) {
        // TODO
        // sub_4CB160();
        // sub_4CF990();
        // sub_4CECB0();
        // sub_4D0B70();
        // sub_4D04E0();

        SI2::RegisterCVars();
    }

    // Enable reverb

    int32_t enableReverb = 0;

    if (!(s_initFlags & 0x1)) {
        SI2::s_pCVar_Sound_EnableReverb = CVar::Lookup("Sound_EnableReverb");
        s_initFlags |= 0x1;
    }

    auto enableReverbVar = SI2::s_pCVar_Sound_EnableReverb;

    if (enableReverbVar) {
        enableReverb = enableReverbVar->GetInt();
    }

    // Enable software HRTF

    int32_t enableSoftwareHRTF = 0;

    if (!(s_initFlags & 0x2)) {
        SI2::s_pCVar_Sound_EnableSoftwareHRTF = CVar::Lookup("Sound_EnableSoftwareHRTF");
        s_initFlags |= 0x2;
    }

    auto enableSoftwareHRTFVar = SI2::s_pCVar_Sound_EnableSoftwareHRTF;

    if (enableSoftwareHRTFVar) {
        enableSoftwareHRTF = enableSoftwareHRTFVar->GetInt();
    }

    // Num channels

    int32_t numChannels = 4;

    if (!(s_initFlags & 0x4)) {
        SI2::s_pCVar_Sound_NumChannels = CVar::Lookup("Sound_NumChannels");
        s_initFlags |= 0x4;
    }

    auto numChannelsVar = SI2::s_pCVar_Sound_NumChannels;

    if (numChannelsVar) {
        numChannels = numChannelsVar->GetInt();
    }

    // Output driver index

    int32_t outputDriverIndex = 0;

    if (!(s_initFlags & 0x8)) {
        SI2::s_pCVar_Sound_OutputDriverIndex = CVar::Lookup("Sound_OutputDriverIndex");
        s_initFlags |= 0x8;
    }

    auto outputDriverIndexVar = SI2::s_pCVar_Sound_OutputDriverIndex;

    if (outputDriverIndexVar) {
        outputDriverIndex = outputDriverIndexVar->GetInt();
    }

    // Output driver name

    const char* outputDriverName = "";

    if (!(s_initFlags & 0x10)) {
        SI2::s_pCVar_Sound_OutputDriverName = CVar::Lookup("Sound_OutputDriverName");
        s_initFlags |= 0x10;
    }

    auto outputDriverNameVar = SI2::s_pCVar_Sound_OutputDriverName;

    if (outputDriverNameVar) {
        outputDriverName = outputDriverNameVar->GetString();
    }

    SESound::Init(
        512,
        nullptr,                // TODO callback fn
        enableReverb,
        enableSoftwareHRTF,
        &numChannels,
        &outputDriverIndex,
        outputDriverName,
        nullptr,                // TODO callback fn
        a1
    );

    // TODO

    return 0;
}

void SI2::RegisterCVars() {
    CVar::Register(
        "StartTalkingDelay",
        "",
        0x0,
        "0.0",
        nullptr,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "StartTalkingTime",
        "",
        0x0,
        "1.0",
        nullptr,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "StopTalkingDelay",
        "",
        0x0,
        "0.0",
        nullptr,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "StopTalkingTime",
        "",
        0x0,
        "2.0",
        nullptr,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "OutboundChatVolume",
        "The software amplification factor (0.0 - 2.0)",
        0x0,
        "1.0",
        &OutboundChatVolumeHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "InboundChatVolume",
        "The volume of all other chat you hear (0.0 - 1.0)",
        0x0,
        "1.0",
        &InboundChatVolumeHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "VoiceChatMode",
        "Push to talk(0) or voice activation(1)",
        0x0,
        "0",
        &VoiceChatModeHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "VoiceActivationSensitivity",
        "Sensitivity of the microphone (0.0 - 1.0)",
        0x0,
        "0.4",
        &VoiceActivationSensitivityHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "EnableMicrophone",
        "Enables the microphone so you can speak.",
        0x0,
        "1",
        &EnableMicrophoneHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "EnableVoiceChat",
        "Enables the voice chat feature.",
        0x0,
        "0",
        &EnableVoiceChatHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "VoiceChatSelfMute",
        "Turn off your ability to talk.",
        0x0,
        "0",
        &SelfMuteHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "PushToTalkButton",
        "String representation of the Push-To-Talk button.",
        0x0,
        "`",
        &PushToTalkButtonHandler,
        DEFAULT,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_OutputQuality",
        "sound quality, default 1 (medium)",
        0x0,
        "1",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_NumChannels",
        "number of sound channels",
        0x0,
        "32",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_EnableReverb",
        "",
        0x0,
        "0",
        &EnableReverbHandler,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_EnableSoftwareHRTF",
        "",
        0x0,
        "0",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_VoiceChatInputDriverIndex",
        "",
        0x0,
        "0",
        &VoiceChatInputDriverHandler,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_VoiceChatInputDriverName",
        "",
        0,
        "Primary Sound Capture Driver",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_VoiceChatOutputDriverIndex",
        "",
        0x0,
        "0",
        &VoiceChatOutputDriverHandler,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_VoiceChatOutputDriverName",
        "",
        0x0,
        "Primary Sound Driver",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_OutputDriverIndex",
        "",
        0x0,
        "0",
        &OutputDriverHandler,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_OutputDriverName",
        "",
        0x0,
        "Primary Sound Driver",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_DSPBufferSize",
        "sound buffer size, default 0",
        0x0,
        "0",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_EnableHardware",
        "Enables Hardware",
        0x0,
        "0",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_EnableMode2",
        "test",
        0x0,
        "0",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );

    CVar::Register(
        "Sound_EnableMixMode2",
        "test",
        0x0,
        "0",
        nullptr,
        SOUND,
        false,
        nullptr,
        false
    );
}

void SI2::RegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_SI2; ++i) {
        FrameScript_RegisterFunction(
            SI2::s_ScriptFunctions[i].name,
            SI2::s_ScriptFunctions[i].method
        );
    }
}
