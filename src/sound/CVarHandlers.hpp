#ifndef SOUND_C_VAR_HANDLERS_HPP
#define SOUND_C_VAR_HANDLERS_HPP

class CVar;

bool EnableMicrophoneHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableReverbHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableVoiceChatHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool InboundChatVolumeHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool OutboundChatVolumeHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool OutputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool PushToTalkButtonHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool SelfMuteHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool VoiceActivationSensitivityHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool VoiceChatInputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool VoiceChatModeHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool VoiceChatOutputDriverHandler(CVar* var, const char* oldValue, const char* value, void* arg);

bool AmbienceVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableAllSound_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableAmbience_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableMusic_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool EnableSFX_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool MasterVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool MusicVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

bool SFXVolume_CVarCallback(CVar* var, const char* oldValue, const char* value, void* arg);

#endif
