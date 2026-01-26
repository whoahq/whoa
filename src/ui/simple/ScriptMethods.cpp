#include "ui/simple/ScriptMethods.hpp"
#include "ui/FrameXML.hpp"
#include "ui/Types.hpp"
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
#include "util/CStatus.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <common/XML.hpp>
#include <storm/String.hpp>
#include <cstdint>

int32_t Script_GetText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumFrames(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EnumerateFrames(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CreateFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CreateFrame(lua_State* L) {
    if (!lua_isstring(L, 1) || (lua_type(L, 3) != -1 && lua_type(L, 3) && lua_type(L, 3) != LUA_TTABLE)) {
        return luaL_error(L, "Usage: CreateFrame(\"frameType\" [, \"name\"] [, parent] [, \"template\"] [, id])");
    }

    const char* v21 = lua_tostring(L, 1);
    const char* v3 = lua_tostring(L, 2);
    const char* inherits = lua_tostring(L, 4);

    CSimpleFrame* parent = nullptr;

    // If parent argument is provided, ensure it exists and is of type CSimpleFrame
    if (lua_type(L, 3) == LUA_TTABLE) {
        lua_rawgeti(L, 3, 0);
        parent = static_cast<CSimpleFrame*>(lua_touserdata(L, -1));
        lua_settop(L, -2);

        if (!parent) {
            return luaL_error(L, "CreateFrame: Couldn't find 'this' in parent object");
        }

        int32_t frameType = CSimpleFrame::GetObjectType();

        if (!parent->IsA(frameType)) {
            return luaL_error(L, "CreateFrame: Wrong parent object type, expected frame");
        }
    }

    // If inherits argument is provided, ensure all inherited nodes exist
    if (lua_type(L, 4) == LUA_TSTRING) {
        inherits = lua_tostring(L, 4);

        char inheritName[1024];
        const char* unk1;
        bool unk2;

        do {
            SStrTokenize(&inherits, inheritName, 0x400u, " ,", 0);

            if (!*inheritName) {
                break;
            }

            if (!FrameXML_AcquireHashNode(inheritName, unk1, unk2)) {
                return luaL_error(L, "CreateFrame(): Couldn't find inherited node \"%s\"", inheritName);
            }

            if (unk2) {
                return luaL_error(L, "CreateFrame(): Recursively inherited node \"%s\"", inheritName);
            }

            FrameXML_ReleaseHashNode(inheritName);
        } while (*inheritName);

        inherits = lua_tostring(L, 4);
    }

    // Build an XMLNode simulating the arguments to CreateFrame()
    XMLNode frameNode(nullptr, v21);

    if (v3) {
        frameNode.SetAttribute("name", v3);
    }

    if (parent) {
        frameNode.SetAttribute("parent", "");
    }

    if (inherits) {
        frameNode.SetAttribute("inherits", inherits);
    }

    if (lua_isstring(L, 5)) {
        const char* idStr = lua_tostring(L, 5);
        frameNode.SetAttribute("id", idStr);
    } else if (lua_isnumber(L, 5)) {
        int32_t idNum = lua_tointeger(L, 5);

        char idStr[256];
        SStrPrintf(idStr, sizeof(idStr), "%d", idNum);

        frameNode.SetAttribute("id", idStr);
    }

    CStatus status;

    // Create an instance of CSimpleFrame or a descendant class
    auto frame = FrameXML_CreateFrame(&frameNode, parent, &status);

    if (!frame) {
        return luaL_error(L, "CreateFrame: Unknown frame type '%s'");
    }

    // Ensure the instance is registered within the Lua context
    if (!frame->lua_registered) {
        frame->RegisterScriptObject(0);
    }

    // Return a reference to the instance
    lua_rawgeti(L, LUA_REGISTRYINDEX, frame->lua_objectRef);

    return 1;
}

int32_t Script_GetFramesRegisteredForEvent(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCurrentKeyBoardFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetText",                        &Script_GetText },
    { "GetNumFrames",                   &Script_GetNumFrames },
    { "EnumerateFrames",                &Script_EnumerateFrames },
    { "CreateFont",                     &Script_CreateFont },
    { "CreateFrame",                    &Script_CreateFrame },
    { "GetFramesRegisteredForEvent",    &Script_GetFramesRegisteredForEvent },
    { "GetCurrentKeyBoardFocus",        &Script_GetCurrentKeyBoardFocus },
};

void RegisterSimpleFrameScriptMethods() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
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
