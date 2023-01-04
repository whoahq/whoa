#include "ui/CLayoutFrame.hpp"
#include "gx/Coordinate.hpp"
#include "math/Utils.hpp"
#include "ui/CFramePoint.hpp"
#include "ui/FrameXML.hpp"
#include "ui/LoadXML.hpp"
#include "util/CStatus.hpp"
#include <common/XML.hpp>
#include <storm/Error.hpp>

STORM_EXPLICIT_LIST(CLayoutFrame, resizeLink) LayoutFrame::s_resizePendingList;

float SynthesizeSide(float center, float opposite, float size) {
    if (center != CFramePoint::UNDEFINED && opposite != CFramePoint::UNDEFINED) {
        return center + center - opposite;
    } else if (opposite != CFramePoint::UNDEFINED && size != 0.0f) {
        return opposite + size;
    } else if (center != CFramePoint::UNDEFINED && size != 0.0f) {
        return center + (size * 0.5f);
    } else {
        return CFramePoint::UNDEFINED;
    }
}

float SynthesizeCenter(float side1, float side2, float size) {
    if (side1 != CFramePoint::UNDEFINED && side2 != CFramePoint::UNDEFINED) {
        return (side1 + side2) * 0.5f;
    } else if (side1 != CFramePoint::UNDEFINED && size != 0.0f) {
        return side1 + (size * 0.5f);
    } else if (side2 != CFramePoint::UNDEFINED && size != 0.0f) {
        return side2 - (size * 0.5f);
    } else {
        return CFramePoint::UNDEFINED;
    }
}

void CLayoutFrame::ResizePending() {
    int32_t loading = 0;

    for (CLayoutFrame* frame = LayoutFrame::s_resizePendingList.Head(); frame && loading < 32; frame = LayoutFrame::s_resizePendingList.Link(frame)->Next()) {
        if (!frame->IsObjectLoaded()) {
            loading++;
            continue;
        }

        if (frame->OnFrameResize() || (frame->m_resizeCounter--, frame->m_resizeCounter == 0)) {
            frame->m_flags &= ~0x4;
            LayoutFrame::s_resizePendingList.UnlinkNode(frame);
        }
    }
}

CLayoutFrame::CLayoutFrame() {
    this->m_flags = 0;

    this->m_rect = { 0.0, 0.0, 0.0, 0.0 };
    this->m_width = 0.0;
    this->m_height = 0.0;

    this->m_layoutScale = 1.0;
    this->m_layoutDepth = 1.0;

    this->m_guard = { 0 };
}

CLayoutFrame::~CLayoutFrame() {
    this->DestroyLayout();

    // TODO
}

void CLayoutFrame::AddToResizeList() {
    if (LayoutFrame::s_resizePendingList.IsLinked(this)) {
        return;
    }

    CLayoutFrame* dependent = nullptr;

    for (auto frame = LayoutFrame::s_resizePendingList.Head(); frame; frame = LayoutFrame::s_resizePendingList.Link(frame)->Next()) {
        for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
            auto point = frame->m_points[i];

            if (point && !(point->m_flags & 0x8) && point->GetRelative() == this) {
                dependent = frame;
            }
        }
    }

    if (dependent) {
        LayoutFrame::s_resizePendingList.LinkNode(this, 2, dependent);
    } else {
        LayoutFrame::s_resizePendingList.LinkToTail(this);
    }

    this->m_resizeCounter = 6;
}

float CLayoutFrame::Bottom() {
    if (this->m_guard.bottom) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.bottom = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_BOTTOMLEFT,
        FRAMEPOINT_BOTTOM,
        FRAMEPOINT_BOTTOMRIGHT
    };

    float bottom = CFramePoint::UNDEFINED;
    this->GetFirstPointY(sidepoints, 3, bottom);

    if (bottom == CFramePoint::UNDEFINED) {
        float size = -(this->GetHeight() * this->m_layoutScale);
        float opposite = this->Top();
        float center = this->CenterY();

        bottom = SynthesizeSide(center, opposite, size);
    }

    this->m_guard.bottom = 0;

    return bottom;
}

int32_t CLayoutFrame::CalculateRect(CRect* rect) {
    rect->minX = this->Left();

    if (rect->minX == CFramePoint::UNDEFINED) {
        return 0;
    }

    rect->minY = this->Bottom();

    if (rect->minY == CFramePoint::UNDEFINED) {
        return 0;
    }

    rect->maxX = this->Right();

    if (rect->maxX == CFramePoint::UNDEFINED) {
        return 0;
    }

    rect->maxY = this->Top();

    if (rect->maxY == CFramePoint::UNDEFINED) {
        return 0;
    }

    if (!(this->m_flags & 0x10)) {
        return 1;
    }

    float v14 = 0.0f;
    float v15 = 0.0f;
    float v16 = 0.0f;
    float v17 = 0.0f;

    this->GetClampRectInsets(v15, v17, v16, v14);

    float v12 = -(this->m_layoutScale * v15);
    float v7 = NDCToDDCWidth(1.0f);
    float v13 = v7 - this->m_layoutScale * v17;
    float v11 = -(this->m_layoutScale * v14);
    float v8 = NDCToDDCHeight(1.0f);
    float v9 = v8 - this->m_layoutScale * v16;

    if (v12 > rect->minX) {
        rect->maxX = rect->maxX - (rect->minX - v12);
        rect->minX = v12;
    }

    if (v11 > rect->minY) {
        rect->maxY = rect->maxY - (rect->minY - v11);
        rect->minY = v11;
    }

    if (v13 < rect->maxX) {
        rect->minX = rect->minX - (rect->maxX - v13);
        rect->maxX = v13;
    }

    if (v9 < rect->maxY ) {
        rect->minY = rect->minY - (rect->maxY - v9);
        rect->maxY = v9;
    }

    return 1;
}

bool CLayoutFrame::CanBeAnchorFor(CLayoutFrame* frame) {
    // TODO

    return true;
}

float CLayoutFrame::CenterX() {
    if (this->m_guard.centerX) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.centerX = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_TOP,
        FRAMEPOINT_CENTER,
        FRAMEPOINT_BOTTOM
    };

    float centerX = CFramePoint::UNDEFINED;
    this->GetFirstPointX(sidepoints, 3, centerX);

    if (centerX == CFramePoint::UNDEFINED) {
        float size = this->GetWidth() * this->m_layoutScale;
        float side2 = this->Right();
        float side1 = this->Left();

        centerX = SynthesizeCenter(side1, side2, size);
    }

    this->m_guard.centerX = 0;

    return centerX;
}

float CLayoutFrame::CenterY() {
    if (this->m_guard.centerY) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.centerY = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_LEFT,
        FRAMEPOINT_CENTER,
        FRAMEPOINT_RIGHT
    };

    float centerY = CFramePoint::UNDEFINED;
    this->GetFirstPointY(sidepoints, 3, centerY);

    if (centerY == CFramePoint::UNDEFINED) {
        float size = this->GetHeight() * this->m_layoutScale;
        float side2 = this->Top();
        float side1 = this->Bottom();

        centerY = SynthesizeCenter(side1, side2, size);
    }

    this->m_guard.centerY = 0;

    return centerY;
}

void CLayoutFrame::ClearAllPoints() {
    this->FreePoints();
}

void CLayoutFrame::DestroyLayout() {
    for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
        auto point = this->m_points[i];

        if (point) {
            if (!(point->m_flags & 0x8) && point->m_relative) {
                point->m_relative->UnregisterResize(this, 1 << i);
            }

            delete point;
            this->m_points[i] = nullptr;
        }
    }

    for (auto node = this->m_resizeList.Head(); node; node = this->m_resizeList.Link(node)->Next()) {
        auto frame = node->frame;

        for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
            auto point = this->m_points[i];

            if (point && point->m_relative == this) {
                point->MarkUnused();
            }
        }

        frame->m_flags &= ~0x8;
    }

    this->m_resizeList.DeleteAll();
}

void CLayoutFrame::FreePoints() {
    for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
        auto point = this->m_points[i];

        if (point && !(point->m_flags & 0x8)) {
            if (point->m_relative) {
                point->m_relative->UnregisterResize(this, 1 << i);
            }

            point->MarkUnused();
        }
    }
}

void CLayoutFrame::GetClampRectInsets(float& a1, float& a2, float& a3, float& a4) {
    a4 = 0.0f;
    a3 = 0.0f;
    a2 = 0.0f;
    a1 = 0.0f;
}

void CLayoutFrame::GetFirstPointX(const FRAMEPOINT* const pointarray, int32_t elements, float& x) {
    // TODO
    // - what's up with this loop?

    for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
        if (this->Sub488DB0(pointarray, elements, x)) {
            break;
        }
    }
}

void CLayoutFrame::GetFirstPointY(const FRAMEPOINT* const pointarray, int32_t elements, float& y) {
    // TODO
    // - what's up with this loop?

    for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
        if (this->Sub488E40(pointarray, elements, y)) {
            break;
        }
    }
}

CLayoutFrame* CLayoutFrame::GetLayoutFrameByName(const char* name) {
    return nullptr;
}

CLayoutFrame* CLayoutFrame::GetLayoutParent() {
    return nullptr;
}

int32_t CLayoutFrame::GetRect(CRect* rect) {
    if (!(this->m_flags & 0x1)) {
        return 0;
    }

    rect->minY = this->m_rect.minY;
    rect->minX = this->m_rect.minX;
    rect->maxY = this->m_rect.maxY;
    rect->maxX = this->m_rect.maxX;

    return 1;
}

float CLayoutFrame::GetHeight() {
    return this->m_height;
}

float CLayoutFrame::GetWidth() {
    return this->m_width;
}

int32_t CLayoutFrame::IsAttachmentOrigin() {
    return 0;
}

int32_t CLayoutFrame::IsObjectLoaded() {
    return 1;
}

int32_t CLayoutFrame::IsResizeDependency(CLayoutFrame* dependentFrame) {
    CLayoutFrame* v2 = nullptr;

    for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
        auto framePoint = this->m_points[i];

        if (!framePoint || framePoint->m_flags & 0x8) {
            continue;
        }

        if (!framePoint->GetRelative() || framePoint->GetRelative() == v2) {
            continue;
        }

        if (framePoint->GetRelative() == dependentFrame || framePoint->GetRelative()->IsResizeDependency(dependentFrame)) {
            return 1;
        }

        v2 = framePoint->GetRelative();
    }

    return 0;
}

uint32_t CLayoutFrame::IsResizePending() {
    return this->m_flags & 0x4;
}

float CLayoutFrame::Left() {
    if (this->m_guard.left) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.left = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_TOPLEFT,
        FRAMEPOINT_LEFT,
        FRAMEPOINT_BOTTOMLEFT
    };

    float left = CFramePoint::UNDEFINED;
    this->GetFirstPointX(sidepoints, 3, left);

    if (left == CFramePoint::UNDEFINED) {
        float size = -(this->GetWidth() * this->m_layoutScale);
        float opposite = this->Right();
        float center = this->CenterX();

        left = SynthesizeSide(center, opposite, size);
    }

    this->m_guard.left = 0;

    return left;
}

void CLayoutFrame::LoadXML(XMLNode* node, CStatus* status) {
    XMLNode* size = node->GetChildByName("Size");
    float width;
    float height;

    if (size && LoadXML_Dimensions(size, width, height, status)) {
        this->SetWidth(width);
        this->SetHeight(height);
    }

    CLayoutFrame* layoutParent = this->GetLayoutParent();

    int32_t setAllPoints;

    const char* setAllPointsAttr = node->GetAttributeByName("setAllPoints");

    if (setAllPointsAttr && !SStrCmpI(setAllPointsAttr, "true", 0x7FFFFFFF)) {
        setAllPoints = 1;
    } else {
        setAllPoints = 0;
    }

    const XMLNode* anchorsNode = node->GetChildByName("Anchors");

    if (anchorsNode) {
        if (setAllPoints) {
            status->Add(
                STATUS_WARNING,
                "SETALLPOINTS set to true in frame with anchors (ignored)"
            );
        }

        for (XMLNode* anchorNode = anchorsNode->m_child; anchorNode; anchorNode = anchorNode->m_next) {
            const char* pointAttr = anchorNode->GetAttributeByName("point");

            FRAMEPOINT point;

            if (!pointAttr || !StringToFramePoint(pointAttr, point)) {
                status->Add(
                    STATUS_WARNING,
                    "Invalid anchor point in frame: %s",
                    pointAttr
                );

                continue;
            }

            const char* relativePointAttr = anchorNode->GetAttributeByName("relativePoint");

            FRAMEPOINT relativePoint;

            if (relativePointAttr && *relativePointAttr) {
                if (!StringToFramePoint(relativePointAttr, relativePoint)) {
                    status->Add(
                        STATUS_WARNING,
                        "Invalid anchor point in frame: %s",
                        relativePointAttr
                    );

                    continue;
                }
            } else {
                relativePoint = point;
            }

            const char* relativeToAttr = anchorNode->GetAttributeByName("relativeTo");

            CLayoutFrame* relative;

            if (!relativeToAttr || !*relativeToAttr) {
                relative = layoutParent;
            } else {
                relative = this->GetLayoutFrameByName(relativeToAttr);

                if (!relative) {
                    status->Add(
                        STATUS_WARNING,
                        "Couldn't find relative frame: %s",
                        relativeToAttr
                    );

                    continue;
                }

                if (relative && relative == this) {
                    status->Add(
                        STATUS_WARNING,
                        "Frame anchored to itself: %s",
                        relativeToAttr
                    );

                    continue;
                }
            }

            XMLNode* offsetNode = anchorNode->GetChildByName("Offset");

            float offsetX = 0.0f;
            float offsetY = 0.0f;

            if (offsetNode) {
                LoadXML_Dimensions(offsetNode, offsetX, offsetY, status);
            } else {
                LoadXML_Dimensions(anchorNode, offsetX, offsetY, status);
            }

            this->SetPoint(point, relative, relativePoint, offsetX, offsetY, 0);
        }

        this->Resize(0);
    } else if (setAllPoints) {
        this->SetAllPoints(layoutParent, 1);
    }
}

int32_t CLayoutFrame::OnFrameResize() {
    CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (!this->CalculateRect(&rect)) {
        this->m_flags = (this->m_flags & ~0x1) | 0x8;
        return 0;
    }

    this->m_flags &= ~(0x4 | 0x8);

    if (
        this->m_flags & 0x1
        && AreEqual(rect.minX, this->m_rect.minX, WHOA_EPSILON_2)
        && AreEqual(rect.maxX, this->m_rect.maxX, WHOA_EPSILON_2)
        && AreEqual(rect.minY, this->m_rect.minY, WHOA_EPSILON_2)
        && AreEqual(rect.maxY, this->m_rect.maxY, WHOA_EPSILON_2)
    ) {
        return 1;
    }

    CRect prevRect = {
        this->m_rect.minY,
        this->m_rect.minX,
        this->m_rect.maxY,
        this->m_rect.maxX
    };

    this->m_rect.minY = rect.minY;
    this->m_rect.minX = rect.minX;
    this->m_rect.maxY = rect.maxY;
    this->m_rect.maxX = rect.maxX;

    this->m_flags |= 0x1;

    this->OnFrameSizeChanged(prevRect);

    return this->m_flags & 0x1;
}

void CLayoutFrame::OnFrameSizeChanged(const CRect& rect) {
    for (auto node = this->m_resizeList.Head(); node; node = this->m_resizeList.Link(node)->Next()) {
        node->frame->Resize(0);
    }
}

void CLayoutFrame::OnProtectedAttach(CLayoutFrame* frame) {
    if (this->m_flags & 0x400) {
        frame->SetProtectFlag(0x400);
    }

    if (this->m_flags & 0x300) {
        frame->SetProtectFlag(0x200);
    }
}

int32_t CLayoutFrame::PtInFrameRect(const C2Vector& pt) {
    // TODO
    return 0;
}

void CLayoutFrame::RegisterResize(CLayoutFrame* frame, uint32_t dep) {
    for (auto node = this->m_resizeList.Head(); node; node = this->m_resizeList.Link(node)->Next()) {
        if (node->frame == frame) {
            node->dep |= dep;
            return;
        }
    }

    auto node = this->m_resizeList.NewNode(2, 0, 0x8);

    node->frame = frame;
    node->dep = dep;

    if (static_cast<CLayoutFrame*>(frame)->m_flags & 0x400) {
        this->SetProtectFlag(0x400);
    }

    if (static_cast<CLayoutFrame*>(frame)->m_flags & (0x100 | 0x200)) {
        this->SetProtectFlag(0x200);
    }
}

void CLayoutFrame::Resize(int32_t force) {
    if (force && !(this->m_flags & 0x8) && this->OnFrameResize()) {
        LayoutFrame::s_resizePendingList.UnlinkNode(this);
        return;
    }

    if (this->m_flags & 0x4 && (this->m_flags & 0x2 || LayoutFrame::s_resizePendingList.IsLinked(this))) {
        this->m_resizeCounter = 6;
        return;
    }

    this->m_flags |= 0x4;

    if (this->m_flags & 0x2) {
        for (auto node = this->m_resizeList.Head(); node; node = this->m_resizeList.Link(node)->Next()) {
            node->frame->Resize(0);
        }
    } else {
        this->AddToResizeList();
    }
}

float CLayoutFrame::Right() {
    if (this->m_guard.right) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.right = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_TOPRIGHT,
        FRAMEPOINT_RIGHT,
        FRAMEPOINT_BOTTOMRIGHT
    };

    float right = CFramePoint::UNDEFINED;
    this->GetFirstPointX(sidepoints, 3, right);

    if (right == CFramePoint::UNDEFINED) {
        float size = this->GetWidth() * this->m_layoutScale;
        float opposite = this->Left();
        float center = this->CenterX();

        right = SynthesizeSide(center, opposite, size);
    }

    this->m_guard.right = 0;

    return right;
}

void CLayoutFrame::SetAllPoints(CLayoutFrame* relative, int32_t doResize) {
    STORM_ASSERT(relative);
    STORM_ASSERT(relative != this);

    if (!relative->CanBeAnchorFor(this)) {
        return;
    }

    this->FreePoints();

    auto topLeft = this->m_points[FRAMEPOINT_TOPLEFT];

    if (topLeft) {
        topLeft->SetRelative(relative, FRAMEPOINT_TOPLEFT, 0.0f, 0.0f);
    } else {
        // TODO
        // CFramePoint::s_framePointHeap->GetData(0, __FILE__, __LINE__);

        void* m = SMemAlloc(sizeof(CFramePoint), __FILE__, __LINE__, 0);
        topLeft = m ? new (m) CFramePoint(relative, FRAMEPOINT_TOPLEFT, 0.0f, 0.0f) : nullptr;

        this->m_points[FRAMEPOINT_TOPLEFT] = topLeft;
    }

    auto bottomRight = this->m_points[FRAMEPOINT_BOTTOMRIGHT];

    if (bottomRight) {
        bottomRight->SetRelative(relative, FRAMEPOINT_BOTTOMRIGHT, 0.0f, 0.0f);
    } else {
        // TODO
        // CFramePoint::s_framePointHeap->GetData(0, __FILE__, __LINE__);

        void* m = SMemAlloc(sizeof(CFramePoint), __FILE__, __LINE__, 0);
        bottomRight = m ? new (m) CFramePoint(relative, FRAMEPOINT_BOTTOMRIGHT, 0.0f, 0.0f) : nullptr;

        this->m_points[FRAMEPOINT_BOTTOMRIGHT] = bottomRight;
    }

    this->m_flags &= ~0x8;
    relative->RegisterResize(this, (1 << FRAMEPOINT_TOPLEFT) | (1 << FRAMEPOINT_BOTTOMRIGHT));

    if (doResize) {
        this->Resize(0);
    }
}

void CLayoutFrame::SetDeferredResize(int32_t enable) {
    if (enable) {
        this->m_flags |= 0x2;

        if (LayoutFrame::s_resizePendingList.Link(this)->IsLinked()) {
            LayoutFrame::s_resizePendingList.UnlinkNode(this);
        }

        return;
    }

    this->m_flags &= ~0x2;

    if (this->m_flags & 0x4) {
        this->Resize(1);
    }
}

void CLayoutFrame::SetHeight(float height) {
    this->m_flags &= ~0x8;
    this->m_height = height;
    this->Resize(0);
}

bool CLayoutFrame::SetLayoutScale(float scale, bool force) {
    STORM_ASSERT(scale);

    if (force || (!AreEqual(this->m_layoutScale, scale, WHOA_EPSILON_1) && scale > 0.00000011920929)) {
        this->m_layoutScale = scale;
        this->m_rect = { 0.0f, 0.0f, 0.0f, 0.0f };
        this->m_flags &= ~0x1;

        this->Resize(0);

        return true;
    }

    return false;
}

void CLayoutFrame::SetPoint(FRAMEPOINT point, CLayoutFrame* relative, FRAMEPOINT relativePoint, float offsetX, float offsetY, int32_t doResize) {
    STORM_ASSERT(relative);
    STORM_ASSERT(relative != this);

    if (!relative->CanBeAnchorFor(this)) {
        return;
    }

    auto framePoint = this->m_points[point];

    if (framePoint) {
        if (framePoint->GetRelative()) {
            if (relative == framePoint->GetRelative()) {
                if (
                    framePoint->m_framePoint != relativePoint
                    || !AreEqual(framePoint->m_offset.x, offsetX, WHOA_EPSILON_1)
                    || !AreEqual(framePoint->m_offset.y, offsetY, WHOA_EPSILON_1)
                ) {
                    framePoint->SetRelative(relative, relativePoint, offsetX, offsetY);
                    this->m_flags &= ~0x8;

                    if (doResize) {
                        this->Resize(0);
                    }
                }

                return;
            } else {
                framePoint->GetRelative()->UnregisterResize(this, 1 << point);
            }
        }

        framePoint->SetRelative(relative, relativePoint, offsetX, offsetY);
    } else {
        // TODO
        // CFramePoint::s_framePointHeap->GetData(0, __FILE__, __LINE__);

        void* m = SMemAlloc(sizeof(CFramePoint), __FILE__, __LINE__, 0);
        framePoint = m ? new (m) CFramePoint(relative, relativePoint, offsetX, offsetY) : nullptr;
        this->m_points[point] = framePoint;
    }

    this->m_flags &= ~0x8;
    relative->RegisterResize(this, 1 << point);

    if (doResize) {
        this->Resize(0);
    }
}

void CLayoutFrame::SetProtectFlag(uint32_t flag) {
    // TODO
}

void CLayoutFrame::SetWidth(float width) {
    this->m_flags &= ~0x8;
    this->m_width = width;
    this->Resize(0);
}

int32_t CLayoutFrame::Sub488DB0(const FRAMEPOINT* const pointarray, int32_t elements, float& x) {
    for (int32_t i = 0; i < elements; i++) {
        FRAMEPOINT f = pointarray[i];
        CFramePoint* point = this->m_points[f];

        if (point && !(point->m_flags & 0x8)) {
            x = point->X(this->m_layoutScale);

            if (point->m_flags & 0x4) {
                x = CFramePoint::UNDEFINED;
                return 0;
            }

            if (x != CFramePoint::UNDEFINED) {
                return 1;
            }
        }
    }

    x = CFramePoint::UNDEFINED;
    return 1;
}

int32_t CLayoutFrame::Sub488E40(const FRAMEPOINT* const pointarray, int32_t elements, float& y) {
    for (int32_t i = 0; i < elements; i++) {
        FRAMEPOINT f = pointarray[i];
        CFramePoint* point = this->m_points[f];

        if (point && !(point->m_flags & 0x8)) {
            y = point->Y(this->m_layoutScale);

            if (point->m_flags & 0x4) {
                y = CFramePoint::UNDEFINED;
                return 0;
            }

            if (y != CFramePoint::UNDEFINED) {
                return 1;
            }
        }
    }

    y = CFramePoint::UNDEFINED;
    return 1;
}

float CLayoutFrame::Top() {
    if (this->m_guard.top) {
        return CFramePoint::UNDEFINED;
    }

    this->m_guard.top = 1;

    static FRAMEPOINT sidepoints[] = {
        FRAMEPOINT_TOPLEFT,
        FRAMEPOINT_TOP,
        FRAMEPOINT_TOPRIGHT
    };

    float top = CFramePoint::UNDEFINED;
    this->GetFirstPointY(sidepoints, 3, top);

    if (top == CFramePoint::UNDEFINED) {
        float size = this->GetHeight() * this->m_layoutScale;
        float opposite = this->Bottom();
        float center = this->CenterY();

        top = SynthesizeSide(center, opposite, size);
    }

    this->m_guard.top = 0;

    return top;
}

void CLayoutFrame::UnflattenFrame(CLayoutFrame* frame) {
    // TODO
}

void CLayoutFrame::UnregisterResize(CLayoutFrame* frame, uint32_t dep) {
    for (auto node = this->m_resizeList.Head(); node; node = this->m_resizeList.Link(node)->Next()) {
        if (node->frame == frame) {
            node->dep &= ~dep;

            if (node->dep) {
                return;
            }

            if (static_cast<CLayoutFrame*>(frame)->m_flags & (0x100 | 0x200 | 0x400)) {
                this->SetProtectFlag(0x400);
            }

            this->m_resizeList.DeleteNode(node);

            return;
        }
    }
}
