#include "sound/SESound.hpp"
#include "event/Event.hpp"
#include "console/CVar.hpp"
#include "util/SFile.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <cstdlib>

#define LOG_WRITE(result, ...) \
    SESound::Log_Write(__LINE__, __FILE__, result, __VA_ARGS__);

STORM_LIST(SoundCacheNode) SESound::s_CacheList;
SCritSect SESound::s_CritSect3;
int32_t SESound::s_Initialized;
SCritSect SESound::s_InternalCritSect;
STORM_LIST(SESoundInternal) SESound::s_InternalList;
TSHashTable<SOUND_INTERNAL_LOOKUP, HASHKEY_NONE> SESound::s_InternalLookupTable;
HASHKEY_NONE SESound::s_InternalLookupKey;
SCritSect SESound::s_LoadingCritSect;
FMOD::System* SESound::s_pGameSystem;
STORM_EXPLICIT_LIST(SEDiskSound, m_readyLink) SESound::s_ReadyDiskSounds;
uint32_t SESound::s_UniqueID;

void* FSoundAllocCallback(uint32_t size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    return SMemAlloc(size, "FMod", 0, 0x8);
};

void* FSoundReallocCallback(void* ptr, uint32_t size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    return SMemReAlloc(ptr, size, "FMod", 0, 0x0);
}

void FSoundFreeCallback(void* ptr, FMOD_MEMORY_TYPE type, const char *sourcestr) {
    SMemFree(ptr, "FMod", 0, 0x0);
}

FMOD_RESULT DoneLoadingCallback(FMOD_SOUND* fmodSound, FMOD_RESULT callbackResult) {
    FMOD_RESULT result;

    // Get hash value

    void* userData = nullptr;

    result = FMOD_Sound_GetUserData(fmodSound, &userData);

    if (result != FMOD_OK && result != FMOD_ERR_CHANNEL_STOLEN && result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_OUTPUT_DRIVERCALL) {
        LOG_WRITE(result, "");

        return result;
    }

    auto hashval = *static_cast<uint32_t*>(userData);

    SESound::s_InternalCritSect.Enter();

    // Get internal lookup

    auto lookup = SESound::s_InternalLookupTable.Ptr(hashval, SESound::s_InternalLookupKey);

    if (!lookup) {
        SESound::s_InternalCritSect.Leave();

        return FMOD_OK;
    }

    if (callbackResult != FMOD_OK) {
        // TODO

        return callbackResult;
    }

    // Add to ready list (processed by SESound::Heartbeat)

    for (auto existing = SESound::s_InternalList.Head(); existing; existing = SESound::s_InternalList.Link(existing)->Next()) {
        auto pendingLoad = existing->m_type == 1
            && static_cast<SEDiskSound*>(existing)->m_fmodSound == reinterpret_cast<FMOD::Sound*>(fmodSound)
            && !existing->m_fmodChannel;

        if (pendingLoad) {
            SESound::s_ReadyDiskSounds.LinkToTail(static_cast<SEDiskSound*>(existing));
        }
    }

    SESound::s_InternalCritSect.Leave();

    SESound::s_LoadingCritSect.Enter();

    // Mark cache sound node as loaded

    if (lookup->m_internal->m_useCache) {
        static_cast<SEDiskSound*>(lookup->m_internal)->m_cacheNode->m_loaded = 1;
    }

    SESound::s_LoadingCritSect.Leave();

    return FMOD_OK;
}

FMOD_RESULT SEOpenCallback(const char* name, uint32_t* filesize, void** handle, void* userdata) {
    uint32_t hashval = std::atol(name);

    SESound::s_InternalCritSect.Enter();

    auto lookup = SESound::s_InternalLookupTable.Ptr(hashval, SESound::s_InternalLookupKey);

    if (!lookup) {
        SESound::s_InternalCritSect.Leave();

        return FMOD_ERR_FILE_NOTFOUND;
    }

    auto internal = static_cast<SEDiskSound*>(lookup->m_internal);

    *filesize = SFile::GetFileSize(internal->m_file, nullptr);
    *handle = internal->m_file;
    userdata = nullptr;

    internal->m_file = nullptr;

    SESound::s_InternalCritSect.Leave();

    return FMOD_OK;
}

FMOD_RESULT SECloseCallback(void* handle, void* userdata) {
    auto file = static_cast<SFile*>(handle);

    if (SFile::Close(file)) {
        return FMOD_OK;
    } else {
        return FMOD_ERR_INVALID_HANDLE;
    }
}

FMOD_RESULT SEReadCallback(void* handle, void* buffer, uint32_t sizebytes, uint32_t* bytesread, void* userdata) {
    // TODO

    size_t read = 0;
    auto file = static_cast<SFile*>(handle);

    auto result = SFile::Read(file, buffer, sizebytes, &read, nullptr, nullptr);

    *bytesread = read;

    // TODO

    if (result) {
        return FMOD_OK;
    } else {
        // TODO
        return FMOD_ERR_FILE_EOF;
    }
}

FMOD_RESULT SESeekCallback(void* handle, uint32_t pos, void* userdata) {
    // TODO
    return FMOD_OK;
}

FMOD::SoundGroup* SESound::CreateSoundGroup(const char* name, int32_t maxAudible) {
    FMOD::SoundGroup* group = nullptr;
    FMOD_RESULT result;

    result = SESound::s_pGameSystem->createSoundGroup(name, &group);
    if (result != FMOD_OK && result != FMOD_ERR_CHANNEL_STOLEN && result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_OUTPUT_DRIVERCALL) {
        LOG_WRITE(result, "");
    }

    result = group->setMaxAudible(maxAudible);
    if (result != FMOD_OK && result != FMOD_ERR_CHANNEL_STOLEN && result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_OUTPUT_DRIVERCALL) {
        LOG_WRITE(result, "");
    }

    return group;
}

int32_t SESound::Heartbeat(const void* data, void* param) {
    if (!SESound::s_Initialized) {
        SESound::s_pGameSystem->update();

        return 1;
    }

    SESound::s_CritSect3.Enter();

    // TODO

    SESound::ProcessReadyDiskSounds();

    // TODO

    SESound::s_pGameSystem->update();

    // TODO

    SESound::s_CritSect3.Leave();

    return 1;
}

void SESound::Init(int32_t maxChannels, int32_t* a2, int32_t enableReverb, int32_t enableSoftwareHRTF, int32_t* numChannels, int32_t* outputDriverIndex, const char* outputDriverName, void* a8, int32_t a9) {
    SESound::s_Initialized = 0;

    // TODO

    FMOD_INITFLAGS fmodFlags = FMOD_INIT_NORMAL;
    FMOD_RESULT result;

    if (!a9) {
        LOG_WRITE(FMOD_OK, "=> Version %s (%s) %s", "3.3.5", "12340", "Jun 24 2010");
    }

    LOG_WRITE(FMOD_OK, " ");
    LOG_WRITE(FMOD_OK, "=> Setting up Game Sound:");
    LOG_WRITE(FMOD_OK, " - SESound Engine Init");

    // Initialize FMOD memory

    LOG_WRITE(FMOD_OK, " - FMOD Memory Init");

    FMOD::Memory_Initialize(nullptr, 0, &FSoundAllocCallback, &FSoundReallocCallback, &FSoundFreeCallback);

    // TODO

    // Create FMOD system

    SESound::s_pGameSystem = nullptr;

    LOG_WRITE(FMOD_OK, " - FMOD System Create");

    result = FMOD::System_Create(&SESound::s_pGameSystem);

    if (result != FMOD_OK) {
        // TODO

        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );
        LOG_WRITE(  result,     " -######## ERROR INITIALIZING. ALL GAME SOUND DISABLED."                                       );
        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );

        SESound::s_pGameSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);

        LOG_WRITE(FMOD_OK, "=> Game Sound Init Failed.");
        LOG_WRITE(FMOD_OK, " ");

        SESound::s_pGameSystem->init(32, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr);

        return;
    }

    // TODO

    SESound::s_pGameSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);

    // Register heartbeat

    EventRegister(EVENT_ID_POLL, &SESound::Heartbeat);

    // Get FMOD version

    uint32_t fmodVersion;

    result = SESound::s_pGameSystem->getVersion(&fmodVersion);

    if (result != FMOD_OK) {
        // TODO

        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );
        LOG_WRITE(  result,     " -######## ERROR INITIALIZING. ALL GAME SOUND DISABLED."                                       );
        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );

        SESound::s_pGameSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);

        LOG_WRITE(FMOD_OK, "=> Game Sound Init Failed.");
        LOG_WRITE(FMOD_OK, " ");

        SESound::s_pGameSystem->init(32, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr);

        return;
    }

    LOG_WRITE(FMOD_OK, " - FMOD version %08x detected", fmodVersion);

    // TODO Check FMOD version

    // TODO

    // Initialize FMOD system

    result = SESound::s_pGameSystem->init(maxChannels, fmodFlags | FMOD_INIT_3D_RIGHTHANDED, nullptr);

    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
        // TODO
    }

    // TODO

    // Set doppler scale, distance factor, rolloff scale

    result = SESound::s_pGameSystem->set3DSettings(1.0f, 1.0f, 4.0f);

    if (result != FMOD_OK) {
        // TODO

        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );
        LOG_WRITE(  result,     " -######## ERROR INITIALIZING. ALL GAME SOUND DISABLED."                                       );
        LOG_WRITE(  FMOD_OK,    " -###########################################################################################" );

        SESound::s_pGameSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);

        LOG_WRITE(FMOD_OK, "=> Game Sound Init Failed.");
        LOG_WRITE(FMOD_OK, " ");

        SESound::s_pGameSystem->init(32, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr);

        return;
    }

    // TODO

    SESound::s_Initialized = 1;

    LOG_WRITE(FMOD_OK, " - FMOD System Init OK!");
    LOG_WRITE(FMOD_OK, "=> Game Sound is ready.");
    LOG_WRITE(FMOD_OK, " ");
}

int32_t SESound::IsInitialized() {
    return SESound::s_Initialized == 1;
}

int32_t SESound::LoadDiskSound(FMOD::System* fmodSystem, const char* filename, FMOD_MODE fmodMode, SESound* sound, FMOD::SoundGroup* fmodSoundGroup1, FMOD::SoundGroup* fmodSoundGroup2, bool a7, int32_t a8, uint32_t a9, int32_t a10, uint32_t decodeBufferSize, int32_t a12, float a13, float a14, float a15, float* a16) {
    SESound::s_LoadingCritSect.Enter();

    if (!sound || !fmodSystem || !filename || !*filename) {
        SESound::s_LoadingCritSect.Leave();

        return 0;
    }

    FMOD_RESULT result;

    if (sound->m_internal) {
        // TODO sound->m_internal->byte6D = 1;

        if (sound->m_internal->m_fmodChannel) {
            sound->m_internal->m_fmodChannel->stop();
            sound->m_internal->m_fmodChannel = nullptr;
        }

        if (sound->m_internal) {
            sound->m_internal->m_sound = nullptr;
            sound->m_internal = nullptr;
        }
    }

    auto internal = STORM_NEW(SEDiskSound);

    // TODO populate various SEDiskSound members

    internal->m_fmodSystem = fmodSystem;
    internal->m_fmodMode = fmodMode;
    internal->m_type = 1;
    internal->m_useCache = 0;
    internal->m_fmodChannel = nullptr;

    internal->m_sound = sound;
    sound->m_internal = internal;

    FMOD_CREATESOUNDEXINFO info = {};
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.nonblockcallback = &DoneLoadingCallback;
    info.fileuseropen = &SEOpenCallback;
    info.fileuserclose = &SECloseCallback;
    info.fileuserread = &SEReadCallback;
    info.fileuserseek = &SESeekCallback;
    info.decodebuffersize = decodeBufferSize;
    info.userdata = &internal->m_uniqueID;
    info.suggestedsoundtype = FMOD_SOUND_TYPE_UNKNOWN;
    info.initialseekposition = 0;
    info.initialseekpostype = 0;

    bool useCache = true;

    auto ext = SStrChrR(filename, '.');
    if (ext) {
        if (!SStrCmpI(ext, ".wav")) {
            info.suggestedsoundtype = SFile::IsStreamingTrial() ? FMOD_SOUND_TYPE_UNKNOWN: FMOD_SOUND_TYPE_WAV;
        } else if (!SStrCmpI(ext, ".mp3")) {
            useCache = false;
            fmodMode |= FMOD_MPEGSEARCH;
            info.suggestedsoundtype = FMOD_SOUND_TYPE_MPEG;
        }
    }

    // Generate name

    char fmodName[300];
    SStrPrintf(fmodName, sizeof(fmodName), "%-24d%s", internal->m_uniqueID, filename);

    // Generate cache key

    char cacheKey[400];
    auto loopStr = fmodMode & FMOD_LOOP_NORMAL ? "_LOOP_" : "_NOLOOP_";
    auto positionStr = fmodMode & FMOD_2D ? "_2D_" : "_3D_";
    SStrPrintf(cacheKey, sizeof(cacheKey), "%s%s%s", filename, positionStr, loopStr);

    auto cacheHashval = SStrHash(cacheKey);

    // Load from cache

    if (useCache) {
        for (auto cacheNode = SESound::s_CacheList.Head(); cacheNode; cacheNode = SESound::s_CacheList.Link(cacheNode)->Next()) {
            if (cacheNode->m_hashval == cacheHashval) {
                // Cache hit

                internal->m_fmodSound = cacheNode->m_fmodSound;
                internal->m_useCache = 1;
                internal->m_cacheNode = cacheNode;

                // TODO

                if (cacheNode->m_loaded) {
                    internal->m_nonblockingReady = 1;
                }

                // TODO

                SESound::s_LoadingCritSect.Leave();

                return 1;
            }
        }
    }

    // Validate file exists

    if (!SFile::OpenEx(nullptr, filename, 1, &internal->m_file)) {
        LOG_WRITE(FMOD_ERR_FILE_NOTFOUND, filename);

        SESound::s_InternalCritSect.Enter();

        // TODO

        SESound::s_InternalCritSect.Leave();

        SESound::s_LoadingCritSect.Leave();

        return 0;
    }

    fmodMode |= FMOD_VIRTUAL_PLAYFROMSTART | FMOD_IGNORETAGS | FMOD_NONBLOCKING;

    uint32_t maxCacheSize = 1048576;
    static auto maxCacheSizeVar = CVar::Lookup("Sound_MaxCacheableSizeInBytes");
    if (maxCacheSizeVar) {
        maxCacheSize = maxCacheSizeVar->GetInt() > 2097152 ? 2097152 : maxCacheSizeVar->GetInt();
    }

    uint32_t fileSize = internal->m_file ? SFile::GetFileSize(internal->m_file, nullptr) : 0;

    // Create FMOD stream or sound

    if (fileSize > maxCacheSize || !useCache) {
        useCache = false;

        result = fmodSystem->createStream(fmodName, fmodMode, &info, &internal->m_fmodSound);

        // TODO counter
    } else {
        result = fmodSystem->createSound(fmodName, fmodMode, &info, &internal->m_fmodSound);

        // TODO other counter
    }

    // Create FMOD stream or sound failed

    if (result != FMOD_OK) {
        if (result != FMOD_ERR_OUTPUT_CREATEBUFFER) {
            LOG_WRITE(result, filename);
        }

        s_InternalCritSect.Enter();

        SFile::Close(internal->m_file);
        internal->m_file = nullptr;

        // TODO

        s_InternalCritSect.Leave();

        s_LoadingCritSect.Leave();

        return 0;
    }

    // Create cache node

    if (useCache) {
        auto cacheNode = STORM_NEW(SoundCacheNode);

        cacheNode->m_fmodSound = internal->m_fmodSound;
        cacheNode->m_hashval = cacheHashval;
        SStrCopy(cacheNode->m_filename, filename, sizeof(cacheNode->m_filename));

        // TODO

        internal->m_useCache = 1;
        internal->m_cacheNode = cacheNode;
    }

    // TODO

    // No cache hit (yet)
    internal->m_nonblockingReady = 0;

    s_LoadingCritSect.Leave();

    return 1;
}

void SESound::Log_Write(int32_t line, const char* file, FMOD_RESULT result, const char* fmt, ...) {
    // TODO
}

void SESound::ProcessReadyDiskSounds() {
    while (auto diskSound = SESound::s_ReadyDiskSounds.Head()) {
        SESound::s_ReadyDiskSounds.UnlinkNode(diskSound);
        diskSound->CompleteNonBlockingLoad();
    }
}

void SESound::CompleteLoad() {
    if (!this->m_internal) {
        return;
    }

    if (this->m_internal->m_type == 1 && this->m_internal->m_nonblockingReady) {
        static_cast<SEDiskSound*>(this->m_internal)->CompleteNonBlockingLoad();
    }
}

SEUserData* SESound::GetUserData() {
    if (!this->m_internal) {
        return nullptr;
    }

    return this->m_internal->m_userData;
}

bool SESound::IsPlaying() {
    if (!this->m_internal) {
        return 0;
    }

    if (!this->m_internal->m_fmodChannel) {
        return 0;
    }

    bool isPlaying;
    this->m_internal->m_fmodChannel->isPlaying(&isPlaying);

    return isPlaying;
}

int32_t SESound::Load(const char* filename, int32_t a3, FMOD::SoundGroup* soundGroup1, FMOD::SoundGroup* soundGroup2, bool a6, bool a7, uint32_t a8, int32_t a9, uint32_t a10) {
    if (!SESound::s_Initialized) {
        return 0;
    }

    return SESound::LoadDiskSound(
        SESound::s_pGameSystem,
        filename,
        ((a3 != 0) + 1) | (0x40 | 0x8), // TODO FMOD_MODE
        this,
        soundGroup1,
        soundGroup2,
        a6,
        a9,
        a8,
        a7,
        a10,
        0,
        360.0f,
        360.0f,
        1.0f,
        nullptr
    );
}

void SESound::Play() {
    if (!this->m_internal) {
        return;
    }

    // TODO volume

    this->m_internal->m_playing = 1;

    if (this->m_internal->m_fmodChannel) {
        this->m_internal->Play();
    }
}

void SESound::SetUserData(SEUserData* userData) {
    if (!this->m_internal) {
        return;
    }

    this->m_internal->m_userData = userData;
}
