#ifndef UI_SIMPLE_C_SIMPLE_HTML_HPP
#define UI_SIMPLE_C_SIMPLE_HTML_HPP

#include "ui/Types.hpp"
#include "ui/simple/CSimpleFontedFrame.hpp"
#include "ui/simple/CSimpleHyperlinkedFrame.hpp"
#include <storm/List.hpp>

class CStatus;
class XMLNode;

struct CONTENTNODE : TSLinkedNode<CONTENTNODE> {
    CSimpleFontString* string;

    ~CONTENTNODE();
};

class CSimpleHTML : public CSimpleHyperlinkedFrame, CSimpleFontedFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        STORM_LIST(CONTENTNODE) m_content;
        CLayoutFrame* m_layoutAnchor;
        float m_layoutOffset;
        CSimpleFontedFrameFont* m_fonts[NUM_HTML_TEXT_TYPES];

        // Virtual member functions
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(const XMLNode* node, CStatus* status);
        virtual void FontUpdated(CSimpleFontedFrameFont* font, int32_t a3);

        // Member functions
        CSimpleHTML(CSimpleFrame* parent);
        void AddText(const char* text, HTML_TEXT_TYPE type, uint32_t justify);
        void ClearContent();
        void ParseBODY(const XMLNode* node, CStatus* status);
        void ParseIMG(const XMLNode* node, CStatus* status);
        void ParseP(const XMLNode* node, HTML_TEXT_TYPE type, CStatus* status);
        bool SetText(const char* text, CStatus* status);
};

#endif
