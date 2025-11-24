#include "sound/SESound.hpp"
#include <storm/Memory.hpp>

#define LOG_WRITE(result, ...) \
    SESound::Log_Write(__LINE__, __FILE__, result, __VA_ARGS__);

int32_t SESound::s_Initialized;
FMOD::System* SESound::s_pGameSystem;

void* FSoundAllocCallback(uint32_t size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    return SMemAlloc(size, "FMod", 0, 0x8);
};

void* FSoundReallocCallback(void* ptr, uint32_t size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    return SMemReAlloc(ptr, size, "FMod", 0, 0x0);
}

void FSoundFreeCallback(void* ptr, FMOD_MEMORY_TYPE type, const char *sourcestr) {
    SMemFree(ptr, "FMod", 0, 0x0);
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

    // TODO

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

void SESound::Log_Write(int32_t line, const char* file, FMOD_RESULT result, const char* fmt, ...) {
    // TODO
}

int32_t SESound::Load(const char* filename, int32_t a3, FMOD::SoundGroup* soundGroup1, FMOD::SoundGroup* soundGroup2, bool a6, bool a7, uint32_t a8, int32_t a9, uint32_t a10) {
    // TODO

    return 0;
}
