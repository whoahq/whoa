#include "sound/SI2.hpp"
#include "SoundKitProperties.hpp"
#include "console/CVar.hpp"
#include "sound/CVarHandlers.hpp"
#include "sound/SESound.hpp"
#include "sound/SOUNDKITDEF.hpp"
#include "sound/SOUNDKITLOOKUP.hpp"
#include "sound/SOUNDKITOBJECT.hpp"
#include "ui/FrameScript.hpp"

TSGrowableArray<SOUNDKITDEF*> SI2::s_SoundKitDefs;
TSHashTable<SOUNDKITLOOKUP, HASHKEY_CONSTSTRI> SI2::s_SoundKitLookupTable;

char SI2::s_SoundCategory[32][18] = {
    "SFX",
    "MUSIC",
    "AMBIENCE",
    "CINEMATIC",
    "SCRIPTSOUND",
    "SCRIPTMUSIC",
    "RACIALCINEMATIC",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
    "SFX",
};

SOUNDKITDEF* SI2::GetSoundKitDef(int32_t id) {
    if (id >= SI2::s_SoundKitDefs.Count() || id <= 0) {
        return nullptr;
    }

    return SI2::s_SoundKitDefs[id];
}

int32_t SI2::GetSoundKitID(const char* name) {
    auto lookup = SI2::s_SoundKitLookupTable.Ptr(name);

    if (lookup) {
        return lookup->ID;
    }

    return 0;
}

int32_t SI2::Init(int32_t a1) {
    // TODO
    // if (CmdLineGetBool(26)) {
    //     SI2::RegisterCVars();
    //     SI2::RegisterUserCVars();
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

    static auto enableReverbVar = CVar::Lookup("Sound_EnableReverb");

    if (enableReverbVar) {
        enableReverb = enableReverbVar->GetInt();
    }

    // Enable software HRTF

    int32_t enableSoftwareHRTF = 0;

    static auto enableSoftwareHRTFVar = CVar::Lookup("Sound_EnableSoftwareHRTF");

    if (enableSoftwareHRTFVar) {
        enableSoftwareHRTF = enableSoftwareHRTFVar->GetInt();
    }

    // Num channels

    int32_t numChannels = 4;

    static auto numChannelsVar = CVar::Lookup("Sound_NumChannels");

    if (numChannelsVar) {
        numChannels = numChannelsVar->GetInt();
    }

    // Output driver index

    int32_t outputDriverIndex = 0;

    static auto outputDriverIndexVar = CVar::Lookup("Sound_OutputDriverIndex");

    if (outputDriverIndexVar) {
        outputDriverIndex = outputDriverIndexVar->GetInt();
    }

    // Output driver name

    const char* outputDriverName = "";

    static auto outputDriverNameVar = CVar::Lookup("Sound_OutputDriverName");

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

    SI2::RegisterUserCVars();

    if (!a1) {
        SI2::InitSoundKitDefs();
        // TODO
    }

    SI2::InitSoundKitGroups();

    // TODO

    return 0;
}

void SI2::InitSoundKitDefs() {
    SI2::s_SoundKitDefs.SetCount(g_soundEntriesDB.m_maxID + 1);

    for (uint32_t i = 0; i < g_soundEntriesDB.GetNumRecords(); i++) {
        auto soundEntriesRec = g_soundEntriesDB.GetRecordByIndex(i);

        // Populate sound kit definitions

        auto soundKitDef = STORM_NEW(SOUNDKITDEF);

        // Combine directory base and file into full path

        for (uint32_t j = 0; j < 10; j++) {
            auto file = soundEntriesRec->m_file[j];
            auto directoryBase = soundEntriesRec->m_directoryBase;

            if (!file || !*file) {
                break;
            }

            uint32_t combinedLength = std::strlen(file) + 1;
            if (*directoryBase) {
                combinedLength += std::strlen(directoryBase) + 1;
            }

            auto fullPath = static_cast<char*>(calloc(1, combinedLength));

            if (*directoryBase) {
                bool directoryEndsInSlash = directoryBase[std::strlen(directoryBase) - 1] == '\\';

                if (directoryEndsInSlash) {
                    sprintf(fullPath, "%s%s", directoryBase, file);
                } else {
                    sprintf(fullPath, "%s\\%s", directoryBase, file);
                }
            } else {
                sprintf(fullPath, "%s", file);
            }

            soundKitDef->files[j] = fullPath;
            soundKitDef->fileCount++;
        }

        // Copy relevant record fields

        soundKitDef->flags = soundEntriesRec->m_flags;
        soundKitDef->name = soundEntriesRec->m_name;
        soundKitDef->minDistance = soundEntriesRec->m_minDistance;
        soundKitDef->distanceCutoff = soundEntriesRec->m_distanceCutoff;
        soundKitDef->volume = soundEntriesRec->m_volumeFloat;
        soundKitDef->ID = soundEntriesRec->m_ID;
        soundKitDef->advancedID = soundEntriesRec->m_soundEntriesAdvancedID;
        soundKitDef->advanced = nullptr;

        if (soundKitDef->advancedID) {
            soundKitDef->advanced = g_soundEntriesAdvancedDB.GetRecord(soundKitDef->advancedID);
        }

        SI2::s_SoundKitDefs[soundEntriesRec->GetID()] = soundKitDef;

        // Populate sound kit lookups

        auto soundKitLookup = SI2::s_SoundKitLookupTable.Ptr(soundEntriesRec->m_name);

        if (!soundKitLookup) {
            soundKitLookup = SI2::s_SoundKitLookupTable.New(soundEntriesRec->m_name, 0, 0x0);
        }

        soundKitLookup->ID = soundEntriesRec->GetID();
    }
}

void SI2::InitSoundKitGroups() {
    for (uint32_t i = 0; i < SI2::s_SoundKitDefs.Count(); i++) {
        auto soundKitDef = SI2::s_SoundKitDefs[i];

        if (soundKitDef && soundKitDef->name) {
            soundKitDef->soundGroup1 = SESound::CreateSoundGroup(soundKitDef->name, 3);
            soundKitDef->soundGroup2 = SESound::CreateSoundGroup(soundKitDef->name, 3);
        }
    }
}

bool SI2::IsPlaying(SOUNDKITOBJECT* object) {
    return object->m_sound->IsPlaying();
}

int32_t SI2::PlaySoundKit(const char *name, int a2, SOUNDKITOBJECT* object, SoundKitProperties* properties) {
    if (!SESound::IsInitialized()) {
        return 17;
    }

    auto id = SI2::GetSoundKitID(name);

    if (!id) {
        return 7;
    }

    return SI2::PlaySoundKit(id, a2, object, properties, 0, nullptr, 1, 0);
}

int32_t SI2::PlaySoundKit(int32_t id, int32_t a2, SOUNDKITOBJECT* object, SoundKitProperties* properties, int32_t a5,  void* a6, int32_t a7, int32_t a8) {
    // Basic validations

    if (id <= 0) {
        return 5;
    }

    auto soundKitDef = SI2::GetSoundKitDef(id);

    if (!soundKitDef) {
        return 5;
    }

    if (!soundKitDef->fileCount) {
        return 6;
    }

    if (!SESound::IsInitialized()) {
        return 17;
    }

    // Default properties

    SoundKitProperties defaultProperties;
    defaultProperties.ResetToDefaults();

    if (!properties) {
        properties = &defaultProperties;
    }

    // Sound type checks

    static auto enableAllSoundVar = CVar::Lookup("Sound_EnableAllSound");

    if (!enableAllSoundVar || !enableAllSoundVar->GetInt() /* TODO || dword_BD0800 */) {
        return 17;
    }

    // TODO

    if (!std::strcmp(SI2::s_SoundCategory[properties->m_type], "SFX")) {
        static auto enableSFXVar = CVar::Lookup("Sound_EnableSFX");

        if (!enableSFXVar || !enableSFXVar->GetInt()) {
            return 9;
        }
    }

    if (
        !std::strcmp(SI2::s_SoundCategory[properties->m_type], "MUSIC")
        || !strcmp(SI2::s_SoundCategory[properties->m_type], "SCRIPTMUSIC")
    ) {
        static auto enableMusicVar = CVar::Lookup("Sound_EnableMusic");

        if (!enableMusicVar || !enableMusicVar->GetInt()) {
            return 10;
        }
    }

    if (properties->m_type == 2) {
        static auto enableAmbienceVar = CVar::Lookup("Sound_EnableAmbience");

        if (!enableAmbienceVar || !enableAmbienceVar->GetInt()) {
            return 11;
        }
    }

    if (properties->m_type >= 18) {
        return 8;
    }

    // TODO

    if (properties->uint24 == 2) {
        soundKitDef->flags |= 0x20;
    }

    if (properties->uint24 == 1) {
        soundKitDef->flags &= ~0x20u;
    }

    if (!(soundKitDef->flags & 0x20) /* TODO || !dword_B4A394 */) {
        // TODO

        return 15;
    }

    SESound* sound;

    if (object) {
        sound = object->m_sound;

        // TODO
    } else {
        SESound newSound = {};
        sound = &newSound;

        // TODO
    }

    // TODO SOUNDKITDEF::SelectNextFile
    auto filename = soundKitDef->files[0];

    // TODO

    int32_t unkInt = 0;
    bool unkBool = true;

    // TODO

    if (a2) {
        // TODO
    } else {
        auto result = sound->Load(
            filename,
            unkInt,
            soundKitDef->soundGroup1,
            soundKitDef->soundGroup2,
            unkBool,
            properties->byte38,
            properties->uint3C,
            properties->int20,
            properties->uint28
        );

        if (!result) {
            // TODO sound->Sub8799E0();

            return 16;
        }
    }

    // TODO

    sound->CompleteLoad();

    // TODO

    sound->Play();

    // TODO

    return 0;
}

void SI2::PlayUISound(int32_t id) {
    static auto voiceChatOnID = SI2::GetSoundKitID("VoiceChatOn");
    static auto voiceChatOffID = SI2::GetSoundKitID("VoiceChatOff");

    SoundKitProperties properties;
    properties.ResetToDefaults();

    if (id == voiceChatOnID || id == voiceChatOffID) {
        properties.m_type = 3;
    }

    properties.int20 = 128;
    properties.uint24 = 2;
    properties.uint3C = 0;

    SI2::PlaySoundKit(id, 0, nullptr, &properties, 0, nullptr, 1, 0);
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

void SI2::RegisterUserCVars() {
    CVar::Register("ChatMusicVolume",
        "music volume (0.0 to 1.0)",
        0x0,
        "0.3",
        nullptr,
        SOUND
    );

    CVar::Register(
        "ChatSoundVolume",
        "sound volume (0.0 to 1.0)",
        0x0,
        "0.4",
        nullptr,
        SOUND
    );

    CVar::Register(
        "ChatAmbienceVolume",
        "Ambience Volume (0.0 to 1.0)",
        0x0,
        "0.3",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableSFX",
        "",
        0x0,
        "1",
        &EnableSFX_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_EnableAmbience",
        "Enable Ambience",
        0x0,
        "1",
        &EnableAmbience_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_EnableErrorSpeech",
        "error speech",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableMusic",
        "Enables music",
        0x0,
        "1",
        &EnableMusic_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_EnableAllSound",
        "",
        0x0,
        "1",
        &EnableAllSound_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_MasterVolume",
        "master volume (0.0 to 1.0)",
        0x0,
        "1.0",
        &MasterVolume_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_SFXVolume",
        "sound volume (0.0 to 1.0)",
        0x0,
        "1.0",
        &SFXVolume_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_MusicVolume",
        "music volume (0.0 to 1.0)",
        0x0,
        "0.4",
        &MusicVolume_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_AmbienceVolume",
        "Ambience Volume (0.0 to 1.0)",
        0x0,
        "0.6",
        &AmbienceVolume_CVarCallback,
        SOUND
    );

    CVar::Register(
        "Sound_ListenerAtCharacter",
        "lock listener at character",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableEmoteSounds",
        "",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_ZoneMusicNoDelay",
        "",
        0x0,
        "0",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableArmorFoleySoundForSelf",
        "",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableArmorFoleySoundForOthers",
        "",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    // TODO

    CVar::Register(
        "Sound_EnablePetSounds",
        "Enables pet sounds",
        0x0,
        "1",
        nullptr,
        SOUND
    );

    CVar::Register(
        "Sound_EnableSoundWhenGameIsInBG",
        "Enable Sound When Game Is In Background",
        0x0,
        "0",
        nullptr,
        SOUND
    );

    // TODO
}

void SI2::RegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_SI2; ++i) {
        FrameScript_RegisterFunction(
            SI2::s_ScriptFunctions[i].name,
            SI2::s_ScriptFunctions[i].method
        );
    }
}

void SI2::StartGlueMusic(const char* name) {
    // TODO
}
