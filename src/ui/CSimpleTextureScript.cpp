#include "ui/CSimpleTextureScript.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleTexture_IsObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetBlendMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetBlendMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetVertexColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetVertexColor(lua_State* L) {
    int32_t type = CSimpleTexture::GetObjectType();
    CSimpleTexture* texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    CImVector curColor = { 0 };
    texture->GetVertexColor(curColor);

    CImVector newColor = { 0 };
    FrameScript_GetColor(L, 2, newColor);
    if (!lua_isnumber(L, 5)) {
        newColor.a = curColor.a;
    }

    texture->SetVertexColor(newColor);

    return 0;
}

int32_t CSimpleTexture_SetGradient(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetGradientAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetAlpha(lua_State* L) {
    int32_t type = CSimpleTexture::GetObjectType();
    CSimpleTexture* texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        luaL_error(L, "Usage: %s:SetAlpha(alpha)", texture->GetDisplayName());
    }

    float alpha = lua_tonumber(L, 2);
    texture->SetAlpha(alpha);

    return 0;
}

int32_t CSimpleTexture_GetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_Show(lua_State* L) {
    int32_t type = CSimpleTexture::GetObjectType();
    CSimpleTexture* texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    texture->Show();

    return 0;
}

int32_t CSimpleTexture_Hide(lua_State* L) {
    int32_t type = CSimpleTexture::GetObjectType();
    CSimpleTexture* texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    texture->Hide();

    return 0;
}

int32_t CSimpleTexture_IsVisible(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_IsShown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetTexture(lua_State* L) {
    int32_t type = CSimpleTexture::GetObjectType();
    CSimpleTexture* texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    if (lua_isnumber(L, 2)) {
        // TODO
        // CArgb v9 = 0;
        // FrameScript_GetRGBA(2, L, &v9);
        // texture->SetTexture(&v9);

        lua_pushnumber(L, 1.0);

        return 1;
    }

    if (!lua_isstring(L, 2)) {
        texture->SetTexture(0, 0, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);

        lua_pushnumber(L, 1.0);

        return 1;
    }

    bool v5 = lua_toboolean(L, 3);
    bool v6 = lua_toboolean(L, 3);
    const char* v7 = lua_tostring(L, 2);

    if (texture->SetTexture(v7, v6, v5, CSimpleTexture::s_textureFilterMode, ImageMode_UI)) {
        lua_pushnumber(L, 1.0);

        return 1;
    }

    lua_pushnil(L);

    return 1;
}

int32_t CSimpleTexture_GetTexCoord(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetTexCoord(lua_State* L) {
    auto type = CSimpleTexture::GetObjectType();
    auto texture = static_cast<CSimpleTexture*>(FrameScript_GetObjectThis(L, type));

    auto argCount = lua_gettop(L) - 1;

    if (argCount == 4) {
        CRect texRect = {
            static_cast<float>(lua_tonumber(L, 4)),
            static_cast<float>(lua_tonumber(L, 2)),
            static_cast<float>(lua_tonumber(L, 5)),
            static_cast<float>(lua_tonumber(L, 3))
        };

        if (
            texRect.minX < -10000.0f
            || texRect.minX > 10000.0f
            || texRect.maxX < -10000.0f
            || texRect.maxX > 10000.0f
            || texRect.minY < -10000.0f
            || texRect.minY > 10000.0f
            || texRect.maxY < -10000.0f
            || texRect.maxY > 10000.0f
        ) {
            luaL_error(L, "TexCoord out of range");
        }

        texture->SetTexCoord(texRect);

    } else if (argCount == 8) {
        C2Vector texCoord[4];

        for (int32_t i = 0; i < 4; i++) {
            texCoord[i] = {
                static_cast<float>(lua_tonumber(L, 2 + (i * 2 + 0))),
                static_cast<float>(lua_tonumber(L, 2 + (i * 2 + 1)))
            };

            if (
                texCoord[i].x < -10000.0f
                || texCoord[i].x > 10000.0f
                || texCoord[i].y < -10000.0f
                || texCoord[i].y > 10000.0f
            ) {
                luaL_error(L, "TexCoord out of range");
            }
        }

        texture->SetTexCoord(texCoord);

    } else {
        luaL_error(L, "Usage: %s:SetTexCoord(minX, maxX, minY, maxY) or SetTexCoord(ULx, ULy, LLx, LLy, URx, URy, LRx, LRy)", texture->GetDisplayName());
    }

    return 0;
}

int32_t CSimpleTexture_SetRotation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetDesaturated(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_IsDesaturated(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetNonBlocking(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetNonBlocking(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetHorizTile(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetHorizTile(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_SetVertTile(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleTexture_GetVertTile(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleTextureMethods[NUM_SIMPLE_TEXTURE_SCRIPT_METHODS] = {
    { "IsObjectType",       &CSimpleTexture_IsObjectType },
    { "GetObjectType",      &CSimpleTexture_GetObjectType },
    { "GetDrawLayer",       &CSimpleTexture_GetDrawLayer },
    { "SetDrawLayer",       &CSimpleTexture_SetDrawLayer },
    { "GetBlendMode",       &CSimpleTexture_GetBlendMode },
    { "SetBlendMode",       &CSimpleTexture_SetBlendMode },
    { "GetVertexColor",     &CSimpleTexture_GetVertexColor },
    { "SetVertexColor",     &CSimpleTexture_SetVertexColor },
    { "SetGradient",        &CSimpleTexture_SetGradient },
    { "SetGradientAlpha",   &CSimpleTexture_SetGradientAlpha },
    { "SetAlpha",           &CSimpleTexture_SetAlpha },
    { "GetAlpha",           &CSimpleTexture_GetAlpha },
    { "Show",               &CSimpleTexture_Show },
    { "Hide",               &CSimpleTexture_Hide },
    { "IsVisible",          &CSimpleTexture_IsVisible },
    { "IsShown",            &CSimpleTexture_IsShown },
    { "GetTexture",         &CSimpleTexture_GetTexture },
    { "SetTexture",         &CSimpleTexture_SetTexture },
    { "GetTexCoord",        &CSimpleTexture_GetTexCoord },
    { "SetTexCoord",        &CSimpleTexture_SetTexCoord },
    { "SetRotation",        &CSimpleTexture_SetRotation },
    { "SetDesaturated",     &CSimpleTexture_SetDesaturated },
    { "IsDesaturated",      &CSimpleTexture_IsDesaturated },
    { "SetNonBlocking",     &CSimpleTexture_SetNonBlocking },
    { "GetNonBlocking",     &CSimpleTexture_GetNonBlocking },
    { "SetHorizTile",       &CSimpleTexture_SetHorizTile },
    { "GetHorizTile",       &CSimpleTexture_GetHorizTile },
    { "SetVertTile",        &CSimpleTexture_SetVertTile },
    { "GetVertTile",        &CSimpleTexture_GetVertTile }
};
