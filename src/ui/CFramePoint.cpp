#include "ui/CFramePoint.hpp"
#include "ui/CLayoutFrame.hpp"
#include <storm/Error.hpp>

const float CFramePoint::UNDEFINED = 3.4028237e38;

CFramePoint::CFramePoint(CLayoutFrame* relative, FRAMEPOINT framePoint, float offsetX, float offsetY) {
    STORM_ASSERT(relative != nullptr && framePoint != FRAMEPOINT_NUMPOINTS);

    this->m_flags = 0x0;

    this->SetRelative(relative, framePoint, offsetX, offsetY);
}

CLayoutFrame* CFramePoint::GetRelative() {
    return this->m_relative;
}

int32_t CFramePoint::GetRelativeRect(CRect& rect) {
    bool flag2initial = this->m_flags & 0x2;

    if (!flag2initial) {
        this->m_flags |= 0x2;
    }

    this->m_flags &= ~0x4;

    CLayoutFrame* relative = this->GetRelative();

    if (relative->IsResizePending()) {
        relative->Resize(1);

        if (this->m_flags & 0x4) {
            if (!flag2initial) {
                this->m_flags &= ~0x2;
            }

            return 0;
        }
    }

    if (relative->m_flags & 0x2) {
        relative->SetDeferredResize(0);

        if (this->m_flags & 0x4) {
            if (!flag2initial) {
                this->m_flags &= ~0x2;
            }

            return 0;
        }
    }

    if (!relative->GetRect(&rect)) {
        if (!flag2initial) {
            this->m_flags &= ~0x2;
        }

        return 0;
    }

    // Nothing appears to override the default IsAttachmentOrigin implementation, which always
    // returns false
    if (relative->IsAttachmentOrigin()) {
        rect.maxX -= rect.minX;
        rect.maxY -= rect.minY;
        rect.minX -= rect.minX;
        rect.minY -= rect.minY;
    }

    if (!flag2initial) {
        this->m_flags &= ~0x2;
    }

    return 1;
}

void CFramePoint::MarkUnused() {
    this->m_framePoint = FRAMEPOINT_NUMPOINTS;
    this->m_offset = { 0.0f, 0.0f };
    this->m_relative = nullptr;
    this->m_flags = this->m_flags & 0x2 ? 0x2 | 0x4 | 0x8 : 0x8;
}

void CFramePoint::SetRelative(CLayoutFrame* relative, FRAMEPOINT relativePoint, float offsetX, float offsetY) {
    this->m_offset = { offsetX, offsetY };
    this->m_framePoint = relativePoint;
    this->m_relative = relative;
    this->m_flags = this->m_flags & 0x2 ? 0x2 | 0x4 : 0x0;
}

float CFramePoint::X(float scale) {
    CRect relative = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (!this->GetRelativeRect(relative)) {
        return UNDEFINED;
    }

    switch (this->m_framePoint) {
        case FRAMEPOINT_TOPLEFT:
        case FRAMEPOINT_LEFT:
        case FRAMEPOINT_BOTTOMLEFT:
            return relative.minX + (this->m_offset.x * scale);

        case FRAMEPOINT_TOP:
        case FRAMEPOINT_CENTER:
        case FRAMEPOINT_BOTTOM:
            return ((relative.minX + relative.maxX) * 0.5f) + (this->m_offset.x * scale);

        case FRAMEPOINT_TOPRIGHT:
        case FRAMEPOINT_RIGHT:
        case FRAMEPOINT_BOTTOMRIGHT:
            return relative.maxX + (this->m_offset.x * scale);

        default:
            return UNDEFINED;
    }
}

float CFramePoint::Y(float scale) {
    CRect relative = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (!this->GetRelativeRect(relative)) {
        return UNDEFINED;
    }

    switch (this->m_framePoint) {
        case FRAMEPOINT_TOPLEFT:
        case FRAMEPOINT_TOP:
        case FRAMEPOINT_TOPRIGHT:
            return relative.maxY + (this->m_offset.y * scale);

        case FRAMEPOINT_LEFT:
        case FRAMEPOINT_CENTER:
        case FRAMEPOINT_RIGHT:
            return ((relative.minY + relative.maxY) * 0.5f) + (this->m_offset.y * scale);

        case FRAMEPOINT_BOTTOMLEFT:
        case FRAMEPOINT_BOTTOM:
        case FRAMEPOINT_BOTTOMRIGHT:
            return relative.minY + (this->m_offset.y * scale);

        default:
            return UNDEFINED;
    }
}
