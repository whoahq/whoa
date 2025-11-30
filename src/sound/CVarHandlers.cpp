#include "sound/CVarHandlers.hpp"
#include "console/CVar.hpp"
#include "sound/SESound.hpp"
#include "util/Unimplemented.hpp"
#include <storm/String.hpp>

bool EnableMicrophoneHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool EnableReverbHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool EnableVoiceChatHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool InboundChatVolumeHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool OutboundChatVolumeHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool OutputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool PushToTalkButtonHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool SelfMuteHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool VoiceActivationSensitivityHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool VoiceChatInputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool VoiceChatModeHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool VoiceChatOutputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool AmbienceVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool EnableAllSound_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // Enable

    if (SStrToInt(value)) {
        static auto enableSfxCVar = CVar::Lookup("Sound_EnableSFX");
        static auto enableMusicCVar = CVar::Lookup("Sound_EnableMusic");
        static auto enableAmbienceCVar = CVar::Lookup("Sound_EnableAmbience");

        SESound::MuteChannelGroup("SFX", enableSfxCVar->GetInt() == 0);
        SESound::MuteChannelGroup("MUSIC", enableMusicCVar->GetInt() == 0);
        SESound::MuteChannelGroup("AMBIENCE", enableAmbienceCVar->GetInt() == 0);

        return true;
    }

    // Disable

    SESound::MuteChannelGroup("SFX", true);
    SESound::MuteChannelGroup("MUSIC", true);
    SESound::MuteChannelGroup("AMBIENCE", true);

    return true;
}

bool EnableAmbience_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool EnableMusic_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool EnableSFX_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool MasterVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    auto volume = SStrToFloat(value);

    SESound::SetMasterVolume(volume);

    return true;
}

bool MusicVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}

bool SFXVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    WHOA_UNIMPLEMENTED(true);
}
