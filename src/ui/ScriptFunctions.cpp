#include "ui/ScriptFunctions.hpp"
#include "ui/FrameScript.hpp"
#include "ui/simple/CSimpleButton.hpp"
#include "ui/simple/CSimpleCheckbox.hpp"
#include "ui/simple/CSimpleEditBox.hpp"
#include "ui/simple/CSimpleFont.hpp"
#include "ui/simple/CSimpleFontString.hpp"
#include "ui/simple/CSimpleFrame.hpp"
#include "ui/simple/CSimpleHTML.hpp"
#include "ui/simple/CSimpleModel.hpp"
#include "ui/simple/CSimpleModelFFX.hpp"
#include "ui/simple/CSimpleScrollFrame.hpp"
#include "ui/simple/CSimpleSlider.hpp"
#include "ui/simple/CSimpleTexture.hpp"

void RegisterSimpleFrameScriptMethods() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_SimpleFrame[i].name,
            FrameScript::s_ScriptFunctions_SimpleFrame[i].method
        );
    }

    // TODO
    // CSimpleAnim::CreateScriptMetaTable();
    // CSimpleTranslationAnim::CreateScriptMetaTable();
    // CSimpleRotationAnim::CreateScriptMetaTable();
    // CSimpleScaleAnim::CreateScriptMetaTable();
    // CSimpleControlPoint::CreateScriptMetaTable();
    // CSimplePathAnim::CreateScriptMetaTable();
    // CSimpleAlphaAnim::CreateScriptMetaTable();
    // CSimpleAnimGroup::CreateScriptMetaTable();

    CSimpleFont::CreateScriptMetaTable();
    CSimpleTexture::CreateScriptMetaTable();
    CSimpleFontString::CreateScriptMetaTable();
    CSimpleFrame::CreateScriptMetaTable();
    CSimpleButton::CreateScriptMetaTable();
    CSimpleCheckbox::CreateScriptMetaTable();
    CSimpleEditBox::CreateScriptMetaTable();
    CSimpleHTML::CreateScriptMetaTable();

    // TODO
    // CSimpleMessageFrame::CreateScriptMetaTable();
    // CSimpleMessageScrollFrame::CreateScriptMetaTable();

    CSimpleModel::CreateScriptMetaTable();
    CSimpleModelFFX::CreateScriptMetaTable();
    CSimpleScrollFrame::CreateScriptMetaTable();
    CSimpleSlider::CreateScriptMetaTable();

    // TODO
    // CSimpleStatusBar::CreateScriptMetaTable();
    // CSimpleColorSelect::CreateScriptMetaTable();
    // CSimpleMovieFrame::CreateScriptMetaTable();
}
