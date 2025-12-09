#include "gx/font/CGxString.hpp"
#include "gx/Buffer.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/GxuFont.hpp"
#include "gx/font/Wrap.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <tempest/Math.hpp>
#include <algorithm>
#include <cmath>
#include <new>

TEXTLINETEXTURE* TEXTLINETEXTURE::NewTextLineTexture() {
    // TODO
    // Allocate off of TEXTLINETEXTURE::s_freeTextLineTextures

    auto m = SMemAlloc(sizeof(TEXTLINETEXTURE), __FILE__, __LINE__, 0x0);
    return new (m) TEXTLINETEXTURE();
}

void TEXTLINETEXTURE::Recycle(TEXTLINETEXTURE* ptr) {
    // TODO if (TEXTLINETEXTURE::s_recycledBytes <= 0x80000)

    if (ptr) {
        delete ptr;
    }
}

void TEXTLINETEXTURE::WriteGeometry(CGxVertexPCT* buf, const CImVector& fontColor, const C2Vector& shadowOffset, const CImVector& shadowColor, const C3Vector& viewTranslation, bool a7, bool a8, int32_t ofs, int32_t size) {
    if (!size || !this->m_vert.Count()) {
        return;
    }

    uint32_t colorCount = this->m_vert.Count() == this->m_colors.Count()
        ? this->m_colors.Count()
        : 0;

    if (ofs >= this->m_vert.Count()) {
        return;
    }

    uint32_t v24 = this->m_vert.Count() - ofs;

    if (size >= v24) {
        size = v24;
    }

    if (a7) {
        C3Vector shadowTranslation = {
            viewTranslation.x + floor(ScreenToPixelWidth(0, shadowOffset.x)),
            viewTranslation.y + floor(ScreenToPixelHeight(0, shadowOffset.y)),
            viewTranslation.z
        };

        auto color = colorCount ? this->m_colors[ofs] : fontColor;

        for (int32_t i = 0; i < size; i++) {
            auto& vert = this->m_vert[i + ofs];

            C3Vector p = {
                vert.vc.x + shadowTranslation.x,
                vert.vc.y + shadowTranslation.y,
                vert.vc.z + shadowTranslation.z
            };

            buf->p = p;
            buf->tc[0] = vert.tc;

            auto formattedShadowColor = shadowColor;
            if (a8 && colorCount) {
                formattedShadowColor.a = static_cast<uint8_t>(
                    (static_cast<float>(formattedShadowColor.a) * static_cast<float>(color.a)) / 65536.0f
                );
            }
            GxFormatColor(formattedShadowColor);
            buf->c = formattedShadowColor;

            buf++;
        }
    }

    // if (BATCHEDRENDERFONTDESC::s_billboarded) {
    //     // TODO
    // }

    for (int32_t i = 0; i < size; i++) {
        auto& vert = this->m_vert[i + ofs];

        auto color = colorCount ? this->m_colors[i + ofs] : fontColor;
        GxFormatColor(color);

        // if (BATCHEDRENDERFONTDESC::s_billboarded) {
        //     // TODO
        //     continue;
        // }

        C3Vector p = {
            vert.vc.x + viewTranslation.x,
            vert.vc.y + viewTranslation.y,
            vert.vc.z + viewTranslation.z
        };

        buf->p = p;
        buf->tc[0] = vert.tc;

        buf->c = color;

        buf++;
    }
}

CGxString* CGxString::GetNewString(int32_t linkOnList) {
    CGxString* string = g_freeStrings.Head();

    if (string) {
        g_strings.LinkToTail(string);
        return string;
    }

    auto m = SMemAlloc(sizeof(CGxString), __FILE__, __LINE__, 0x8);
    string = new (m) CGxString();

    if (linkOnList) {
        g_strings.LinkToTail(string);
    }

    return string;
}

CGxString::~CGxString() {
    // TODO
}

void CGxString::AddShadow(const C2Vector& offset, const CImVector& color) {
    this->m_shadowColor = color;
    this->m_shadowColor.a = std::min(this->m_shadowColor.a, this->m_fontColor.a);
    this->m_shadowOffset = offset;

    this->m_flags |= 0x1;

    if (this->m_flags & 0x20) {
        this->ClearInstanceData();
    }
}

uint32_t CGxString::CalculateVertsNeeded(int32_t line) {
    if (this->m_flags & 0x01) {
        return 2 * this->m_textLines[line]->m_vert.Count();
    }

    return this->m_textLines[line]->m_vert.Count();
}

bool CGxString::CheckGeometry() {
    if (this->m_textureEvicted) {
        if (!this->m_currentFace->CheckStringGlyphs(this->m_text)) {
            this->ClearInstanceData();
        }

        this->m_textureEvicted = 0;
    }

    this->CreateGeometry();
    this->m_intD4 = 0;

    return this->m_intB0 != 0;
}

void CGxString::ClearInstanceData() {
    // TODO this->m_hyperlinkInfo->SetCount(0);
    // TODO this->m_textureInfo->SetCount(0);

    // TODO this->dwordA4 = 0;
    this->m_lastGradientStart = -1;
    this->m_lastGradientLength = -1;
    this->m_intB0 = 0;

    for (int32_t i = 0; i < 8; i++) {
        if (this->m_textLines[i]) {
            TEXTLINETEXTURE::Recycle(this->m_textLines[i]);
            this->m_textLines[i] = nullptr;
        }
    }
}

void CGxString::CreateGeometry() {
    if (this->m_intB0 || !this->m_text || !*this->m_text) {
        return;
    }

    EMBEDDEDPARSEINFO info;
    uint32_t flags = this->m_flags;
    uint32_t numBytes = 0;
    const char* currentText = this->m_text;
    const char* nextText = nullptr;
    int32_t advance = 0;
    uint32_t wide = 0;
    int32_t gStart = this->m_lastGradientStart;
    int32_t gLength = this->m_lastGradientLength;
    CImVector lineColor = this->m_fontColor;
    float height = -this->m_currentFontHeight;
    C3Vector linePos = {
        0.0f,
        this->m_flags & 0x80 ? 0.0f : ScreenToPixelHeight(0, -this->m_currentFontHeight),
        0.0f
    };
    bool a10 = true;
    float blockWidth = this->m_blockWidth;

    auto v611 = static_cast<float>(CMath::fuint_pi(GetScreenPixelHeight() * this->m_spacing));
    auto v51 = ScreenToPixelHeight(0, this->m_currentFontHeight);
    auto spacing = this->m_flags & 0x80 ? this->m_spacing : v611 / static_cast<float>(GetScreenPixelHeight());
    auto v59 = this->m_flags & 0x80 ? this->m_currentFontHeight + this->m_spacing : v611 + v51;

    float v49 = 0.0f;

    while (*currentText) {
        if (this->m_blockHeight <= v49) {
            break;
        }

        if (this->m_flags & 0x2000 && this->m_intB0 == 1) {
            blockWidth -= GetIndentNormWidth();
        }

        v49 += this->m_currentFontHeight + spacing;

        QUOTEDCODE code = GxuDetermineQuotedCode(currentText, advance, 0, flags, wide);

        if (code == CODE_NEWLINE) {
            currentText += advance;
            this->m_intB0++;
            linePos.y -= v59;

            // TODO
            // info.dword0C -= v59;
            // info.dword04 -= v59;
            // info.dword34 -= v59;
            // info.dword2C -= v59;

            continue;
        }

        float extent = 0.0f;
        float a8 = 0.0f;
        float a11 = 0.0f;

        if (this->m_flags & 0x1) {
            a8 = this->m_shadowOffset.x;
        }

        CalcWrapPoint(
            this->m_currentFace,
            currentText,
            this->m_currentFontHeight,
            blockWidth,
            &numBytes,
            &extent,
            &nextText,
            a8,
            this->m_flags,
            &a10,
            &a11,
            this->m_scale
        );

        if (nextText == currentText || !nextText || (!numBytes && !*nextText)) {
            break;
        }

        if (this->m_horzJust == GxHJ_Right) {
            float indent = this->m_flags & 0x2000 && this->m_intB0 ? GetIndentPixelWidth() : 0.0f;
            linePos.x = ScreenToPixelWidth(this->m_flags & 0x80, -extent) - indent;
        } else if (this->m_horzJust == GxHJ_Center) {
            linePos.x = ScreenToPixelWidth(this->m_flags & 0x80, -(extent * 0.5f));
        } else if (this->m_flags & 0x2000 && this->m_intB0) {
            linePos.x = GetIndentPixelWidth();
        } else {
            linePos.x = 0.0f;
        }

        float offsetY = 0.0f;
        if (v51 < a11) {
            if (this->m_vertJust == GxVJ_Bottom) {
                offsetY = a11 - v51;
            } else if (this->m_vertJust == GxVJ_Middle) {
                offsetY = floor((a11 - v51) * 0.5);
            }
        }
        linePos.y -= offsetY;

        // TODO
        // info.dword0C -= v62;
        // info.dword04 -= v62;
        // info.dword34 = linePos.y;
        // info.dword2C = linePos.y + v59;
        // if (info.dword00) {
        //     info.dword08 = linePos.x;
        // }

        uint32_t texturePagesUsedFlag = 0;

        this->InitializeTextLine(currentText, numBytes, lineColor, linePos, &texturePagesUsedFlag, info);
        this->m_intB0++;

        this->m_texturePagesUsed |= texturePagesUsedFlag;

        currentText = nextText;

        linePos.y -= offsetY + v59;

        if (this->m_flags & 0x2) {
            break;
        }

        // TODO
        // info.dword0C -= offsetY + v59;
        // info.dword04 -= offsetY + v59;
        // info.dword34 = linePos.y;
    };

    this->InitializeViewTranslation();

    if (this->m_flags & 0x20 && (gStart != -1 || gLength != -1)) {
        this->SetGradient(gStart, gLength);
    }
}

int32_t CGxString::Initialize(float fontHeight, const C3Vector& position, float blockWidth, float blockHeight, CGxFont* face, const char* text, EGxFontVJusts vertJust, EGxFontHJusts horzJust, float spacing, uint32_t flags, const CImVector& color, float scale) {
    uint32_t textLen = SStrLen(text) + 1;
    if (textLen > this->m_textLen) {
        if (this->m_text) {
            SMemFree(this->m_text, __FILE__, __LINE__, 0x0);
        }

        this->m_textLen = textLen;
        this->m_text = static_cast<char*>(SMemAlloc(textLen, __FILE__, __LINE__, 0x0));
    }

    SStrCopy(this->m_text, text, this->m_textLen);

    this->m_blockWidth = blockWidth;
    this->m_blockHeight = blockHeight;
    this->m_spacing = spacing;
    this->m_position = position;
    this->m_horzJust = horzJust;
    this->m_vertJust = vertJust;
    this->m_flags = flags;
    this->m_fontColor = color;
    this->m_scale = scale;
    this->m_currentFace = face;

    face->m_strings.LinkToTail(this);

    float requestedFontHeight = this->m_flags & 0x4 && !(this->m_flags & 0x80)
        ? GxuFontGetOneToOneHeight(face)
        : fontHeight;
    this->m_requestedFontHeight = requestedFontHeight;

    this->m_currentFontHeight = std::max(this->m_requestedFontHeight, 2.0f / GetScreenPixelHeight());

    return 1;
}

void CGxString::InitializeTextLine(const char* currentText, uint32_t numBytes, CImVector& workingColor, const C3Vector& position, uint32_t* texturePagesUsedFlag, EMBEDDEDPARSEINFO& info) {
    if (this->m_flags & 0x08) {
        // TODO
    }

    C3Vector curPos = position;

    float screenPixelHeight = ScreenToPixelHeight(this->m_flags & 0x80, this->m_currentFontHeight);
    float glyphToScreenPixels = screenPixelHeight / this->m_currentFace->GetPixelSize();
    float glyphPixelHeight = ScreenToPixelHeight(this->m_flags & 0x80, this->m_currentFontHeight);

    if (this->m_currentFace->m_flags & 0x08) {
        glyphPixelHeight += 4.0f;
    } else if (this->m_currentFace->m_flags & 0x01) {
        glyphPixelHeight += 2.0f;
    }

    // TODO
    // - billboard adjustment table init
    // - EMBEDDEDPARSEINFO update

    float stepGlyph = 0.0f;
    float stepScreen = 0.0f;
    uint32_t prevCode = 0;
    CImVector color;

    while (numBytes && *currentText) {
        int32_t advance;
        uint32_t code;
        CImVector quotedColor;

        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(currentText, advance, &quotedColor, this->m_flags, code);

        currentText += advance;
        numBytes -= advance;

        if (prevCode) {
            if (this->m_flags & 0x10) {
                // TODO
                // stepGlyph = this->m_currentFace->ComputeStepFixedWidth(prevCode, code);
            } else {
                stepGlyph = this->m_currentFace->ComputeStep(prevCode, code);
            }
        }

        stepScreen = stepGlyph * glyphToScreenPixels;

        switch (quotedCode) {
            case CODE_COLORON:
                if (!(this->m_flags & 0x08)) {
                    color = quotedColor;
                    color.a = this->m_fontColor.a;
                }

                // TODO
                // EMBEDDEDPARSEINFO update

                continue;

            case CODE_COLORRESTORE:
                color = this->m_fontColor;

                continue;

            case CODE_NEWLINE:
                continue;

            case CODE_HYPERLINKSTART:
                // TODO

                continue;

            case CODE_HYPERLINKSTOP:
                // TODO

                continue;

            case CODE_TEXTURESTART:
                // TODO

                continue;

            case CODE_TEXTURESTOP:
                continue;

            default: {
                auto glyph = this->m_currentFace->NewCodeDesc(code);

                if (!glyph) {
                    glyph = this->m_currentFace->NewCodeDesc('?');
                }

                if (!glyph) {
                    continue;
                }

                *texturePagesUsedFlag |= 1 << glyph->textureNumber;

                if (!this->m_textLines[glyph->textureNumber]) {
                    this->m_textLines[glyph->textureNumber] = TEXTLINETEXTURE::NewTextLineTexture();
                }

                auto line = this->m_textLines[glyph->textureNumber];

                if (!(this->m_flags & 0x08)) {
                    uint32_t index = line->m_colors.Add(4, 0, &color);

                    if (this->m_flags & 0x20) {
                        // TODO
                    }
                }

                if (!(this->m_flags & 0x80)) {
                    stepScreen = floor(stepScreen + 0.5f);
                }

                curPos.x += stepScreen;

                VERT vert;
                vert.vc = { curPos.x, curPos.y, curPos.z };
                vert.tc = { 0.0f, 0.0f };

                vert.vc.x += this->m_currentFace->GetGlyphBearing(
                    glyph,
                    this->m_flags & 0x80,
                    this->m_currentFontHeight
                );

                if (this->m_currentFace->m_flags & 0x08) {
                    vert.vc.y -= 2.0f;
                } else if (this->m_currentFace->m_flags & 0x01) {
                    vert.vc.y -= 1.0f;
                }

                vert.vc.y += (glyph->bitmapData.m_yOffset * glyphToScreenPixels);

                uint32_t index = line->m_vert.Add(4, 0, &vert);
                VERT* verts = &line->m_vert[index];

                float width = this->m_flags & 0x80
                    ? glyph->bitmapData.m_glyphCellWidth * glyphToScreenPixels
                    : floor(glyph->bitmapData.m_glyphCellWidth * glyphToScreenPixels);
                float height = this->m_flags & 0x80
                    ? screenPixelHeight
                    : glyphPixelHeight;

                verts[0].vc.x = verts[1].vc.x;
                verts[3].vc.x = verts[3].vc.x + width;
                verts[2].vc.x = verts[3].vc.x;
                verts[3].vc.y = verts[3].vc.y + height;
                verts[1].vc.y = verts[3].vc.y;
                verts[0].vc.y = verts[2].vc.y;

                verts[2].tc.y = glyph->bitmapData.m_textureCoords.maxY;
                verts[0].tc.y = verts[2].tc.y;
                verts[3].tc.y = glyph->bitmapData.m_textureCoords.minY;
                verts[1].tc.y = verts[3].tc.y;
                verts[1].tc.x = glyph->bitmapData.m_textureCoords.minX;
                verts[0].tc.x = verts[1].tc.x;
                verts[3].tc.x = glyph->bitmapData.m_textureCoords.maxX;
                verts[2].tc.x = verts[3].tc.x;

                if (this->m_flags & 0x80) {
                    // TODO
                    // verts[3].tc.y += flt_C7D2E8;
                    // verts[1].tc.y += flt_C7D2E8;
                    // verts[1].tc.x += flt_C7D2EC;
                    // verts[0].tc.x += flt_C7D2EC;
                    // verts[2].tc.y += flt_C7D2F0;
                    // verts[0].tc.y += flt_C7D2F0;
                    // verts[3].tc.x += flt_C7D2F4;
                    // verts[2].tc.x += flt_C7D2F4;
                }

                prevCode = code;
            }
        }
    }

    // TODO
    // info.dword10 = curPos.x + stepScreen;
}

void CGxString::InitializeViewTranslation() {
    this->m_viewTranslation = this->m_position;

    if (!this->m_intB0 || this->m_flags & 0x80) {
        return;
    }

    if (this->m_horzJust == GxHJ_Right) {
        this->m_viewTranslation.x += this->m_blockWidth;
    } else if (this->m_horzJust == GxHJ_Center) {
        this->m_viewTranslation.x += (this->m_blockWidth * 0.5f);
    }

    float v13 = CMath::fuint_pi(this->m_spacing * static_cast<float>(GetScreenPixelHeight())) / GetScreenPixelHeight();
    float v24 = v13 * (float)(this->m_intB0 - 1) + (float)this->m_intB0 * this->m_currentFontHeight;

    if (this->m_intD4 >= 4) {
        // TODO
    }

    // TODO
    // - current font height logic

    if (this->m_vertJust == GxVJ_Bottom) {
        this->m_viewTranslation.y += v24;
    } else if (this->m_vertJust == GxVJ_Middle) {
        this->m_viewTranslation.y += 0.5f * (this->m_blockHeight - v24) + v24;
    } else {
        this->m_viewTranslation.y += this->m_blockHeight;
    }

    this->m_viewTranslation.x = floor((float)GetScreenPixelWidth() * this->m_viewTranslation.x);
    this->m_viewTranslation.y = floor((float)GetScreenPixelHeight() * this->m_viewTranslation.y);
}

void CGxString::Recycle() {
    this->m_batchedStringLink.Unlink();
    g_freeStrings.LinkToTail(this);
    this->ClearInstanceData();
}

void CGxString::SetColor(const CImVector& color) {
    if (this->m_fontColor == color) {
        return;
    }

    this->m_fontColor = color;
    this->m_shadowColor.a = std::min(color.a, this->m_shadowColor.a);

    if (!(this->m_flags & 0x8) || this->m_flags & 0x20 || this->m_intD4) {
        this->ClearInstanceData();
    }
}

int32_t CGxString::SetGradient(int32_t startCharacter, int32_t length) {
    // TODO
    return 0;
}

void CGxString::SetStringPosition(const C3Vector& position) {
    this->m_position = position;
    this->InitializeViewTranslation();
}

void CGxString::Tick() {
    if (this->m_intB0) {
        this->m_intD4++;

        if (this->m_intD4 - 1 > 30) {
            this->ClearInstanceData();
        }
    }
}

void CGxString::WriteGeometry(CGxVertexPCT* buf, int32_t line, int32_t ofs, int32_t size) {
    auto textLine = this->m_textLines[line];

    if (textLine) {
        textLine->WriteGeometry(
            buf,
            this->m_fontColor,
            this->m_shadowOffset,
            this->m_shadowColor,
            this->m_viewTranslation,
            this->m_flags & 0x01,
            this->m_flags & 0x20,
            ofs,
            size
        );
    }
}
