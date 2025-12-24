#include "ui/simple/CSimpleTexture.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
#include "gx/Shader.hpp"
#include "ui/CFramePoint.hpp"
#include "ui/CRenderBatch.hpp"
#include "ui/FrameXML.hpp"
#include "ui/LoadXML.hpp"
#include "ui/Util.hpp"
#include "ui/simple/CSimpleFrame.hpp"
#include "ui/simple/CSimpleTextureScript.hpp"
#include "util/StringTo.hpp"
#include <algorithm>
#include <climits>
#include <common/XML.hpp>
#include <storm/String.hpp>

CGxShader* CSimpleTexture::s_imageModePixelShaders[2] = {};

uint16_t CSimpleTexture::s_indices[] = {
    0, 1, 2,
    2, 1, 3
};

int32_t CSimpleTexture::s_metatable;
int32_t CSimpleTexture::s_objectType;
EGxTexFilter CSimpleTexture::s_textureFilterMode = GxTex_Linear;

void CSimpleTexture::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleTexture::RegisterScriptMethods);
    CSimpleTexture::s_metatable = ref;
}

CGxShader* CSimpleTexture::GetImageModePixelShader(TextureImageMode mode) {
    CGxShader* shader = CSimpleTexture::s_imageModePixelShaders[mode];

    if (shader && shader->Valid()) {
        return shader;
    } else {
        return nullptr;
    }
}

int32_t CSimpleTexture::GetObjectType() {
    if (!CSimpleTexture::s_objectType) {
        CSimpleTexture::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleTexture::s_objectType;
}

void CSimpleTexture::Init() {
    static const char* paths[] = { "UI", "Desaturate" };

    for (int32_t i = 0; i < 2; i++) {
        g_theGxDevicePtr->ShaderCreate(
            &s_imageModePixelShaders[i],
            GxSh_Pixel,
            "Shaders\\Pixel",
            paths[i],
            1
        );
    }
}

void CSimpleTexture::RegisterScriptMethods(lua_State* L) {
    CScriptRegion::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleTextureMethods, NUM_SIMPLE_TEXTURE_SCRIPT_METHODS);
}

CSimpleTexture::CSimpleTexture(CSimpleFrame* frame, uint32_t drawlayer, int32_t show) : CSimpleRegion(frame, drawlayer, show) {
    this->m_nonBlocking = 0;
    this->m_updateTexCoord = 0;
    this->m_horizTile = 0;
    this->m_vertTile = 0;

    this->m_shader = CSimpleTexture::GetImageModePixelShader(ImageMode_UI);

    // TODO
    // CSimpleTexture::s_count++;

    this->m_texCoord[0] = { 0.0f, 0.0f };
    this->m_texCoord[1] = { 0.0f, 1.0f };
    this->m_texCoord[2] = { 1.0f, 0.0f };
    this->m_texCoord[3] = { 1.0f, 1.0f };
}

CSimpleTexture::~CSimpleTexture() {
    if (this->m_texture) {
        HandleClose(this->m_texture);
    }

    // TODO CSimpleTexture::s_count++;
}

void CSimpleTexture::Draw(CRenderBatch* batch) {
    if (this->m_texture) {
        batch->QueueTexture(this);
    }
}

float CSimpleTexture::GetHeight() {
    float layoutHeight = CLayoutFrame::GetHeight();

    if (layoutHeight != 0.0f) {
        return layoutHeight;
    }

    uint32_t textureHeight;

    if (TextureGetDimensions(this->m_texture, nullptr, &textureHeight, 0)) {
        float ndcHeight = static_cast<float>(textureHeight) / (CoordinateGetAspectCompensation() * 1024.0f);
        float ddcHeight = NDCToDDCWidth(ndcHeight);
        return ddcHeight;
    }

    return 0.0f;
}

int32_t CSimpleTexture::GetScriptMetaTable() {
    return CSimpleTexture::s_metatable;
}

void CSimpleTexture::GetTexCoord(C2Vector* texCoord) {
    texCoord[0] = { this->m_texCoord[0].x, this->m_texCoord[0].y };
    texCoord[1] = { this->m_texCoord[1].x, this->m_texCoord[1].y };
    texCoord[2] = { this->m_texCoord[2].x, this->m_texCoord[2].y };
    texCoord[3] = { this->m_texCoord[3].x, this->m_texCoord[3].y };
}

float CSimpleTexture::GetWidth() {
    float layoutWidth = CLayoutFrame::GetWidth();

    if (layoutWidth != 0.0f) {
        return layoutWidth;
    }

    uint32_t textureWidth;

    if (TextureGetDimensions(this->m_texture, &textureWidth, nullptr, 0)) {
        float ndcWidth = static_cast<float>(textureWidth) / (CoordinateGetAspectCompensation() * 1024.0f);
        float ddcWidth = NDCToDDCWidth(ndcWidth);
        return ddcWidth;
    }

    return 0.0f;
}

bool CSimpleTexture::IsA(int32_t type) {
    return type == CSimpleTexture::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleTexture::LoadXML(const XMLNode* node, CStatus* status) {
    const char* inheritsAttr = node->GetAttributeByName("inherits");

    if (inheritsAttr && *inheritsAttr) {
        const char* tainted;
        bool locked;

        XMLNode* inheritsNode = FrameXML_AcquireHashNode(inheritsAttr, tainted, locked);

        if (inheritsNode) {
            if (locked) {
                status->Add(STATUS_WARNING, "Recursively inherited node: %s", inheritsAttr);
            } else {
                this->LoadXML(inheritsNode, status);
                FrameXML_ReleaseHashNode(inheritsAttr);
            }
        } else {
            status->Add(STATUS_WARNING, "Couldn't find inherited node: %s", inheritsAttr);
        }
    }

    CScriptRegion::LoadXML(node, status);

    const char* hiddenAttr = node->GetAttributeByName("hidden");

    if (hiddenAttr && *hiddenAttr) {
        bool hide = StringToBOOL(hiddenAttr);

        if (hide) {
            this->Hide();
        } else {
            this->Show();
        }
    }

    int32_t wrapU = 0;
    int32_t wrapV = 0;
    CImVector color = { 0 };

    const char* horizTileAttr = node->GetAttributeByName("horizTile");
    if (horizTileAttr && *horizTileAttr) {
        this->m_horizTile = StringToBOOL(horizTileAttr);
        wrapU = 1;
    }

    const char* vertTileAttr = node->GetAttributeByName("vertTile");
    if (vertTileAttr && *vertTileAttr) {
        this->m_vertTile = StringToBOOL(vertTileAttr);
        wrapV = 1;
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "TexCoords", INT_MAX)) {
            int32_t valid = 1;

            float left = 0.0f;
            float right = 1.0f;
            float top = 0.0f;
            float bottom = 1.0f;

            const char* name = this->GetName();
            if (!name) {
                name = "<unnamed>";
            }

            const XMLNode* rectNode = child->GetChildByName("Rect");

            if (rectNode) {
                // TODO
            } else {
                const char* leftAttr = child->GetAttributeByName("left");
                if (leftAttr && *leftAttr) {
                    if (this->m_horizTile) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (horizTile is on)",
                            name
                        );

                        valid = 0;
                    }

                    left = SStrToFloat(leftAttr);

                    if (left < -10000.0f || left > 10000.0f) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (out of range)",
                            name
                        );

                        valid = 0;
                    }
                }

                const char* rightAttr = child->GetAttributeByName("right");
                if (rightAttr && *rightAttr) {
                    if (this->m_horizTile) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (horizTile is on)",
                            name
                        );

                        valid = 0;
                    }

                    right = SStrToFloat(rightAttr);

                    if (right < -10000.0f || right > 10000.0f) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (out of range)",
                            name
                        );

                        valid = 0;
                    }
                }

                const char* topAttr = child->GetAttributeByName("top");
                if (topAttr && *topAttr) {
                    if (this->m_vertTile) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (vertTile is on)",
                            name
                        );

                        valid = 0;
                    }

                    top = SStrToFloat(topAttr);

                    if (top < -10000.0f || top > 10000.0f) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (out of range)",
                            name
                        );

                        valid = 0;
                    }
                }

                const char* bottomAttr = child->GetAttributeByName("bottom");
                if (bottomAttr && *bottomAttr) {
                    if (this->m_vertTile) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (vertTile is on)",
                            name
                        );

                        valid = 0;
                    }

                    bottom = SStrToFloat(bottomAttr);

                    if (bottom < -10000.0f || bottom > 10000.0f) {
                        status->Add(
                            STATUS_ERROR,
                            "Texture %s: Invalid TexCoords value (out of range)",
                            name
                        );

                        valid = 0;
                    }
                }
            }

            if (valid) {
                C2Vector coords[4];

                coords[0].x = left;
                coords[0].y = top;

                coords[1].x = left;
                coords[1].y = bottom;

                coords[2].x = right;
                coords[2].y = top;

                coords[3].y = bottom;
                coords[3].x = right;

                this->SetTexCoord(coords);

                if (left < 0.0f || left > 1.0f || right < 0.0f || right > 1.0f) {
                    wrapU = 1;
                }

                if (top < 0.0f || top > 1.0f || bottom < 0.0f || bottom > 1.0f) {
                    wrapV = 1;
                }
            }

        } else if (!SStrCmpI(child->GetName(), "Color", INT_MAX)) {
            LoadXML_Color(child, color);
            this->SetTexture(color);

        } else if (!SStrCmpI(child->GetName(), "Gradient", INT_MAX)) {
            ORIENTATION orientation;
            CImVector minColor = {};
            CImVector maxColor = {};
            LoadXML_Gradient(child, orientation, minColor, maxColor, status);

            this->SetVertexGradient(orientation, minColor, maxColor);
        }
    }

    const char* v91 = node->GetAttributeByName("file");

    if (v91 && *v91) {
        if (this->SetTexture(v91, wrapU, wrapV, CSimpleTexture::s_textureFilterMode, ImageMode_UI)) {

            if (color.value) {
                this->SetVertexColor(color);
            }
        } else {
            const char* v94 = this->GetName();

            if (!v94) {
                v94 = "<unnamed>";
            }

            status->Add(
                STATUS_WARNING,
                "Texture %s: Unable to load texture file %s",
                v94,
                v91
            );

            // TODO
            // CTexture* texture = TextureCreateSolid(&CRAPPY_GREEN); // 0xFF00FF00

            // if (this->m_texture) {
            //     HandleClose(this->m_texture);
            // }

            // this->m_texture = texture;

            // this->OnRegionChanged();
        }
    }

    const char* alphaModeAttr = node->GetAttributeByName("alphaMode");
    if (alphaModeAttr && *alphaModeAttr) {
        EGxBlend alphaMode;

        if (StringToBlendMode(alphaModeAttr, alphaMode)) {
            this->SetBlendMode(alphaMode);
        }
    }

    const char* alphaAttr = node->GetAttributeByName("alpha");
    if (alphaAttr && *alphaAttr) {
        float alpha = SStrToFloat(alphaAttr);
        this->SetAlpha(alpha);
    }

    // TODO
    // - nonBlocking
}

void CSimpleTexture::OnFrameSizeChanged(const CRect& rect) {
    CLayoutFrame::OnFrameSizeChanged(rect);

    if (false) {
        // TODO
        // - something about animations?
    } else {
        this->SetPosition(this->m_rect, this->m_position);
    }

    if (this->m_horizTile || this->m_vertTile) {
        this->m_updateTexCoord = 1;
        this->OnRegionChanged();
    }

    if (this->m_parent) {
        this->m_parent->NotifyScrollParent();
    }
}

void CSimpleTexture::PostLoadXML(const XMLNode* node, CStatus* status) {
    if (this->m_parent) {
        for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
            if (this->m_points[i] && !(this->m_points[i]->m_flags & 0x8)) {
                break;
            }

            if (i + 1 == FRAMEPOINT_NUMPOINTS) {
                this->SetAllPoints(this->m_parent, 1);
                break;
            }
        }
    }
}

void CSimpleTexture::SetAlpha(float alpha) {
    // Clamp
    alpha = std::max(0.0f, std::min(alpha, 1.0f));

    CImVector color = { 0 };
    this->GetVertexColor(color);
    color.a = alpha * 255.0f;
    this->SetVertexColor(color);
}

void CSimpleTexture::SetBlendMode(EGxBlend blend) {
    if (blend == this->m_alphaMode) {
        return;
    }

    this->m_alphaMode = blend;
    this->OnRegionChanged();
}

void CSimpleTexture::SetPosition(const CRect& rect, C3Vector* position) {
    position[0] = { rect.minX, rect.maxY, this->m_layoutDepth };
    position[1] = { rect.minX, rect.minY, this->m_layoutDepth };
    position[2] = { rect.maxX, rect.maxY, this->m_layoutDepth };
    position[3] = { rect.maxX, rect.minY, this->m_layoutDepth };
}

void CSimpleTexture::SetTexCoord(const CRect& texRect) {
    this->m_texCoord[0] = { texRect.minX, texRect.minY };
    this->m_texCoord[1] = { texRect.minX, texRect.maxY };
    this->m_texCoord[2] = { texRect.maxX, texRect.minY };
    this->m_texCoord[3] = { texRect.maxX, texRect.maxY };
}

void CSimpleTexture::SetTexCoord(const C2Vector* texCoord) {
    this->m_texCoord[0] = { texCoord[0].x, texCoord[0].y };
    this->m_texCoord[1] = { texCoord[1].x, texCoord[1].y };
    this->m_texCoord[2] = { texCoord[2].x, texCoord[2].y };
    this->m_texCoord[3] = { texCoord[3].x, texCoord[3].y };
}

int32_t CSimpleTexture::SetTexture(const char* fileName, bool wrapU, bool wrapV, EGxTexFilter filter, TextureImageMode mode) {
    if (this->m_texture && fileName && TextureIsSame(this->m_texture, fileName)) {
        return 1;
    }

    HTEXTURE texture = nullptr;

    if (fileName && *fileName) {
        CStatus status;

        CGxTexFlags texFlags = CGxTexFlags(filter, wrapU, wrapV, 0, 0, 0, 1);

        texture = TextureCreate(fileName, texFlags, &status, 23);

        // TODO
        // if (status.unk4 >= 2) {
        //     HandleClose(texture);
        //     SysMsgAdd(&status);

        //     return 0;
        // }

        this->m_shader = CSimpleTexture::GetImageModePixelShader(mode);
    }

    if (this->m_texture) {
        HandleClose(this->m_texture);
    }

    this->m_texture = texture;

    this->OnRegionChanged();

    return 1;
}

int32_t CSimpleTexture::SetTexture(const CImVector& color) {
    HTEXTURE texture = TextureCreateSolid(color);

    if (this->m_texture) {
        HandleClose(this->m_texture);
    }

    this->m_texture = texture;

    this->OnRegionChanged();

    return 1;
}
