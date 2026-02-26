#include "ui/simple/CSimpleRegion.hpp"
#include "ui/simple/CSimpleFrame.hpp"
#include <cstring>

CSimpleRegion::~CSimpleRegion() {
    this->SetFrame(nullptr, DRAWLAYER_BACKGROUND_BORDER, 0);
}

CSimpleRegion::CSimpleRegion(CSimpleFrame* frame, uint32_t drawlayer, int32_t show) : CScriptRegion() {
    memset(this->m_color, 0, sizeof(CImVector) * this->m_colorCount);
    this->m_colorCount = 0;

    memset(this->m_alpha, 0, sizeof(uint8_t) * this->m_alphaCount);
    this->m_alphaCount = 0;

    if (frame) {
        this->SetFrame(frame, drawlayer, show);
    }
}

void CSimpleRegion::GetVertexColor(CImVector& color) const {
    if (this->m_colorCount == 1) {
        color.a = this->m_alpha[0];
        color.r = this->m_color[0].r;
        color.g = this->m_color[0].g;
        color.b = this->m_color[0].b;
    } else {
        color = { 0xFF, 0xFF, 0xFF, 0xFF };
    }
}

void CSimpleRegion::Hide() {
    this->m_shown = 0;
    this->HideThis();
}

void CSimpleRegion::HideThis() {
    if (this->m_visible && this->m_parent) {
        if (!this->m_parent->m_loading) {
            this->SetDeferredResize(1);
        }

        this->m_parent->RemoveFrameRegion(this, this->m_drawlayer);
        this->m_visible = 0;
    }
}

bool CSimpleRegion::IsShown() {
    return this->m_shown == 1;
}

bool CSimpleRegion::IsVisible() {
    return this->m_visible == 1;
}

void CSimpleRegion::OnColorChanged(bool a2) {
    if (this->m_parent) {
        uint8_t effectiveAlpha = this->m_parent->m_alpha * this->m_parent->alphaBD / 255;

        if (effectiveAlpha < 254) {
            if (this->m_colorCount == 0) {
                this->m_alphaCount = 1;
                this->m_colorCount = 1;

                this->m_alpha[0] = 255;

                this->m_color[0].r = 255;
                this->m_color[0].g = 255;
                this->m_color[0].b = 255;

                a2 = true;
            }
        } else {
            if (this->m_colorCount) {
                bool clearColors = true;

                for (uint32_t i = 0; i < this->m_colorCount; i++) {
                    auto alpha = this->m_alpha[i];
                    auto& color = this->m_color[i];

                    // If any color is set to a non-default value, do not clear colors
                    if (alpha < 254 || color.r != 255 || color.g != 255 || color.b != 255) {
                        clearColors = false;
                        break;
                    }
                }

                // If all colors were set to default values, clear colors
                if (clearColors) {
                    this->m_colorCount = 0;
                    this->m_alphaCount = 0;

                    a2 = true;
                }
            }
        }

        for (uint32_t i = 0; i < this->m_colorCount; i++) {
            this->m_color[i].a = this->m_alpha[i] * effectiveAlpha / 255;
        }
    }

    if (a2) {
        this->OnRegionChanged();
    }
}

void CSimpleRegion::OnRegionChanged() {
    if (this->m_visible && this->m_parent) {
        this->m_parent->NotifyDrawLayerChanged(this->m_drawlayer);
    }
}

void CSimpleRegion::SetVertexColor(const CImVector& color) {
    if (
        this->m_colorCount == 0
        && color.a >= 0xFE
        && color.r == 0xFF
        && color.g == 0xFF
        && color.b == 0xFF
    ) {
        return;
    }

    if (
        this->m_colorCount == 1
        && this->m_alpha[0] == color.a
        && this->m_color[0].r == color.r
        && this->m_color[0].g == color.g
        && this->m_color[0].b == color.b
    ) {
        return;
    }

    bool b1 = false;

    if (this->m_colorCount != 1) {
        this->m_colorCount = 1;
        this->m_alphaCount = 1;

        b1 = true;
    }

    this->m_color[0] = color;
    this->m_alpha[0] = color.a;

    this->OnColorChanged(b1);
}

void CSimpleRegion::SetVertexGradient(ORIENTATION orientation, const CImVector& minColor, const CImVector& maxColor) {
    bool b1 = false;

    if (this->m_colorCount != 4) {
        this->m_alphaCount = 4;
        this->m_colorCount = 4;
        b1 = true;
    }

    if (orientation == ORIENTATION_VERTICAL) {
        this->m_alpha[1] = minColor.a;
        this->m_color[1] = minColor;
        this->m_alpha[3] = minColor.a;
        this->m_color[3] = minColor;

        this->m_alpha[0] = maxColor.a;
        this->m_color[0] = maxColor;
        this->m_alpha[2] = maxColor.a;
        this->m_color[2] = maxColor;
    } else {
        this->m_alpha[0] = minColor.a;
        this->m_color[0] = minColor;
        this->m_alpha[1] = minColor.a;
        this->m_color[1] = minColor;

        this->m_alpha[2] = maxColor.a;
        this->m_color[2] = maxColor;
        this->m_alpha[3] = maxColor.a;
        this->m_color[3] = maxColor;
    }

    this->OnColorChanged(b1);
}

void CSimpleRegion::SetFrame(CSimpleFrame* frame, uint32_t drawlayer, int32_t show) {
    if (this->m_parent == frame) {
        if (this->m_drawlayer == drawlayer) {
            if (show != this->m_shown) {
                if (show) {
                    this->Show();
                } else {
                    this->Hide();
                }
            }
        } else {
            if (this->m_shown) {
                this->Hide();
            }

            this->m_drawlayer = drawlayer;

            if (show) {
                this->Show();
            }
        }
    } else {
        if (this->m_parent) {
            this->HideThis();
            this->m_parent->UnregisterRegion(this);
        }

        this->m_parent = frame;
        this->m_drawlayer = drawlayer;

        if (frame) {
            frame->RegisterRegion(this);
            this->SetDeferredResize(static_cast<CLayoutFrame*>(this->m_parent)->m_flags & 0x2);
            this->OnColorChanged(0);

            if (show) {
                this->Show();
            } else {
                this->Hide();
            }
        }
    }
}

void CSimpleRegion::Show() {
    this->m_shown = 1;
    this->ShowThis();
}

void CSimpleRegion::ShowThis() {
    if (this->m_shown && this->m_parent && this->m_parent->m_visible && !this->m_visible) {
        if (!this->m_parent->m_loading) {
            this->SetDeferredResize(0);
        }

        this->m_parent->AddFrameRegion(this, this->m_drawlayer);
        this->m_visible = 1;
    }
}
