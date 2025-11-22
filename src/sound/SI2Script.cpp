#include "sound/SI2Script.hpp"
#include "sound/SI2.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

int32_t Script_PlaySound(lua_State* L) {
    if (lua_isnumber(L, 1)) {
        int32_t id = lua_tonumber(L, 1);
        SI2::PlayUISound(id);

        return 0;
    }

    if (!lua_isstring(L, 1)) {
        lua_pushfstring(L, "Usage: PlaySound(\"sound\")");
        lua_error(L);
    }

    char name[256];
    SStrCopy(name, lua_tostring(L, 1), sizeof(name));

    int32_t id = SI2::GetSoundKitID(name);
    SI2::PlayUISound(id);

    return 0;
}

int32_t Script_PlayMusic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlaySoundFile(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopMusic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_GameSystem_GetNumInputDrivers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_GameSystem_GetInputDriverNameByIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_GameSystem_GetNumOutputDrivers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_GameSystem_GetOutputDriverNameByIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_GameSystem_RestartSoundSystem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_ChatSystem_GetNumInputDrivers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_ChatSystem_GetInputDriverNameByIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_ChatSystem_GetNumOutputDrivers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Sound_ChatSystem_GetOutputDriverNameByIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_StartCapture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_StopCapture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_RecordLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_StopRecordingLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_PlayLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_StopPlayingLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_IsRecordingLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_IsPlayingLoopbackSound(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_GetCurrentMicrophoneSignalLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoiceChat_ActivatePrimaryCaptureCallback(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SI2::s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2] = {
    { "PlaySound",                                      &Script_PlaySound },
    { "PlayMusic",                                      &Script_PlayMusic },
    { "PlaySoundFile",                                  &Script_PlaySoundFile },
    { "StopMusic",                                      &Script_StopMusic },
    { "Sound_GameSystem_GetNumInputDrivers",            &Script_Sound_GameSystem_GetNumInputDrivers },
    { "Sound_GameSystem_GetInputDriverNameByIndex",     &Script_Sound_GameSystem_GetInputDriverNameByIndex },
    { "Sound_GameSystem_GetNumOutputDrivers",           &Script_Sound_GameSystem_GetNumOutputDrivers },
    { "Sound_GameSystem_GetOutputDriverNameByIndex",    &Script_Sound_GameSystem_GetOutputDriverNameByIndex },
    { "Sound_GameSystem_RestartSoundSystem",            &Script_Sound_GameSystem_RestartSoundSystem },
    { "Sound_ChatSystem_GetNumInputDrivers",            &Script_Sound_ChatSystem_GetNumInputDrivers },
    { "Sound_ChatSystem_GetInputDriverNameByIndex",     &Script_Sound_ChatSystem_GetInputDriverNameByIndex },
    { "Sound_ChatSystem_GetNumOutputDrivers",           &Script_Sound_ChatSystem_GetNumOutputDrivers },
    { "Sound_ChatSystem_GetOutputDriverNameByIndex",    &Script_Sound_ChatSystem_GetOutputDriverNameByIndex },
    { "VoiceChat_StartCapture",                         &Script_VoiceChat_StartCapture },
    { "VoiceChat_StopCapture",                          &Script_VoiceChat_StopCapture },
    { "VoiceChat_RecordLoopbackSound",                  &Script_VoiceChat_RecordLoopbackSound },
    { "VoiceChat_StopRecordingLoopbackSound",           &Script_VoiceChat_StopRecordingLoopbackSound },
    { "VoiceChat_PlayLoopbackSound",                    &Script_VoiceChat_PlayLoopbackSound },
    { "VoiceChat_StopPlayingLoopbackSound",             &Script_VoiceChat_StopPlayingLoopbackSound },
    { "VoiceChat_IsRecordingLoopbackSound",             &Script_VoiceChat_IsRecordingLoopbackSound },
    { "VoiceChat_IsPlayingLoopbackSound",               &Script_VoiceChat_IsPlayingLoopbackSound },
    { "VoiceChat_GetCurrentMicrophoneSignalLevel",      &Script_VoiceChat_GetCurrentMicrophoneSignalLevel },
    { "VoiceChat_ActivatePrimaryCaptureCallback",       &Script_VoiceChat_ActivatePrimaryCaptureCallback }
};
