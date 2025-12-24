#include "ui/CBackdropGenerator.hpp"
#include "ui/simple/CSimpleFrame.hpp"
#include "ui/simple/CSimpleTexture.hpp"
#include "ui/LoadXML.hpp"
#include "ui/Util.hpp"
#include "util/CStatus.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>
#include <storm/Error.hpp>

CBackdropGenerator::CBackdropGenerator() {
    this->m_color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    this->m_borderColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
}

void CBackdropGenerator::Generate(const CRect* rect) {
    STORM_ASSERT(rect);

    C2Vector texCoords[4];

    float v3 = rect->maxX - rect->minX;
    float v7 = rect->maxY - rect->minY;

    float v4 = 1.0f / this->m_cornerSize;
    float v5 = v4 * v3 - 2.0f;
    float v9 = v4 * v7 - 2.0f;

    float v34 = v5 < 0.0f ? 0.0f : v5;
    float v17 = v9 < 0.0f ? 0.0f : v9;

    const char* background = this->m_background.GetString();

    if (background && *background && this->m_tileBackground) {
        float v13 = this->m_backgroundSize == 0.0f
            ? this->m_cornerSize
            : this->m_backgroundSize;

        texCoords[0] = { 0.0f, 0.0f };
        texCoords[1] = { 0.0f, v7 / v13 };
        texCoords[2] = { v3 / v13, 0.0f };
        texCoords[3] = { v3 / v13, v7 / v13 };

        this->m_backgroundTexture->SetTexCoord(texCoords);
    }

    if (this->m_pieces & 0x1) {
        texCoords[0] = { 0.0078125f, 0.0625f };
        texCoords[1] = { 0.0078125f, v17 - 0.0625f };
        texCoords[2] = { 0.1171875f, 0.0625f };
        texCoords[3] = { 0.1171875f, v17 - 0.0625f };

        this->m_leftTexture->SetTexCoord(texCoords);
    }

    if (this->m_pieces & 0x2) {
        texCoords[0] = { 0.1328125f, 0.0625f };
        texCoords[1] = { 0.1328125f, v17 - 0.0625f };
        texCoords[2] = { 0.2421875f, 0.0625f };
        texCoords[3] = { 0.2421875f, v17 - 0.0625f };

        this->m_rightTexture->SetTexCoord(texCoords);
    }

    if (this->m_pieces & 0x4) {
        texCoords[0] = { 0.2578125f, v34 - 0.0625f };
        texCoords[1] = { 0.3671875f, v34 - 0.0625f };
        texCoords[2] = { 0.2578125f, 0.0625f };
        texCoords[3] = { 0.3671875f, 0.0625f };

        this->m_topTexture->SetTexCoord(texCoords);
    }

    if (this->m_pieces & 0x8) {
        texCoords[0] = { 0.3828125f, v34 - 0.0625f };
        texCoords[1] = { 0.4921875f, v34 - 0.0625f };
        texCoords[2] = { 0.3828125f, 0.0625f };
        texCoords[3] = { 0.4921875f, 0.0625f };

        this->m_bottomTexture->SetTexCoord(texCoords);
    }

    if (this->m_backgroundTexture) {
        this->m_backgroundTexture->SetVertexColor(this->m_color);
    }

    this->SetBorderVertexColor(this->m_borderColor);
}

void CBackdropGenerator::LoadXML(const XMLNode* node, CStatus* status) {
    const char* bgFileAttr = node->GetAttributeByName("bgFile");
    const char* edgeFileAttr = node->GetAttributeByName("edgeFile");
    const char* tileAttr = node->GetAttributeByName("tile");
    const char* alphaModeAttr = node->GetAttributeByName("alphaMode");

    this->m_background.Copy(bgFileAttr);
    this->m_border.Copy(edgeFileAttr);

    this->m_tileBackground = tileAttr && *tileAttr
        ? StringToBOOL(tileAttr)
        : 0;

    this->m_pieces = edgeFileAttr && *edgeFileAttr
        ? 0xFF
        : 0;

    this->m_blend = GxBlend_Alpha;
    if (alphaModeAttr && *alphaModeAttr) {
        StringToBlendMode(alphaModeAttr, this->m_blend);
    }

    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "TileSize", STORM_MAX_STR)) {
            float backgroundSize;

            if (LoadXML_Value(child, backgroundSize, status)) {
                this->m_backgroundSize = backgroundSize;
            }

        } else if (!SStrCmpI(child->GetName(), "EdgeSize", STORM_MAX_STR)) {
            float cornerSize;

            if (LoadXML_Value(child, cornerSize, status)) {
                this->m_cornerSize = cornerSize;
            }

        } else if (!SStrCmpI(child->GetName(), "BackgroundInsets", STORM_MAX_STR)) {
            float leftInset, rightInset, topInset, bottomInset;

            if (LoadXML_Insets(child, leftInset, rightInset, topInset, bottomInset, status)) {
                this->m_leftInset = leftInset;
                this->m_rightInset = rightInset;
                this->m_topInset = topInset;
                this->m_bottomInset = bottomInset;
            }

        } else if (!SStrCmpI(child->GetName(), "Color", STORM_MAX_STR)) {
            // TODO

        } else if (!SStrCmpI(child->GetName(), "BorderColor", STORM_MAX_STR)) {
            CImVector borderColor = { 0 };
            LoadXML_Color(child, borderColor);
            this->SetBorderVertexColor(borderColor);

        } else {
            status->Add(
                STATUS_WARNING,
                "Unknown child node in %s element: %s",
                node->GetName(),
                child->GetName()
            );
        }
    }
}

void CBackdropGenerator::SetBorderVertexColor(const CImVector& borderColor) {
    this->m_borderColor = borderColor;

    if (this->m_leftTexture) {
        this->m_leftTexture->SetVertexColor(borderColor);
    }

    if (this->m_rightTexture) {
        this->m_rightTexture->SetVertexColor(borderColor);
    }

    if (this->m_topTexture) {
        this->m_topTexture->SetVertexColor(borderColor);
    }

    if (this->m_bottomTexture) {
        this->m_bottomTexture->SetVertexColor(borderColor);
    }

    if (this->m_topLeftTexture) {
        this->m_topLeftTexture->SetVertexColor(borderColor);
    }

    if (this->m_topRightTexture) {
        this->m_topRightTexture->SetVertexColor(borderColor);
    }

    if (this->m_bottomLeftTexture) {
        this->m_bottomLeftTexture->SetVertexColor(borderColor);
    }

    if (this->m_bottomRightTexture) {
        this->m_bottomRightTexture->SetVertexColor(borderColor);
    }
}

void CBackdropGenerator::SetOutput(CSimpleFrame* frame) {
    STORM_ASSERT(frame);

    C2Vector texCoords[4];

    const char* background = this->m_background.GetString();
    const char* border = this->m_border.GetString();

    if (background && *background) {
        // TODO
        // CSimpleTexture* backgroundTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto backgroundTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND, 1);

        this->m_backgroundTexture = backgroundTexture;

        backgroundTexture->SetPoint(FRAMEPOINT_TOPLEFT, frame, FRAMEPOINT_TOPLEFT, this->m_leftInset, -this->m_topInset, 0);
        backgroundTexture->SetPoint(FRAMEPOINT_TOPRIGHT, frame, FRAMEPOINT_TOPRIGHT, -this->m_rightInset, -this->m_topInset, 0);
        backgroundTexture->SetPoint(FRAMEPOINT_BOTTOMLEFT, frame, FRAMEPOINT_BOTTOMLEFT, this->m_leftInset, this->m_bottomInset, 0);
        backgroundTexture->SetPoint(FRAMEPOINT_BOTTOMRIGHT, frame, FRAMEPOINT_BOTTOMRIGHT, -this->m_rightInset, this->m_topInset, 0);

        backgroundTexture->Resize(0);

        backgroundTexture->SetTexture(background, this->m_tileBackground, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
    }

    if (this->m_pieces & 0x1) {
        // TODO
        // CSimpleTexture* leftTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto leftTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_leftTexture = leftTexture;

        leftTexture->SetWidth(this->m_cornerSize);
        leftTexture->SetPoint(FRAMEPOINT_TOPLEFT, frame, FRAMEPOINT_TOPLEFT, 0.0f, -this->m_cornerSize, 0);
        leftTexture->SetPoint(FRAMEPOINT_BOTTOMLEFT, frame, FRAMEPOINT_BOTTOMLEFT, 0.0f, this->m_cornerSize, 0);

        leftTexture->Resize(0);

        leftTexture->SetTexture(border, 1, 1, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        leftTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x2) {
        // TODO
        // CSimpleTexture* rightTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto rightTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_rightTexture = rightTexture;

        rightTexture->SetWidth(this->m_cornerSize);
        rightTexture->SetPoint(FRAMEPOINT_TOPRIGHT, frame, FRAMEPOINT_TOPRIGHT, 0.0f, -this->m_cornerSize, 0);
        rightTexture->SetPoint(FRAMEPOINT_BOTTOMRIGHT, frame, FRAMEPOINT_BOTTOMRIGHT, 0.0f, this->m_cornerSize, 0);

        rightTexture->Resize(0);

        rightTexture->SetTexture(border, 1, 1, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        rightTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x4) {
        // TODO
        // CSimpleTexture* topTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto topTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_topTexture = topTexture;

        topTexture->SetHeight(this->m_cornerSize);
        topTexture->SetPoint(FRAMEPOINT_TOPLEFT, frame, FRAMEPOINT_TOPLEFT, this->m_cornerSize, 0.0f, 0);
        topTexture->SetPoint(FRAMEPOINT_TOPRIGHT, frame, FRAMEPOINT_TOPRIGHT, -this->m_cornerSize, 0.0f, 0);

        topTexture->Resize(0);

        topTexture->SetTexture(border, 1, 1, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        topTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x8) {
        // TODO
        // CSimpleTexture* bottomTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto bottomTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_bottomTexture = bottomTexture;

        bottomTexture->SetHeight(this->m_cornerSize);
        bottomTexture->SetPoint(FRAMEPOINT_BOTTOMLEFT, frame, FRAMEPOINT_BOTTOMLEFT, this->m_cornerSize, 0.0f, 0);
        bottomTexture->SetPoint(FRAMEPOINT_BOTTOMRIGHT, frame, FRAMEPOINT_BOTTOMRIGHT, -this->m_cornerSize, 0.0f, 0);

        bottomTexture->Resize(0);

        bottomTexture->SetTexture(border, 1, 1, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        bottomTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x10) {
        // TODO
        // CSimpleTexture* topLeftTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto topLeftTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_topLeftTexture = topLeftTexture;

        topLeftTexture->SetWidth(this->m_cornerSize);
        topLeftTexture->SetHeight(this->m_cornerSize);
        topLeftTexture->SetPoint(FRAMEPOINT_TOPLEFT, frame, FRAMEPOINT_TOPLEFT, 0.0f, 0.0f, 1);

        texCoords[0] = { 0.5078125f, 0.0625f };
        texCoords[1] = { 0.5078125f, 0.9375f };
        texCoords[2] = { 0.6171875f, 0.0625f };
        texCoords[3] = { 0.6171875f, 0.9375f };

        topLeftTexture->SetTexture(border, 0, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        topLeftTexture->SetTexCoord(texCoords);
        topLeftTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x20) {
        // TODO
        // CSimpleTexture* topRightTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto topRightTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_topRightTexture = topRightTexture;

        topRightTexture->SetWidth(this->m_cornerSize);
        topRightTexture->SetHeight(this->m_cornerSize);
        topRightTexture->SetPoint(FRAMEPOINT_TOPRIGHT, frame, FRAMEPOINT_TOPRIGHT, 0.0f, 0.0f, 1);

        texCoords[0] = { 0.6328125f, 0.0625f };
        texCoords[1] = { 0.6328125f, 0.9375f };
        texCoords[2] = { 0.7421875f, 0.0625f };
        texCoords[3] = { 0.7421875f, 0.9375f };

        topRightTexture->SetTexture(border, 0, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        topRightTexture->SetTexCoord(texCoords);
        topRightTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x40) {
        // TODO
        // CSimpleTexture* bottomLeftTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto bottomLeftTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_bottomLeftTexture = bottomLeftTexture;

        bottomLeftTexture->SetWidth(this->m_cornerSize);
        bottomLeftTexture->SetHeight(this->m_cornerSize);
        bottomLeftTexture->SetPoint(FRAMEPOINT_BOTTOMLEFT, frame, FRAMEPOINT_BOTTOMLEFT, 0.0f, 0.0f, 1);

        texCoords[0] = { 0.7578125f, 0.0625f };
        texCoords[1] = { 0.7578125f, 0.9375f };
        texCoords[2] = { 0.8671875f, 0.0625f };
        texCoords[3] = { 0.8671875f, 0.9375f };

        bottomLeftTexture->SetTexture(border, 0, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        bottomLeftTexture->SetTexCoord(texCoords);
        bottomLeftTexture->SetBlendMode(this->m_blend);
    }

    if (this->m_pieces & 0x80) {
        // TODO
        // CSimpleTexture* bottomRightTexture = CSimpleTexture::s_allocator.GetData(0, __FILE__, __LINE__);

        auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto bottomRightTexture = new (m) CSimpleTexture(frame, DRAWLAYER_BACKGROUND_BORDER, 1);

        this->m_bottomRightTexture = bottomRightTexture;

        bottomRightTexture->SetWidth(this->m_cornerSize);
        bottomRightTexture->SetHeight(this->m_cornerSize);
        bottomRightTexture->SetPoint(FRAMEPOINT_BOTTOMRIGHT, frame, FRAMEPOINT_BOTTOMRIGHT, 0.0f, 0.0f, 1);

        texCoords[0] = { 0.8828125f, 0.0625f };
        texCoords[1] = { 0.8828125f, 0.9375f };
        texCoords[2] = { 0.9921875f, 0.0625f };
        texCoords[3] = { 0.9921875f, 0.9375f };

        bottomRightTexture->SetTexture(border, 0, 0, CSimpleTexture::s_textureFilterMode, ImageMode_UI);
        bottomRightTexture->SetTexCoord(texCoords);
        bottomRightTexture->SetBlendMode(this->m_blend);
    }
}

void CBackdropGenerator::SetVertexColor(const CImVector& color) {
    this->m_color = color;

    if (this->m_backgroundTexture) {
        this->m_backgroundTexture->SetVertexColor(this->m_color);
    }
}
