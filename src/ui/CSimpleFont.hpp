#ifndef UI_C_SIMPLE_FONT_HPP
#define UI_C_SIMPLE_FONT_HPP

#include "ui/CSimpleFontable.hpp"
#include "ui/CSimpleFontStringAttributes.hpp"
#include "ui/FrameScript_Object.hpp"
#include <cstdint>
#include <storm/Hash.hpp>
#include <storm/List.hpp>

class CSimpleFont;
class CStatus;
class XMLNode;

class SIMPLEFONT : public TSHashObject<SIMPLEFONT, HASHKEY_STRI> {
    public:
        // Member variables
        CSimpleFont* font = nullptr;

        // Member functions
        SIMPLEFONT();
};

class CSimpleFont : public FrameScript_Object, public CSimpleFontable {
    public:
        // Static variables
        static TSHashTable<SIMPLEFONT, HASHKEY_STRI> s_fontList;
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static CSimpleFont* GetFont(const char*, int32_t);
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        CSimpleFontStringAttributes m_attributes;
        const char* m_name = nullptr;
        STORM_EXPLICIT_LIST(CSimpleFontable, m_fontableLink) m_fontableList;

        // Virtual member functions
        virtual char* GetName(void);
        virtual int32_t GetScriptMetaTable(void);
        virtual bool IsA(int32_t);
        virtual void FontObjectUpdated(CSimpleFontStringAttributes&);

        // Member functions
        CSimpleFont();
        void LoadXML(XMLNode*, CStatus*);
        void UpdateObjects(void);
};

#endif
