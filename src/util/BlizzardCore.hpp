#ifndef UTIL_BLIZZARD_CORE_HPP
#define UTIL_BLIZZARD_CORE_HPP

#include <cstdint>
#include <map>

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    #include <pthread.h>
#endif

#if defined(NDEBUG)
    #define BLIZZARD_ASSERT(x) if (!(x)) { return 0; }
#else
    #define BLIZZARD_ASSERT(x) if (!(x)) { Blizzard::Debug::Assert(#x, __FILE__, __LINE__); }
#endif

namespace Blizzard {
    namespace Memory {
        // Functions
        void* Allocate(uint32_t);
        void* Allocate(uint32_t, uint32_t, const char*, uint32_t, const char*);
        void Free(void*);
    };

    namespace String {
        // Functions
        int32_t Copy(char*, const char*, size_t);
        uint32_t Length(const char*);
        void MemFill(void*, uint32_t, unsigned char);
    };

    namespace Process {
        // Functions
        void Sleep(uint32_t);
    }

    namespace System_Debug {
        // Types
        typedef void (*AssertCallback)(const char*, const char*, const char*, uint32_t);

        // Variables
        extern AssertCallback s_assertCallback;
    }

    namespace Debug {
        // Functions
        void Assert(const char*, const char*, uint32_t);
        void SetAssertHandler(Blizzard::System_Debug::AssertCallback);
    }

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    namespace Lock {
        // Types
        typedef pthread_mutex_t Mutex;

        struct DoOnceData {
            bool done;
            int32_t atomic;
        };

        // Functions
        void DoOnce(DoOnceData&, void (*)(void*), void*);
        bool MutexCreate(Mutex&);
        bool MutexEnter(Mutex&);
        bool MutexLeave(Mutex&);

        namespace Atomic {
            int32_t Increment(volatile int32_t*);
        }
    }

    namespace System_Lock {
        // Types
        typedef pthread_mutexattr_t MutexAttr;

        // Variables
        extern Lock::DoOnceData s_initMutexAttrOnce;
        extern MutexAttr s_mutexattr;

        // Functions
        void InitAttr(void*);
    }

    namespace Thread {
        // Types
        struct ThreadRecord {
            pthread_t unk0;
            void* unk4;
            uint32_t (*unk8)(void*);
            int32_t unkC;
            int32_t unk10;
            char* unk2C;
        };

        struct TLSSlot {
            pthread_key_t key;
            void (*destructor)(void*);
            bool allocated;
        };

        // Functions
        void AllocateLocalStorage(TLSSlot*);
        void* RegisterLocalStorage(TLSSlot*, void* (*)(void*), void*, void (*)(void*));
        void SetLocalStorage(const TLSSlot*, const void*);
        bool TLSSlotIsAllocated(const TLSSlot*);
    };

    namespace System_Thread {
        // Variables
        extern bool s_initialized;
        extern Thread::ThreadRecord* s_mainThread;
        extern Lock::Mutex s_mutex;
        extern Lock::Mutex s_registryMutex;
        extern Thread::TLSSlot s_stackTraceEntryPointTLS;
        extern Thread::TLSSlot s_threadRecordTLS;
        extern std::map<Thread::ThreadRecord*, Thread::ThreadRecord*>* s_threadRegistry;
        extern Thread::TLSSlot* s_slotList[128];
        extern int32_t s_slotListUsed;

        // Functions
        void AddToRegistry(Thread::ThreadRecord*);
        bool AllocateLocalStorage(Thread::TLSSlot*, void (*)(void*));
        bool AllocateTLSSlot(Thread::TLSSlot*, void (*)(void*));
        void InitThreadSystem(void);
        bool InternalAllocateLocalStorage(Thread::TLSSlot*, void (*)(void*));
        void* InternalGetLocalStorage(const Thread::TLSSlot*);
        void InternalSetLocalStorage(const Thread::TLSSlot*, const void*);
        Thread::ThreadRecord* NewThread(uint32_t (*)(void*), void*, const char*);
        bool TLSSlotIsAllocated(const Thread::TLSSlot*);
    };
#endif
};

#endif
