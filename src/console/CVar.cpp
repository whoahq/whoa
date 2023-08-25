#include "console/CVar.hpp"
#include "console/Command.hpp"
#include "console/Types.hpp"
#include "console/Line.hpp"
#include "util/SFile.hpp"

#include <bc/os/File.hpp>
#include <storm/String.hpp>

const char* s_filename = nullptr;
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

        ConsoleCommandRegister(name, CvarCommandHandler, CATEGORY(category), help);
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

bool CVar::Reset() {
    auto value = this->m_resetValue;
    if (value.GetString() == nullptr) {
        value = this->m_defaultValue;
    }

    this->InternalSet(value.GetString(), true, false, false, true);
    return true;
}

bool CVar::Default() {
    auto value = this->m_defaultValue;
    if (value.GetString() == nullptr) {
        value = this->m_resetValue;
    }

    this->InternalSet(value.GetString(), true, false, false, true);
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

static int32_t s_CreatePathDirectories(const char* szPath) {
    return true == OsCreateDirectory(szPath, 1);
}

int32_t CVar::Load(HOSFILE file) {
    char fastData[2048] = {0};
    char line[2048] = {0};

    auto size = OsGetFileSize(file);

    char* data = nullptr;

    if (0x1fff < size) {
        data = reinterpret_cast<char*>(SMemAlloc(size + 1, __FILE__, __LINE__, 0));
    } else {
        data = fastData;
    }

    auto grown = 0x1FFF >= size;

    int32_t result    = 0;
    size_t  bytesRead = 0;

    if (OsReadFile(file, data, size, &bytesRead) == 0) {
        result = 0;
    } else {
        data[size] = '\0';
        const char* curr = data;

        // Skip over UTF-8 byte order mark
        if ((((data != nullptr) && (2 < bytesRead)) && (data[0] == 0xef)) && ((data[1] == 0xbb && (data[2] == 0xbf)))) {
            curr = data + 3;
        }

        do {
            SStrTokenize(&curr, line, 0x800, "\r\n", 0);

            // Do not execute commands other than "set ..."
            if (SStrCmpI(line, "SET ", 4) == 0) {
                // Execute without adding to history
                ConsoleCommandExecute(line, 0);
            }

            result = 1;
        } while ((curr != nullptr) && (*curr != '\0'));
    }

    if (grown) {
        SMemFree(data, __FILE__, __LINE__, 0);
    }

    return result;
}

int32_t CVar::Load(const char* filename) {
    char path[STORM_MAX_PATH] = {0};

    auto file = OsCreateFile(filename, OS_GENERIC_READ, 0, OS_CREATE_NEW | OS_CREATE_ALWAYS, OS_FILE_ATTRIBUTE_NORMAL, 0x3f3f3f3f);

    if (file == HOSFILE_INVALID) {
        SStrPrintf(path, STORM_MAX_PATH, "WTF\\%s", filename);
        file = OsCreateFile(filename, OS_GENERIC_READ, 0, OS_CREATE_NEW | OS_CREATE_ALWAYS, OS_FILE_ATTRIBUTE_NORMAL, 0x3f3f3f3f);
        if (file == HOSFILE_INVALID) {
            return 0;
        }
    }

    auto result = CVar::Load(file);

    OsCloseFile(file);

    return result;
}

void CVar::Initialize(const char* filename) {
    STORM_ASSERT(filename);
    s_filename = filename;

    // Get data path
    char path[STORM_MAX_PATH] = {0};
    SFile::GetBasePath(path, STORM_MAX_PATH);
    SStrPrintf(path, STORM_MAX_PATH, "%s%s\\", path, "WTF");

    s_CreatePathDirectories(path);

    static ConsoleCommandList baseCommands[] = {
        { "set",          SetCommandHandler,         "Set the value of a CVar"                             },
        { "cvar_reset",   CvarResetCommandHandler,   "Set the value of a CVar to it's startup value"       },
        { "cvar_default", CvarDefaultCommandHandler, "Set the value of a CVar to it's coded default value" },
        { "cvarlist",     CvarListCommandHandler,    "List cvars"                                          }
    };

    CONSOLE_REGISTER_LIST(DEFAULT, baseCommands);

    CVar::Load(s_filename);
}

int32_t CvarResetCommandHandler(const char* command, const char* arguments) {
    char cvarName[256] = {0};
    auto string        = arguments;

    SStrTokenize(&string, cvarName, sizeof(cvarName), " ,;\t\"\r\n", nullptr);

    if (*cvarName) {
        // reset a specific cvar
        auto cvar = CVar::Lookup(cvarName);
        if (cvar) {
            cvar->Reset();
        } else {
            ConsoleWriteA("No such cvar \"%s\"\n", ERROR_COLOR, cvarName);
        }

        return 1;
    } else {
        // reset all cvars
        ConsoleWrite("Resetting all cvars\n", DEFAULT_COLOR);

        for (auto cvar = CVar::s_registeredCVars.Head(); cvar != nullptr; cvar = CVar::s_registeredCVars.Next(cvar)) {
            cvar->Reset();
        }

        return 1;
    }
}

int32_t CvarDefaultCommandHandler(const char* command, const char* arguments) {
    char cvarName[256] = {0};
    auto string        = arguments;

    SStrTokenize(&string, cvarName, sizeof(cvarName), " ,;\t\"\r\n", nullptr);

    if (*cvarName) {
        // restore a specific cvar
        auto cvar = CVar::Lookup(cvarName);
        if (cvar) {
            cvar->Default();
        } else {
            ConsoleWriteA("No such cvar \"%s\"\n", ERROR_COLOR, cvarName);
        }

        return 1;
    } else {
        // restore all cvars
        ConsoleWrite("Restoring all cvars\n", DEFAULT_COLOR);

        for (auto cvar = CVar::s_registeredCVars.Head(); cvar != nullptr; cvar = CVar::s_registeredCVars.Next(cvar)) {
            cvar->Default();
        }

        return 1;
    }
}

int32_t CvarCommandHandler(const char* command, const char* arguments) {
    auto cvar = CVar::Lookup(command);
    STORM_ASSERT(cvar);

    while (*arguments == ' ') {
        arguments++;
    }

    if (arguments[0] != '\0') {
        cvar->Set(arguments, true, true, false, false);
        return 1;
    }

    auto value = cvar->m_stringValue.GetString();

    ConsoleWriteA("CVar \"%s\" is \"%s\"", DEFAULT_COLOR, command, value ? value : "");
    return 1;
}

int32_t SetCommandHandler(const char* command, const char* arguments) {
    char cvarValue[2048] = {0};
    char cvarName[256]   = {0};

    auto str = arguments;

    SStrTokenize(&str, cvarName, sizeof(cvarName), " ,;\t\"\r\n", nullptr);
    SStrTokenize(&str, cvarValue, sizeof(cvarValue), " ,;\t\"\r\n", nullptr);

    auto var = CVar::Lookup(cvarName);
    if (var) {
        var->Set(cvarValue, true, false, false, true);
    } else {
        CVar::Register(cvarName, "", 0, cvarValue, nullptr, DEFAULT, true, nullptr, false);
    }

    return 1;
}

int32_t CvarListCommandHandler(const char* command, const char* arguments) {
    char text[256];
    char text2[256];

    for (auto cvar = CVar::s_registeredCVars.Head(); cvar != nullptr; cvar = CVar::s_registeredCVars.Next(cvar)) {
        SStrPrintf(text, sizeof(text), "  \"%s\" is \"%s\"", cvar->m_key.m_str, cvar->m_stringValue);

        if (cvar->m_defaultValue.GetString()) {
            if (SStrCmp(cvar->m_stringValue.GetString(), cvar->m_defaultValue.GetString(), STORM_MAX_STR)) {
                SStrPrintf(text2, sizeof(text2), " (default \"%s\")", cvar->m_defaultValue.GetString());
                SStrPack(text, text2, sizeof(text));
            }
        }

        if (cvar->m_resetValue.GetString()) {
            if (SStrCmp(cvar->m_stringValue.GetString(), cvar->m_resetValue.GetString(), STORM_MAX_STR)) {
                SStrPrintf(text2, sizeof(text2), " (reset \"%s\")", cvar->m_resetValue.GetString());
                SStrPack(text, text2, sizeof(text));
            }
        }

        ConsoleWrite(text, DEFAULT_COLOR);
    }

    return 1;
}
