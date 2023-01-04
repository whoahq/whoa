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
        static void CreateScriptMetaTable();
        static CSimpleFont* GetFont(const char* name, int32_t a2);
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        CSimpleFontStringAttributes m_attributes;
        const char* m_name = nullptr;
        STORM_EXPLICIT_LIST(CSimpleFontable, m_fontableLink) m_fontableList;

        // Virtual member functions
        virtual char* GetName();
        virtual int32_t GetScriptMetaTable();
        virtual bool IsA(int32_t type);
        virtual void FontObjectUpdated(CSimpleFontStringAttributes& attributes);

        // Member functions
        CSimpleFont();
        void LoadXML(XMLNode* node, CStatus* status);
        void UpdateObjects();
};

#endif
