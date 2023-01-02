#ifndef UI_C_SIMPLE_EDIT_BOX_HPP
#define UI_C_SIMPLE_EDIT_BOX_HPP

#include "ui/CSimpleFrame.hpp"
#include "ui/CSimpleFontedFrame.hpp"

class CSimpleFontString;
class CSimpleTexture;

class CSimpleEditBox : public CSimpleFrame, CSimpleFontedFrame {
    public:
        // Static variables
        static CSimpleEditBox* s_currentFocus;
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);
        static void SetKeyboardFocus(CSimpleEditBox*);

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
        virtual ScriptIx* GetScriptByName(const char*, ScriptData&);
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual void OnLayerUpdate(float);
        virtual void OnFrameSizeChanged(float, float);
        virtual int32_t OnLayerChar(const CCharEvent&);
        virtual int32_t OnLayerKeyDown(const CKeyEvent&);
        virtual int32_t OnLayerMouseDown(const CMouseEvent&, const char*);
        virtual int32_t OnLayerMouseUp(const CMouseEvent&, const char*);
        virtual void FontUpdated(CSimpleFontedFrameFont*, int32_t);

        // Member functions
        CSimpleEditBox(CSimpleFrame*);
        int32_t ConvertCoordinateToIndex(float, float, int32_t&);
        void DeleteBackward(int32_t a2);
        void DeleteForward(int32_t a2);
        void DeleteHighlight(int32_t);
        void DeleteSubstring(int32_t left, int32_t right, int32_t a4);
        int32_t GetNumToLen(int32_t, int32_t, bool);
        void GrowText(int32_t);
        void Insert(uint32_t);
        void Insert(const char*, const char*, int32_t, int32_t, int32_t);
        int32_t IsCurrentFocus(void);
        void Move(int32_t distance, int32_t highlight);
        void MoveBackward(int32_t highlight);
        void MoveForward(int32_t highlight);
        void OnEnterPressed(void);
        void OnEscapePressed(void);
        void OnTextChanged(void);
        int32_t PrevCharOffset(int32_t);
        void RunOnCursorChangedScript(float, float, float, float);
        void RunOnEditFocusGainedScript(void);
        void RunOnEditFocusLostScript(void);
        void RunOnEnterPressedScript(void);
        void RunOnTextChangedScript(int32_t);
        void SetCursorPosition(int32_t);
        void SetHistoryLines(int32_t);
        void SetMultiLine(int32_t);
        void StartHighlight(void);
        void UpdateDirtyBits(void);
        void UpdateSizes(void);
        void UpdateTextInfo(void);
        void UpdateVisibleCursor(void);
        void UpdateVisibleHighlight(void);
        void UpdateVisibleText(void);
};

#endif
