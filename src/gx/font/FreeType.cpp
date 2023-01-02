#include "gx/font/FreeType.hpp"
#include "gx/font/FreeTypeInternal.hpp"
#include "freetype/ftmodule.h"
#include <storm/Memory.hpp>

FT_Library g_FTLibrary;

FT_MemoryRec_ s_GxuMemoryRecord = {
    nullptr,
    &FreeTypeAllocFunction,
    &FreeTypeFreeFunction,
    &FreeTypeReallocFunction
};

void FreeTypeInitialize() {
    FT_New_Library(&s_GxuMemoryRecord, &g_FTLibrary);
    FT_Add_Default_Modules(g_FTLibrary);
}

int32_t FREETYPE_RenderGlyph(uint32_t code, bool monochrome, FT_Face face) {
    FT_UInt index = FT_Get_Char_Index(face, code);

    if (!index) {
        return 0;
    }

    FT_Int flags = monochrome
        ? FT_LOAD_NO_HINTING | FT_LOAD_CROP_BITMAP | FT_LOAD_PEDANTIC | FT_LOAD_LINEAR_DESIGN
        : FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP | FT_LOAD_PEDANTIC | FT_LOAD_LINEAR_DESIGN;

    if (FT_Load_Glyph(face, index, flags)) {
        return 0;
    }

    if (FT_Render_Glyph(face->glyph, monochrome ? ft_render_mode_mono : ft_render_mode_normal)) {
        return 0;
    }

    return 1;
}

FT_Library GetFreeTypeLibrary() {
    return g_FTLibrary;
};
