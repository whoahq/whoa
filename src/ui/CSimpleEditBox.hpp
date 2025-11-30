#ifndef UI_C_SIMPLE_EDIT_BOX_HPP
#define UI_C_SIMPLE_EDIT_BOX_HPP

#include "ui/CSimpleFrame.hpp"
#include "ui/CSimpleFontedFrame.hpp"

class CObserver;
class CSimpleFontString;
class CSimpleTexture;

class CSimpleEditBox : public CSimpleFrame, CSimpleFontedFrame {
    public:
        // Enums
        enum {
            EVENT_ENTER             = 0,
            EVENT_ESCAPE            = 1,
            EVENT_SPACE             = 2,
            EVENT_TAB               = 3,
            EVENT_CHANGED           = 4,
            EVENT_SET               = 5,
            NUM_EDITBOX_ACTIONS,
        };

        // Static variables
        static CSimpleEditBox* s_currentFocus;
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);
        static void SetKeyboardFocus(CSimpleEditBox* editBox);

        // Member variables
        int32_t m_autoFocus : 1;
        int32_t m_multiline : 1;
        int32_t m_numeric : 1;
        int32_t m_password : 1;
        int32_t m_ignoreArrows : 1;
        int32_t m_countInvisibleLetters : 1;
        int32_t m_dirtyFlags = 0x1;
        CSimpleFontedFrameFont* m_font;
        CSimpleFontString* m_string;
        char* m_text;
        uint32_t* m_textInfo;
        char* m_textHidden = nullptr;
        int32_t m_textLength = 0;
        int32_t m_textLengthMax = -1;
        int32_t m_textLettersMax = 0;
        int32_t m_textSize = 32;
        int32_t m_visiblePos = 0;
        int32_t m_visibleLen = 0;
        CSimpleTexture* m_highlight[3];
        int32_t m_highlightLeft = 0;
        int32_t m_highlightRight = 0;
        int32_t m_highlightDrag = 0;
        CSimpleTexture* m_cursor = nullptr;
        int32_t m_cursorPos = 0;
        float m_cursorBlinkSpeed = 0.5f;
        float m_blinkElapsedTime = 0.0f;
        TSGrowableArray<uint32_t> m_visibleLines;
        struct {
            uint32_t id;
            CObserver* obj;
        } m_actions[NUM_EDITBOX_ACTIONS];
        int32_t m_imeInputMode = 0;
        CRect m_editTextInset = {};
        ScriptIx m_onEnterPressed;
        ScriptIx m_onEscapePressed;
        ScriptIx m_onSpacePressed;
        ScriptIx m_onTabPressed;
        ScriptIx m_onTextChanged;
        ScriptIx m_onTextSet;
        ScriptIx m_onCursorChanged;
        ScriptIx m_onInputLanguageChanged;
        ScriptIx m_onEditFocusGained;
        ScriptIx m_onEditFocusLost;
        ScriptIx m_onCharComposition;

        // Virtual member functions
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void OnLayerUpdate(float elapsedSec);
        virtual void OnFrameSizeChanged(float width, float height);
        virtual int32_t OnLayerKeyDownRepeat(const CKeyEvent& evt);
        virtual int32_t OnLayerChar(const CCharEvent& evt);
        virtual int32_t OnLayerKeyDown(const CKeyEvent& evt);
        virtual int32_t OnLayerMouseDown(const CMouseEvent& evt, const char* btn);
        virtual int32_t OnLayerMouseUp(const CMouseEvent& evt, const char* btn);
        virtual void FontUpdated(CSimpleFontedFrameFont* font, int32_t a3);

        // Member functions
        CSimpleEditBox(CSimpleFrame* parent);
        int32_t ConvertCoordinateToIndex(float a2, float a3, int32_t& a4);
        void DeleteBackward(int32_t a2);
        void DeleteForward(int32_t a2);
        void DeleteHighlight(int32_t a2);
        void DeleteSubstring(int32_t left, int32_t right, int32_t userInput);
        void DispatchAction(int32_t action);
        int32_t GetNumToLen(int32_t offset, int32_t amount, bool a4);
        void GrowText(int32_t size);
        void Insert(uint32_t chr);
        void Insert(const char* a2, const char* a3, int32_t a4, int32_t a5, int32_t a6);
        int32_t IsCurrentFocus();
        void Move(int32_t distance, int32_t highlight);
        void MoveBackward(int32_t highlight);
        void MoveForward(int32_t highlight);
        void OnEnterPressed();
        void OnEscapePressed();
        void OnTabPressed();
        void OnTextChanged();
        int32_t PrevCharOffset(int32_t offset);
        void RunOnCursorChangedScript(float x, float y, float w, float h);
        void RunOnEditFocusGainedScript();
        void RunOnEditFocusLostScript();
        void RunOnEnterPressedScript();
        void RunOnEscapePressedScript();
        void RunOnTabPressedScript();
        void RunOnTextChangedScript(int32_t changed);
        void RunOnTextSetScript(const char* a2);
        void SetCursorPosition(int32_t position);
        void SetHistoryLines(int32_t a2);
        void SetMultiLine(int32_t enabled);
        void SetText(const char* text, const char* a3);
        void StartHighlight();
        void UpdateDirtyBits();
        void UpdateSizes();
        void UpdateTextInfo();
        void UpdateVisibleCursor();
        void UpdateVisibleHighlight();
        void UpdateVisibleText();
};

#endif
