#ifndef UI_C_SIMPLE_HTML_HPP
#define UI_C_SIMPLE_HTML_HPP

#include "ui/CSimpleFontedFrame.hpp"
#include "ui/CSimpleHyperlinkedFrame.hpp"
#include "ui/Types.hpp"
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
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        STORM_LIST(CONTENTNODE) m_content;
        CLayoutFrame* m_layoutAnchor;
        float m_layoutOffset;
        CSimpleFontedFrameFont* m_fonts[NUM_HTML_TEXT_TYPES];

        // Virtual member functions
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void FontUpdated(CSimpleFontedFrameFont* font, int32_t a3);

        // Member functions
        CSimpleHTML(CSimpleFrame*);
        void AddText(const char* text, HTML_TEXT_TYPE type, uint32_t justify);
        void ClearContent();
        void ParseBODY(XMLNode* node, CStatus* status);
        void ParseIMG(XMLNode* node, CStatus* status);
        void ParseP(XMLNode* node, HTML_TEXT_TYPE type, CStatus* status);
        bool SetText(const char* text, CStatus* status);
};

#endif
