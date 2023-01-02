#include "util/CVar.hpp"
#include <storm/String.hpp>

bool CVar::m_needsSave;
TSHashTable<CVar, HASHKEY_STRI> CVar::s_registeredCVars;

CVar* CVar::Lookup(const char* name) {
    return name
        ? CVar::s_registeredCVars.Ptr(name)
        : nullptr;
}

CVar* CVar::Register(const char* name, const char* help, uint32_t flags, const char* value, bool (*fcn)(CVar*, const char*, const char*, void*), uint32_t category, bool a7, void* arg, bool a9) {
    CVar* var = CVar::s_registeredCVars.Ptr(name);

    if (var) {
        bool setReset = var->m_resetValue.GetString() == nullptr;
        bool setDefault = var->m_defaultValue.GetString() == nullptr;

        var->m_flags |= (var->m_flags & 0xFFFFFFCF);

        var->m_callback = fcn;
        var->m_arg = arg;

        bool setValue = false;
        if (fcn && !fcn(var, var->GetString(), var->GetString(), arg)) {
            setValue = true;
        }

        var->Set(value, setValue, setReset, setDefault, false);

        if (!a7) {
            var->m_flags |= 0x80000000;
        }

        if (a9 && var->m_flags) {
            var->m_flags |= 0x80;
        }
    } else {
        var = CVar::s_registeredCVars.New(name, 0, 0);

        var->m_stringValue.Copy(nullptr);
        var->m_floatValue = 0.0f;
        var->m_intValue = 0;
        var->m_modified = 0;
        var->m_category = category;
        var->m_defaultValue.Copy(nullptr);
        var->m_resetValue.Copy(nullptr);
        var->m_latchedValue.Copy(nullptr);
        var->m_callback = fcn;
        var->m_flags = 0;
        var->m_arg = arg;
        var->m_help.Copy(help);

        if (a7) {
            var->Set(value, true, true, false, false);
        } else {
            var->Set(value, true, false, true, false);
        }

        var->m_flags = flags | 0x1;

        if (!a7) {
            var->m_flags |= 0x8000000;
        }

        if (a9 && var->m_flags) {
            var->m_flags |= 0x80;
        }

        // TODO
        // ConsoleCommandRegister(var->m_key.GetString(), &CvarCommandHandler, category, help);
    }

    return var;
}

CVar::CVar() : TSHashObject<CVar, HASHKEY_STRI>() {
    // TODO
}

int32_t CVar::GetInt() {
    return this->m_intValue;
}

const char* CVar::GetString() {
    return this->m_stringValue.GetString();
}

void CVar::InternalSet(const char* value, bool setValue, bool setReset, bool setDefault, bool a6) {
    if (this->m_flags & 0x4 || !value) {
        return;
    }

    bool modified = false;

    const char* existingValue = this->m_stringValue.GetString();

    if (setValue && (!existingValue || SStrCmpI(value, existingValue, 0x7FFFFFFF))) {
        modified = true;

        this->m_stringValue.Copy(value);
        this->m_intValue = SStrToInt(value);
        this->m_floatValue = SStrToFloat(value);
    }

    if (setReset && !this->m_resetValue.GetString()) {
        modified = true;

        this->m_resetValue.Copy(value);
    }

    if (setDefault && !this->m_defaultValue.GetString()) {
        this->m_defaultValue.Copy(value);
    } else if (!modified) {
        return;
    }

    if (a6) {
        CVar::m_needsSave = 1;
    }
}

bool CVar::Set(const char* value, bool setValue, bool setReset, bool setDefault, bool a6) {
    if (setValue) {
        if (this->m_callback) {
            // TODO
            // sub_86B5A0(this->m_callback);

            if (!this->m_callback(this, this->m_stringValue.GetString(), value, this->m_arg)) {
                return true;
            }
        }

        this->m_modified++;

        if (this->m_flags & 0x2) {
            this->m_latchedValue.Copy(value);
            CVar::m_needsSave = 1;

            return true;
        }
    }

    this->InternalSet(value, setValue, setReset, setDefault, a6);

    return true;
}

int32_t CVar::Update() {
    if (!(this->m_flags & 0x2)) {
        return 0;
    }

    if (!this->m_latchedValue.GetString()) {
        return 0;
    }

    this->InternalSet(this->m_latchedValue.GetString(), true, false, false, true);
    this->m_latchedValue.Copy(nullptr);

    return 1;
}
