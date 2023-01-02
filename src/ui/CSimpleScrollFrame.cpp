#include "ui/CSimpleScrollFrame.hpp"
#include "gx/Camera.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Transform.hpp"
#include "ui/CRenderBatch.hpp"
#include "ui/CSimpleScrollFrameScript.hpp"
#include "ui/FrameXML.hpp"
#include "util/CStatus.hpp"
#include "util/Lua.hpp"
#include <cmath>
#include <limits>
#include <common/XML.hpp>
#include <tempest/Matrix.hpp>

int32_t CSimpleScrollFrame::s_metatable;
int32_t CSimpleScrollFrame::s_objectType;

void CSimpleScrollFrame::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleScrollFrame::RegisterScriptMethods);
    CSimpleScrollFrame::s_metatable = ref;
}

int32_t CSimpleScrollFrame::GetObjectType() {
    if (!CSimpleScrollFrame::s_objectType) {
        CSimpleScrollFrame::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleScrollFrame::s_objectType;
}

void CSimpleScrollFrame::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleScrollFrameMethods, NUM_SIMPLE_SCROLL_FRAME_SCRIPT_METHODS);
}

void CSimpleScrollFrame::RenderScrollChild(void* param) {
    auto frame = static_cast<CSimpleScrollFrame*>(param);

    CRect viewRect;
    if (!frame->GetHitRect(viewRect)) {
        return;
    }

    auto scrollChild = frame->m_scrollChild;
    if (!scrollChild) {
        return;
    }

    if (!scrollChild->m_visible) {
        return;
    }

    C44Matrix savedProj;
    GxXformProjection(savedProj);

    C44Matrix savedView;
    GxXformView(savedView);

    float minX, maxX, minY, maxY, minZ, maxZ;
    GxXformViewport(minX, maxX, minY, maxY, minZ, maxZ);

    // TODO clean up
    float v34 = 0.0f;
    float v35 = 0.0f;
    float v36 = 0.0f;
    float v37 = 0.0f;
    NDCToDDC(minX, minY, &v35, &v34);
    NDCToDDC(maxX, maxY, &v37, &v36);

    auto v4 = viewRect.minX;
    if (v35 > viewRect.minX) {
        v4 = v35;
        viewRect.minX = v35;
    }
    auto v5 = viewRect.maxX;
    if (v37 < viewRect.maxX) {
        v5 = v37;
        viewRect.maxX = v37;
    }
    auto v6 = viewRect.minY;
    if (v34 > viewRect.minY) {
        v6 = v34;
        viewRect.minY = v34;
    }
    auto v7 = viewRect.maxY;
    if (v36 < viewRect.maxY) {
        v7 = v36;
        viewRect.maxY = v36;
    }

    if (v4 != v5 && v6 != v7) {
        C2Vector screenPoint = { 0.0f, 0.0f };
        CameraSetupScreenProjection(viewRect, screenPoint, 0.0f, 0);

        DDCToNDC(viewRect.minX, viewRect.minY, &viewRect.minX, &viewRect.minY);
        DDCToNDC(viewRect.maxX, viewRect.maxY, &viewRect.maxX, &viewRect.maxY);

        float v11, v12, v13, v14;

        if (viewRect.minX >= 0.0f) {
            v12 = viewRect.minX;
            v11 = viewRect.maxX;
        } else {
            v11 = viewRect.maxX - viewRect.minX;
            viewRect.maxX = v11;
            v12 = 0.0f;
            viewRect.minX = 0.0f;
        }

        if (v11 > 1.0f) {
            v12 = v12 - (v11 - 1.0f);
            viewRect.minX = v12;
            viewRect.maxX = 1.0f;
            v11 = 1.0f;
        }

        if (viewRect.minY >= 0.0f) {
            v14 = viewRect.minY;
            v13 = viewRect.maxY;
        } else {
            v13 = viewRect.maxY - viewRect.minY;
            viewRect.maxY = v13;
            v14 = 0.0f;
            viewRect.minY = 0.0f;
        }

        float v15 = v14;
        float v16 = v13;
        float v17 = v15;

        if (v16 > 1.0f) {
            v17 = v17 - (v16 - 1.0f);
            viewRect.minY = v17;
            v16 = 1.0f;
            viewRect.maxY = 1.0f;
        }

        float v18 = v16;
        float v19 = v12;
        float v20 = v18;

        if (v19 < v11) {
            float v21 = v19;
            float v22 = v17;
            float v23 = v21;

            if (v22 < v20) {
                float v27 = v20;
                float v26 = v22;
                float v25 = v11;
                float v24 = v23;

                GxXformSetViewport(v24, v25, v26, v27, 0.0f, 1.0f);
                scrollChild->OnFrameRender();
            }
        }

        GxXformSetProjection(savedProj);
        GxXformSetView(savedView);
        GxXformSetViewport(minX, maxX, minY, maxY, minZ, maxZ);
    }
}

CSimpleScrollFrame::CSimpleScrollFrame(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}

int32_t CSimpleScrollFrame::GetBoundsRect(CRect& bounds) {
    if (this->IsResizePending()) {
        this->Resize(1);
    }

    CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (this->GetRect(&rect)) {
        bounds.minX = rect.minX >= bounds.minX ? bounds.minX : rect.minX;
        bounds.maxX = rect.maxX <= bounds.maxX ? bounds.maxX : rect.maxX;
        bounds.minY = rect.minY >= bounds.minY ? bounds.minY : rect.minY;
        bounds.maxY = rect.maxY <= bounds.maxY ? bounds.maxY : rect.maxY;
    }

    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Next(region)) {
        if (region->IsShown()) {
            if (region->IsResizePending()) {
                region->Resize(1);
            }

            if (region->GetRect(&rect)) {
                bounds.minX = rect.minX >= bounds.minX ? bounds.minX : rect.minX;
                bounds.maxX = rect.maxX <= bounds.maxX ? bounds.maxX : rect.maxX;
                bounds.minY = rect.minY >= bounds.minY ? bounds.minY : rect.minY;
                bounds.maxY = rect.maxY <= bounds.maxY ? bounds.maxY : rect.maxY;
            }
        }
    }

    for (auto child = this->m_children.Head(); child; child = this->m_children.Next(child)) {
        if (child->frame != this->m_scrollChild && child->frame->m_shown) {
            child->frame->GetBoundsRect(bounds);
        }
    }

    return bounds.maxY > bounds.minY && bounds.maxX > bounds.minX;
}

FrameScript_Object::ScriptIx* CSimpleScrollFrame::GetScriptByName(const char* name, ScriptData& data) {
    auto parentScript = CSimpleFrame::GetScriptByName(name, data);

    if (parentScript) {
        return parentScript;
    }

    if (!SStrCmpI(name, "OnHorizontalScroll", STORM_MAX_STR)) {
        data.wrapper = "return function(self,offset) %s end";
        return &this->m_onHorizontalScroll;
    }

    if (!SStrCmpI(name, "OnVerticalScroll", STORM_MAX_STR)) {
        data.wrapper = "return function(self,offset) %s end";
        return &this->m_onVerticalScroll;
    }

    if (!SStrCmpI(name, "OnScrollRangeChanged", STORM_MAX_STR)) {
        data.wrapper = "return function(self,xrange,yrange) %s end";
        return &this->m_onScrollRangeChanged;
    }

    return nullptr;
}

int32_t CSimpleScrollFrame::GetScriptMetaTable() {
    return CSimpleScrollFrame::s_metatable;
}

bool CSimpleScrollFrame::IsA(int32_t type) {
    return type == CSimpleScrollFrame::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleScrollFrame::LoadXML(XMLNode* node, CStatus* status) {
    CSimpleFrame::LoadXML(node, status);

    XMLNode* scrollChildNode = node->GetChildByName("ScrollChild");

    if (scrollChildNode) {
        XMLNode* childNode = scrollChildNode->m_child;

        if (childNode) {
            CSimpleFrame* childFrame = FrameXML_CreateFrame(childNode, this, status);

            if (childFrame) {
                this->SetScrollChild(childFrame);
            }
        } else {
            const char* frameName = this->GetName();

            if (!frameName) {
                frameName = "<unnamed>";
            }

            status->Add(
                STATUS_WARNING,
                "Frame %s: Scroll frame created without scroll child",
                frameName
            );
        }
    }
}

void CSimpleScrollFrame::OnFrameRender(CRenderBatch* batch, uint32_t layer) {
    CSimpleFrame::OnFrameRender(batch, layer);

    if (layer == DRAWLAYER_HIGHLIGHT) {
        batch->QueueCallback(CSimpleScrollFrame::RenderScrollChild, this);
    }
}

void CSimpleScrollFrame::OnFrameSizeChanged(float w, float h) {
    CSimpleFrame::OnFrameSizeChanged(w, h);
    this->m_updateScrollChild = 1;
}

void CSimpleScrollFrame::OnLayerUpdate(float elapsedSec) {
    CSimpleFrame::OnLayerUpdate(elapsedSec);

    if (this->m_updateScrollChild) {
        CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (this->GetRect(&rect)) {
            float w = rect.maxX - rect.minX;
            float h = rect.maxY - rect.minY;
            this->UpdateScrollChildRect(w, h);
        }

        this->m_updateScrollChild = 0;
    }
}

void CSimpleScrollFrame::RunOnScrollRangeChangedScript() {
    if (!this->m_onScrollRangeChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    auto rangeX = this->m_scrollRange.x;
    auto ddcRangeX = CoordinateGetAspectCompensation() * 1024.0f * rangeX;
    auto ndcRangeX = DDCToNDCWidth(ddcRangeX);
    lua_pushnumber(L, ndcRangeX);

    auto rangeY = this->m_scrollRange.y;
    auto ddcRangeY = CoordinateGetAspectCompensation() * 1024.0f * rangeY;
    auto ndcRangeY = DDCToNDCWidth(ddcRangeY);
    lua_pushnumber(L, ndcRangeY);

    this->RunScript(this->m_onScrollRangeChanged, 2, nullptr);
}

void CSimpleScrollFrame::RunOnVerticalScrollScript() {
    if (!this->m_onVerticalScroll.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    auto offsetY = this->m_scrollOffset.y;
    auto ddcOffsetY = CoordinateGetAspectCompensation() * 1024.0f * offsetY;
    auto ndcOffsetY = DDCToNDCWidth(ddcOffsetY);
    lua_pushnumber(L, ndcOffsetY);

    this->RunScript(this->m_onVerticalScroll, 1, nullptr);
}

void CSimpleScrollFrame::SetScrollChild(CSimpleFrame* frame) {
    if (this->m_scrollChild) {
        this->m_scrollChild->SetBeingScrolled(0, 0);
        this->m_scrollChild->SetParent(nullptr);
    }

    this->m_scrollChild = frame;

    if (this->m_scrollChild) {
        this->m_scrollChild->SetParent(this);
        this->m_scrollChild->SetBeingScrolled(1, 1);
    }

    this->UpdateScrollChild();

    this->m_updateScrollChild = 1;
}

void CSimpleScrollFrame::SetVerticalScroll(float offset) {
    if (fabs(offset - this->m_scrollOffset.y) >= 0.00000095367432) {
        this->m_scrollOffset.y = offset;
        this->UpdateScrollChild();
        this->RunOnVerticalScrollScript();
    }
}

void CSimpleScrollFrame::UpdateScrollChild() {
    if (!this->m_scrollChild) {
        return;
    }

    this->m_scrollChild->ClearAllPoints();
    this->m_scrollChild->SetPoint(
        FRAMEPOINT_TOPLEFT,
        this,
        FRAMEPOINT_TOPLEFT,
        -this->m_scrollOffset.x,
        this->m_scrollOffset.y,
        1
    );
}

void CSimpleScrollFrame::UpdateScrollChildRect(float w, float h) {
    if (!this->m_scrollChild) {
        return;
    }

    float rangeX = this->m_scrollRange.x;
    float rangeY = this->m_scrollRange.y;

    CRect childBounds = {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        0.0f,
        0.0f
    };

    this->m_scrollChild->GetBoundsRect(childBounds);

    auto ooScale = 1.0f / this->m_layoutScale;

    auto v7 = std::max(childBounds.maxX - childBounds.minX - w, 0.0f);
    this->m_scrollRange.x = v7 * ooScale;

    auto v12 = std::max(childBounds.maxY - childBounds.minY - h, 0.0f);
    this->m_scrollRange.y = v12 * ooScale;

    if (
        fabs(this->m_scrollRange.x - rangeX) >= 0.00000095367432
        || fabs(this->m_scrollRange.y - rangeY) >= 0.00000095367432
    ) {
        this->RunOnScrollRangeChangedScript();
    }
}
