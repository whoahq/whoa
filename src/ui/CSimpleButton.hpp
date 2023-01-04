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
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

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
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void OnLayerHide();
        virtual void OnLayerCursorEnter(int32_t a2);
        virtual void OnLayerCursorExit(int32_t a2, int32_t a3);
        virtual int32_t OnLayerMouseDown(const CMouseEvent& evt, const char* btn);
        virtual int32_t OnLayerMouseUp(const CMouseEvent& evt, const char* btn);
        virtual void LockHighlight(int32_t lock);
        virtual void Enable(int32_t enabled);
        virtual void OnClick(const char* btn, int32_t a3);
        virtual void OnDoubleClick(const char* btn);
        virtual void SetButtonState(CSimpleButtonState state, int32_t stateLocked);

        // Member functions
        CSimpleButton(CSimpleFrame* parent);
        void RunOnClickScript(const char* btn, int32_t down);
        void SetClickAction(uint64_t action);
        void SetFontString(CSimpleFontString* text);
        void SetHighlight(CSimpleTexture* texture, EGxBlend blend);
        int32_t SetHighlight(const char* texFile, EGxBlend blendMode);
        void SetPressedOffset(C2Vector& offset);
        void SetStateTexture(CSimpleButtonState state, CSimpleTexture* texture);
        int32_t SetStateTexture(CSimpleButtonState state, const char* texFile);
        void SetText(const char* string);
        void UpdateTextState(CSimpleButtonState state);
};

#endif
