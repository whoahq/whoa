#include "util/BlizzardCore.hpp"
#include <cstring>
#include <ctime>
#include <map>
#include <storm/Memory.hpp>

#if defined(WHOA_SYSTEM_MAC)
    #include <libkern/OSAtomic.h>
#elif defined(WHOA_SYSTEM_LINUX)
    #include <atomic>
#endif

Blizzard::System_Debug::AssertCallback Blizzard::System_Debug::s_assertCallback = nullptr;

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
bool Blizzard::System_Thread::s_initialized;
Blizzard::Lock::DoOnceData Blizzard::System_Lock::s_initMutexAttrOnce;
Blizzard::System_Lock::MutexAttr Blizzard::System_Lock::s_mutexattr;
Blizzard::Thread::ThreadRecord* Blizzard::System_Thread::s_mainThread;
Blizzard::Lock::Mutex Blizzard::System_Thread::s_mutex;
Blizzard::Lock::Mutex Blizzard::System_Thread::s_registryMutex;
Blizzard::Thread::TLSSlot Blizzard::System_Thread::s_stackTraceEntryPointTLS;
Blizzard::Thread::TLSSlot Blizzard::System_Thread::s_threadRecordTLS;
std::map<Blizzard::Thread::ThreadRecord*, Blizzard::Thread::ThreadRecord*>* Blizzard::System_Thread::s_threadRegistry;
Blizzard::Thread::TLSSlot* Blizzard::System_Thread::s_slotList[128];
int32_t Blizzard::System_Thread::s_slotListUsed;
#endif

void Blizzard::Debug::Assert(const char* a1, const char* a2, uint32_t a3) {
    if (System_Debug::s_assertCallback) {
        System_Debug::s_assertCallback(a1, "", a2, a3);
    }
}

void Blizzard::Debug::SetAssertHandler(Blizzard::System_Debug::AssertCallback callback) {
    System_Debug::s_assertCallback = callback;
}

void* Blizzard::Memory::Allocate(uint32_t bytes) {
    return SMemAlloc(bytes, __FILE__, __LINE__, 0);
}

void* Blizzard::Memory::Allocate(uint32_t a1, uint32_t a2, const char* a3, uint32_t a4, const char* a5) {
    // TODO
    // - what is a5?
    // - arg order?
    // - flag manipluation?

    return SMemAlloc(a1, a3, a4, a2);
}

void Blizzard::Memory::Free(void* ptr) {
    SMemFree(ptr);
}

int32_t Blizzard::String::Copy(char* dst, const char* src, size_t len) {
    if (!len || !dst) {
        return 0;
    }

    if (!src) {
        *dst = 0;
        return 0;
    }

    char* v3 = dst + len - 1;

    char v4;
    const char* v5;
    char* v6;

    int32_t result;

    if (dst < v3 && (v4 = *src, v5 = src, v6 = dst, *src)) {
        do {
            *v6++ = v4;

            if (v3 <= v6) {
                break;
            }

            v4 = (v5++)[1];
        } while ( v4 );

        result = v6 - dst;
    } else {
        v6 = dst;
        result = 0;
    }

    *v6 = 0;

    return result;
}

uint32_t Blizzard::String::Length(const char* a1) {
    if (a1) {
        return strlen(a1);
    } else {
        return 0;
    }
}

void Blizzard::String::MemFill(void* a1, uint32_t a2, unsigned char a3) {
    memset(a1, a3, a2);
}

void Blizzard::Process::Sleep(uint32_t duration) {
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    struct timespec request;

    request.tv_sec = 0;
    request.tv_nsec = duration * 1000000;

    nanosleep(&request, nullptr);
#endif
}

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
void Blizzard::Lock::DoOnce(DoOnceData& a1, void (*a2)(void*), void* a3) {
    if (!a1.done) {
        if (Blizzard::Lock::Atomic::Increment(&a1.atomic) == 1) {
            a2(a3);
            a1.done = true;
        } else {
            while (!a1.done) {
                Blizzard::Process::Sleep(1);
            }
        }
    }
}

bool Blizzard::Lock::MutexCreate(Blizzard::Lock::Mutex& a1) {
    Blizzard::Lock::DoOnce(System_Lock::s_initMutexAttrOnce, System_Lock::InitAttr, 0);

    bool result = pthread_mutex_init(&a1, &System_Lock::s_mutexattr);

    // Blizzard::Debug::Assert(result == 0);

    return result;
}

bool Blizzard::Lock::MutexEnter(Blizzard::Lock::Mutex& a1) {
    bool result = pthread_mutex_lock(&a1);

    // Blizzard::Debug::Assert(result == 0);

    return result;
}

bool Blizzard::Lock::MutexLeave(Blizzard::Lock::Mutex& a1) {
    bool result = pthread_mutex_unlock(&a1);

    // Blizzard::Debug::Assert(result == 0);

    return result;
}

int32_t Blizzard::Lock::Atomic::Increment(volatile int32_t* value) {
#if defined(WHOA_SYSTEM_MAC)
    return OSAtomicAdd32(1, value);
#elif defined(WHOA_SYSTEM_LINUX)
    volatile std::atomic<int32_t> atom(*value);
    std::atomic_fetch_add<int32_t>(&atom, 1);
    return atom;
#endif
}

void Blizzard::System_Lock::InitAttr(void* a1) {
    pthread_mutexattr_init(&System_Lock::s_mutexattr);
    pthread_mutexattr_settype(&System_Lock::s_mutexattr, PTHREAD_MUTEX_RECURSIVE);
}

void Blizzard::Thread::AllocateLocalStorage(TLSSlot* slot) {
    System_Thread::AllocateLocalStorage(slot, 0);
}

void* Blizzard::Thread::RegisterLocalStorage(TLSSlot* slot, void* (*constructor)(void*), void* userData, void (*destructor)(void*)) {
    if (!System_Thread::TLSSlotIsAllocated(slot) && !System_Thread::AllocateTLSSlot(slot, destructor)) {
        // Blizzard::Debug::Assert(!"Unable to allocate thread-local storage");
    }

    void* value;

    value = System_Thread::InternalGetLocalStorage(slot);

    if (value) {
        return value;
    }

    value = constructor(userData);

    System_Thread::InternalSetLocalStorage(slot, value);

    return value;
}

void Blizzard::Thread::SetLocalStorage(const TLSSlot* slot, const void* value) {
    BLIZZARD_ASSERT(Blizzard::Thread::TLSSlotIsAllocated(slot));

    System_Thread::InternalSetLocalStorage(slot, value);
}

bool Blizzard::Thread::TLSSlotIsAllocated(const TLSSlot* slot) {
    return System_Thread::TLSSlotIsAllocated(slot);
}

void Blizzard::System_Thread::AddToRegistry(Thread::ThreadRecord* thread) {
    Blizzard::Lock::MutexEnter(System_Thread::s_registryMutex);

    if (!System_Thread::s_threadRegistry) {
        auto m = Blizzard::Memory::Allocate(sizeof(std::map<Thread::ThreadRecord*, Thread::ThreadRecord*>), 0, __FILE__, __LINE__, nullptr);
        System_Thread::s_threadRegistry = new (m) std::map<Thread::ThreadRecord*, Thread::ThreadRecord*>();
    }

    System_Thread::s_threadRegistry->insert(std::pair<Thread::ThreadRecord*, Thread::ThreadRecord*>(thread, thread));

    Blizzard::Lock::MutexLeave(System_Thread::s_registryMutex);
}

bool Blizzard::System_Thread::AllocateLocalStorage(Thread::TLSSlot* slot, void (*destructor)(void*)) {
    System_Thread::InitThreadSystem();

    BLIZZARD_ASSERT(!System_Thread::TLSSlotIsAllocated(slot));

    int32_t result;

    if (System_Thread::InternalAllocateLocalStorage(slot, destructor)) {
        slot->destructor = destructor;

        Blizzard::Lock::MutexEnter(System_Thread::s_mutex);

        int32_t v3 = System_Thread::s_slotListUsed;
        System_Thread::s_slotList[System_Thread::s_slotListUsed] = slot;
        System_Thread::s_slotListUsed = v3 + 1;

        Blizzard::Lock::MutexLeave(System_Thread::s_mutex);

        result = 1;
    } else {
        BLIZZARD_ASSERT(!"failed to allocate TLS");

        result = 0;
    }

    return result;
}

bool Blizzard::System_Thread::AllocateTLSSlot(Thread::TLSSlot* slot, void (*destructor)(void*)) {
    System_Thread::InitThreadSystem();

    Blizzard::Lock::MutexEnter(System_Thread::s_mutex);

    int32_t result = 0;

    if (System_Thread::TLSSlotIsAllocated(slot) || System_Thread::AllocateLocalStorage(slot, destructor)) {
        result = 1;
    }

    Blizzard::Lock::MutexLeave(System_Thread::s_mutex);

    return result;
}

void Blizzard::System_Thread::InitThreadSystem() {
    if (System_Thread::s_initialized) {
        return;
    }

    System_Thread::s_initialized = true;

    Blizzard::Lock::MutexCreate(System_Thread::s_mutex);
    Blizzard::Lock::MutexCreate(System_Thread::s_registryMutex);

    Blizzard::Thread::AllocateLocalStorage(&System_Thread::s_threadRecordTLS);
    Blizzard::Thread::AllocateLocalStorage(&System_Thread::s_stackTraceEntryPointTLS);

    System_Thread::s_mainThread = System_Thread::NewThread(nullptr, nullptr, nullptr);
    Blizzard::Thread::SetLocalStorage(&System_Thread::s_threadRecordTLS, System_Thread::s_mainThread);

    System_Thread::s_mainThread->unkC = 1;
    System_Thread::s_mainThread->unk10 = 1;
}

bool Blizzard::System_Thread::InternalAllocateLocalStorage(Thread::TLSSlot* slot, void (*destructor)(void*)) {
    if (!pthread_key_create(&slot->key, destructor)) {
        slot->allocated = true;
        return 1;
    } else {
        return 0;
    }
}

void* Blizzard::System_Thread::InternalGetLocalStorage(const Thread::TLSSlot* slot) {
    return pthread_getspecific(slot->key);
}

void Blizzard::System_Thread::InternalSetLocalStorage(const Thread::TLSSlot* slot, const void* value) {
    int32_t err = pthread_setspecific(slot->key, value);

    BLIZZARD_ASSERT(err == 0);
}

Blizzard::Thread::ThreadRecord* Blizzard::System_Thread::NewThread(uint32_t (*a1)(void*), void* a2, const char* a3) {
    const char* v3 = a3;

    if (!a3) {
        v3 = "";
    }

    int32_t v4 = Blizzard::String::Length(v3);

    Blizzard::Thread::ThreadRecord* thread = static_cast<Blizzard::Thread::ThreadRecord*>(
        Blizzard::Memory::Allocate(sizeof(Blizzard::Thread::ThreadRecord) + v4 + 1, 0, __FILE__, __LINE__, nullptr)
    );

    Blizzard::String::MemFill(thread, sizeof(Blizzard::Thread::ThreadRecord), 0);

    thread->unk4 = a2;
    thread->unk8 = a1;
    thread->unkC = 0;
    thread->unk10 = 2;

    Blizzard::String::Copy(thread->unk2C, v3, v4 + 1);

    System_Thread::AddToRegistry(thread);

    return thread;
}

bool Blizzard::System_Thread::TLSSlotIsAllocated(const Thread::TLSSlot* slot) {
    return slot->allocated;
}
#endif
