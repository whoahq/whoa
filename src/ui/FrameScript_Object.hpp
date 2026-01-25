#ifndef UI_FRAME_SCRIPT_OBJECT_HPP
#define UI_FRAME_SCRIPT_OBJECT_HPP

#include "ui/FrameScript.hpp"
#include <cstdint>

class FrameScript_Object {
    public:
        // Structs
        struct ScriptData {
            const char* wrapper;
        };

        struct ScriptIx {
            int32_t luaRef = 0;
            const char* unk = nullptr;
            ~ScriptIx();
        };

        class ScriptFunction {
            public:
                int32_t luaRef = 0;
        };

        // Static members
        static int32_t s_objectTypes;

        // Static functions
        static int32_t CreateScriptMetaTable(lua_State* L, void(*a2)(lua_State*));
        static void FillScriptMethodTable(lua_State* L, FrameScript_Method methods[], int32_t count);

        // Member variables
        int32_t lua_registered = 0;
        int32_t lua_objectRef = -2;
        ScriptIx m_onEvent;

        // Virtual member functions
        virtual ~FrameScript_Object() = default;
        virtual char* GetName() = 0;
        virtual int32_t GetScriptMetaTable() = 0;
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type) = 0;

        // Member functions
        const char* GetDisplayName();
        int32_t RegisterScriptEvent(const char* name);
        void RegisterScriptObject(const char* name);
        void RunScript(ScriptIx const& script, int32_t argCount, const char* a4);
        int32_t SetScript(lua_State* L);
        void UnregisterScriptObject(const char* name);
};

FrameScript_Object* FrameScript_GetObjectThis(lua_State* L, int32_t type);

#endif
