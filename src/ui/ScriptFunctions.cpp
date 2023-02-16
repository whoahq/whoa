#include "ui/ScriptFunctions.hpp"
#include "ui/CSimpleButton.hpp"
#include "ui/CSimpleCheckbox.hpp"
#include "ui/CSimpleEditBox.hpp"
#include "ui/CSimpleFont.hpp"
#include "ui/CSimpleFontString.hpp"
#include "ui/CSimpleFrame.hpp"
#include "ui/CSimpleHTML.hpp"
#include "ui/CSimpleModel.hpp"
#include "ui/CSimpleModelFFX.hpp"
#include "ui/CSimpleScrollFrame.hpp"
#include "ui/CSimpleSlider.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/FrameScript.hpp"

void CharacterCreateRegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_CHAR_CREATE; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_CharCreate[i].name,
            FrameScript::s_ScriptFunctions_CharCreate[i].method
        );
    }
}

void CharSelectRegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_CHAR_SELECT; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_CharSelect[i].name,
            FrameScript::s_ScriptFunctions_CharSelect[i].method
        );
    }
}

void GlueScriptEventsRegisterFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_GlueScriptEvents[i].name,
            FrameScript::s_ScriptFunctions_GlueScriptEvents[i].method
        );
    }
}

void RealmListRegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_REALM_LIST; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_RealmList[i].name,
            FrameScript::s_ScriptFunctions_RealmList[i].method
        );
    }
}

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

void SystemRegisterFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_SYSTEM; ++i) {
        FrameScript_RegisterFunction(
            FrameScript::s_ScriptFunctions_System[i].name,
            FrameScript::s_ScriptFunctions_System[i].method
        );
    }
}
