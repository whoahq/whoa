#include "ui/CSimpleRegion.hpp"
#include "ui/CSimpleFrame.hpp"
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

void CSimpleRegion::OnColorChanged(bool a2) {
    if (this->m_parent) {
        // TODO adjust color based on parent
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
