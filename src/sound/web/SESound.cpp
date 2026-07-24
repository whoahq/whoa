#include "sound/SESound.hpp"
#include "sound/SEChannelGroup.hpp"

// Web stub implementation - sound is not supported on web platform

TSGrowableArray<SEChannelGroup> SESound::s_ChannelGroups;
int32_t SESound::s_Initialized = 0;

void* SESound::CreateSoundGroup(const char* name, int32_t maxAudible) {
    return nullptr;
}

SEChannelGroup* SESound::GetChannelGroup(const char* name, bool create, bool createInMaster) {
    return nullptr;
}

float SESound::GetChannelGroupVolume(const char* name) {
    return 0.0f;
}

int32_t SESound::Heartbeat(const void* data, void* param) {
    return 1;
}

void SESound::Init(int32_t maxChannels, int32_t (*a2), int32_t enableReverb, int32_t enableSoftwareHRTF, int32_t* numChannels, int32_t* outputDriverIndex, const char* outputDriverName, void (*a8), int32_t a9) {
    // Sound not supported on web - leave as not initialized
    SESound::s_Initialized = 0;
}

int32_t SESound::IsInitialized() {
    return 0;
}

void SESound::MuteChannelGroup(const char* name, bool mute) {
}

void SESound::SetChannelGroupVolume(const char* name, float volume) {
}

void SESound::SetMasterVolume(float volume) {
}

void SESound::CompleteLoad() {
}

SEUserData* SESound::GetUserData() {
    return nullptr;
}

bool SESound::IsPlaying() {
    return false;
}

int32_t SESound::Load(const char* filename, int32_t a3, void* soundGroup1, void* soundGroup2, bool a6, bool a7, uint32_t a8, int32_t a9, uint32_t a10) {
    return 0;
}

void SESound::Play() {
}

void SESound::SetChannelGroup(const char* name, bool inMaster) {
}

void SESound::SetFadeInTime(float fadeInTime) {
}

void SESound::SetFadeOutTime(float fadeOutTime) {
}

void SESound::SetUserData(SEUserData* userData) {
}

void SESound::SetVolume(float volume) {
}

void SESound::StopOrFadeOut(int32_t stop, float fadeOutTime) {
}
