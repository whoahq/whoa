#ifndef UI_C_SIMPLE_BUTTON_HPP
#define UI_C_SIMPLE_BUTTON_HPP

#include "ui/CSimpleFrame.hpp"
#include <tempest/Vector.hpp>

class CSimpleFont;
class CSimpleFontString;
class CSimpleTexture;

enum CSimpleButtonState {
    BUTTONSTATE_DISABLED = 0x0,
    BUTTONSTATE_NORMAL = 0x1,
    BUTTONSTATE_PUSHED = 0x2,
    NUM_BUTTONSTATES = 0x3,
};

class CSimpleButton : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        uint32_t m_doubleClickTime = 0;
        uint64_t m_clickAction = 0x0000000080000000;
        CSimpleButtonState m_state = BUTTONSTATE_DISABLED;
        int32_t m_stateLocked = 0;
        CSimpleFontString* m_text = nullptr;
        CSimpleFont* m_normalFont = nullptr;
        CSimpleFont* m_highlightFont = nullptr;
        CSimpleFont* m_disabledFont = nullptr;
        C2Vector m_pressedOffset;
        CSimpleTexture* m_textures[NUM_BUTTONSTATES];
        CSimpleTexture* m_activeTexture = nullptr;
        CSimpleTexture* m_highlightTexture = nullptr;
        ScriptIx m_preClick;
        ScriptIx m_onClick;
        ScriptIx m_postClick;
        ScriptIx m_onDoubleClick;

        // Virtual member functions
        virtual ScriptIx* GetScriptByName(const char*, ScriptData&);
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual void OnLayerHide(void);
        virtual void OnLayerCursorEnter(int32_t);
        virtual void OnLayerCursorExit(int32_t, int32_t);
        virtual int32_t OnLayerMouseDown(const CMouseEvent&, const char*);
        virtual int32_t OnLayerMouseUp(const CMouseEvent&, const char*);
        virtual void LockHighlight(int32_t);
        virtual void Enable(int32_t);
        virtual void OnClick(const char*, int32_t);
        virtual void OnDoubleClick(const char*);
        virtual void SetButtonState(CSimpleButtonState, int32_t);

        // Member functions
        CSimpleButton(CSimpleFrame*);
        void RunOnClickScript(const char*, int32_t);
        void SetClickAction(uint64_t action);
        void SetFontString(CSimpleFontString*);
        void SetHighlight(CSimpleTexture*, EGxBlend);
        int32_t SetHighlight(const char* texFile, EGxBlend blendMode);
        void SetPressedOffset(C2Vector&);
        void SetStateTexture(CSimpleButtonState, CSimpleTexture*);
        int32_t SetStateTexture(CSimpleButtonState state, const char* texFile);
        void SetText(const char*);
        void UpdateTextState(CSimpleButtonState);
};

#endif
