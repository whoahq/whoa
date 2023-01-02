#ifndef UI_C_SCRIPT_OBJECT_HPP
#define UI_C_SCRIPT_OBJECT_HPP

#include "ui/FrameScript_Object.hpp"
#include <cstdint>
#include <common/String.hpp>

class CStatus;
class XMLNode;

class CScriptObject : public FrameScript_Object {
    public:
        // Static variables
        static int32_t s_objectType;
        static const char* s_objectTypeName;

        // Static functions
        static void RegisterScriptMethods(lua_State*);
        static CScriptObject* GetScriptObjectByName(const char*, int32_t);

        // Member variables
        RCString m_name;

        // Virtual member functions
        virtual ~CScriptObject(void);
        virtual char* GetName(void);
        virtual bool IsA(int32_t);
        virtual CScriptObject* GetScriptObjectParent(void) = 0;
        virtual bool IsA(const char*);
        virtual const char* GetObjectTypeName(void);

        // Member functions
        void CreateName(const char*, char*, uint32_t);
        void PreLoadXML(XMLNode*, CStatus*);
        void SetName(const char*);
};

#endif
