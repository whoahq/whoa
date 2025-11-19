#ifndef CONSOLE_C_VAR_HPP
#define CONSOLE_C_VAR_HPP

#include "console/Types.hpp"
#include <common/String.hpp>
#include <storm/Hash.hpp>
#include <cstdint>

class CVar : public TSHashObject<CVar, HASHKEY_STRI> {
    public:
        // Static variables
        static bool m_initialized;
        static TSHashTable<CVar, HASHKEY_STRI> s_registeredCVars;
        static bool m_needsSave;

        // Static functions
        static void Initialize();
        static int32_t Load(const char* filename);
        static CVar* Lookup(const char* name);
        static CVar* LookupRegistered(const char* name);
        static CVar* Register(const char* name, const char* help, uint32_t flags, const char* value, bool (*fcn)(CVar*, const char*, const char*, void*), uint32_t category, bool a7, void* arg, bool a9);

        // Member variables
        uint32_t m_category = 0;
        uint32_t m_flags = 0;
        RCString m_stringValue;
        float m_floatValue = 0.0;
        int32_t m_intValue = 0;
        int32_t m_modified = 0;
        RCString m_defaultValue;
        RCString m_resetValue;
        RCString m_latchedValue;
        RCString m_help;
        bool (*m_callback)(CVar*, const char*, const char*, void*) = nullptr;
        void* m_arg = nullptr;

        // Member functions
        CVar();
        const char* GetDefaultValue();
        float GetFloat();
        int32_t GetInt();
        const char* GetString();
        void InternalSet(const char*, bool, bool, bool, bool);
        bool Set(const char*, bool, bool, bool, bool);
        int32_t Update();
};

#endif
