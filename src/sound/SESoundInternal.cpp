#include "sound/SESoundInternal.hpp"
#include "sound/SESound.hpp"

#define LOG_WRITE(result, ...) \
    SESound::Log_Write(__LINE__, __FILE__, result, __VA_ARGS__);

SoundCacheNode::SoundCacheNode() {
    SESound::s_CacheList.LinkToTail(this);
}

SESoundInternal::SESoundInternal() {
    // TODO
    this->m_uniqueID = SESound::s_UniqueID++;
}

float SESoundInternal::GetVolume() {
    if (!SESound::s_Initialized) {
        return 0.0f;
    }

    float volume = this->m_volume;

    // Apply fade volume

    if (this->m_fadeIn || this->m_fadeOut) {
        volume *= this->m_fadeVolume;
    }

    // Apply channel group volume

    if (SESound::s_ChannelGroups.Count()) {
        auto channelGroupIndex = this->m_channelGroup;

        while (channelGroupIndex != -1) {
            auto channelGroup = &SESound::s_ChannelGroups[channelGroupIndex];
            auto channelGroupVolume = channelGroup->m_volume * channelGroup->m_muteVolume;

            volume *= channelGroupVolume;

            channelGroupIndex = channelGroup->m_parentChannelGroup;
        }
    }

    return volume;
}

void SESoundInternal::Play() {
    FMOD_RESULT result;

    // TODO

    // Set up fade

    if (this->m_fadeInTime <= 0.0f) {
        this->m_fadeVolume = 1.0f;
    } else {
        this->m_fadeOut = 0;
        this->m_fadeIn = 1;
    }

    // Update volume

    this->UpdateVolume();

    // Unpause channel (triggers playback)

    result = this->m_fmodChannel->setPaused(false);

    if (result != FMOD_OK && result != FMOD_ERR_CHANNEL_STOLEN && result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_OUTPUT_DRIVERCALL) {
        LOG_WRITE(result, "");
    }
}

void SESoundInternal::UpdateVolume() {
    if (!this->m_fmodChannel) {
        return;
    }

    this->m_fmodChannel->setVolume(this->GetVolume());
}

SEDiskSound::SEDiskSound() : SESoundInternal() {
    SESound::s_InternalCritSect.Enter();

    SESound::s_InternalList.LinkToTail(this);

    auto lookup = SESound::s_InternalLookupTable.New(this->m_uniqueID, SESound::s_InternalLookupKey, 0, 0x0);
    lookup->m_internal = this;

    SESound::s_InternalCritSect.Leave();
}

void SEDiskSound::Abort(FMOD_RESULT result) {
    // TODO
}

void SEDiskSound::CompleteNonBlockingLoad() {
    FMOD_RESULT result;

    // Validate sound

    if (!this->m_fmodSound) {
        LOG_WRITE(FMOD_ERR_INTERNAL, "##### ERROR! INVALID m_fmodSound");
        this->Abort(FMOD_OK);

        return;
    }

    // Prepare sound for playback (mark as paused to permit additional setup)

    result = this->m_fmodSystem->playSound(this->m_fmodSound, nullptr, true, &this->m_fmodChannel);

    if (result != FMOD_OK) {
        if (result != FMOD_ERR_MAXAUDIBLE) {
            LOG_WRITE(result, "CNBL: playSound Failed");
        }

        this->Abort(result);

        return;
    }

    // Store internal sound ID in FMOD user data

    result = this->m_fmodChannel->setUserData(&this->m_uniqueID);

    if (result != FMOD_OK) {
        LOG_WRITE(result, "CNBL: setUserData Failed");
        this->Abort(result);

        return;
    }

    // TODO

    // Play sound if appropriate

    if (this->m_playing) {
        this->Play();
    }
}
