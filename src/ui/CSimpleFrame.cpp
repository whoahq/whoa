#include "ui/CSimpleFrame.hpp"
#include "event/Event.hpp"
#include "event/Input.hpp"
#include "gx/Coordinate.hpp"
#include "math/Utils.hpp"
#include "ui/CBackdropGenerator.hpp"
#include "ui/CScriptRegion.hpp"
#include "ui/CSimpleFontString.hpp"
#include "ui/CSimpleFrameScript.hpp"
#include "ui/CSimpleRender.hpp"
#include "ui/CSimpleScrollFrame.hpp"
#include "ui/CSimpleTitleRegion.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/CSimpleTop.hpp"
#include "ui/FrameScript_Object.hpp"
#include "ui/FrameXML.hpp"
#include "ui/LoadXML.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include <algorithm>
#include <common/XML.hpp>
#include <storm/Error.hpp>
#include <storm/String.hpp>
#include <storm/Unicode.hpp>

int32_t CSimpleFrame::s_metatable;
int32_t CSimpleFrame::s_objectType;

int32_t CSimpleFrame::GetObjectType() {
    if (!CSimpleFrame::s_objectType) {
        CSimpleFrame::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleFrame::s_objectType;
}

void CSimpleFrame::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleFrame::RegisterScriptMethods);
    CSimpleFrame::s_metatable = ref;
}

void CSimpleFrame::RegisterScriptMethods(lua_State* L) {
    CScriptRegion::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleFrameMethods, NUM_SIMPLE_FRAME_SCRIPT_METHODS);
}

CSimpleFrame::CSimpleFrame(CSimpleFrame* parent) : CScriptRegion() {
    // TODO
    // other constructor logic

    this->m_top = CSimpleTop::s_instance;
    this->m_top->RegisterFrame(this);

    this->SetParent(parent);

    this->m_drawenabled[DRAWLAYER_BACKGROUND]           = 1;
    this->m_drawenabled[DRAWLAYER_BACKGROUND_BORDER]    = 1;
    this->m_drawenabled[DRAWLAYER_ARTWORK]              = 1;
    this->m_drawenabled[DRAWLAYER_ARTWORK_OVERLAY]      = 1;
    this->m_drawenabled[DRAWLAYER_HIGHLIGHT]            = 0;

    this->Show();
}

CSimpleFrame::~CSimpleFrame() {
    // TODO

    this->m_intAC = 3;

    this->m_top->UnregisterFrame(this);

    // TODO
}

void CSimpleFrame::AddFrameRegion(CSimpleRegion* region, uint32_t drawlayer) {
    region->SetLayoutScale(this->m_layoutScale, 0);
    this->m_drawlayers[drawlayer].LinkToTail(region);
    this->NotifyDrawLayerChanged(drawlayer);
}

void CSimpleFrame::DisableDrawLayer(uint32_t drawlayer) {
    this->m_drawenabled[drawlayer] = 0;
    this->NotifyDrawLayerChanged(drawlayer);
}

void CSimpleFrame::EnableDrawLayer(uint32_t drawlayer) {
    this->m_drawenabled[drawlayer] = 1;
    this->NotifyDrawLayerChanged(drawlayer);
}

void CSimpleFrame::LoadXML(XMLNode* node, CStatus* status) {
    const char* inherits = node->GetAttributeByName("inherits");

    const char** v68 = &inherits;
    char inheritName[1024];

    if (inherits && *inherits) {
        do {
            SStrTokenize(v68, inheritName, 0x400u, " ,", nullptr);

            if (!*inheritName) {
                break;
            }

            const char* tainted;
            bool locked;

            XMLNode* inheritNode = FrameXML_AcquireHashNode(inheritName, tainted, locked);

            if (inheritNode) {
                if (locked) {
                    status->Add(STATUS_WARNING, "Recursively inherited node: %s", inheritName);
                } else {
                    // TODO lua taint management

                    this->LoadXML(inheritNode, status);

                    // TODO lua taint management

                    FrameXML_ReleaseHashNode(inheritName);
                }
            } else {
                status->Add(STATUS_WARNING, "Couldn't find inherited node: %s", inheritName);
            }

        } while (*inheritName);
    }

    CScriptRegion::LoadXML(node, status);

    // hidden

    const char* hidden = node->GetAttributeByName("hidden");

    if (hidden && *hidden) {
        if (StringToBOOL(hidden)) {
            this->Hide();
        } else {
            this->Show();
        }
    }

    // toplevel

    const char* toplevel = node->GetAttributeByName("toplevel");

    if (toplevel && *toplevel) {
        this->SetFrameFlag(0x1, StringToBOOL(toplevel));
    }

    // movable

    const char* movable = node->GetAttributeByName("movable");

    if (movable && *movable) {
        this->SetFrameFlag(0x100, StringToBOOL(movable));
    }

    // dontSavePosition

    const char* dontSavePosition = node->GetAttributeByName("dontSavePosition");

    if (dontSavePosition && *dontSavePosition) {
        this->SetFrameFlag(0x80000, StringToBOOL(dontSavePosition));
    }

    // resizable

    const char* resizable = node->GetAttributeByName("resizable");

    if (resizable && *resizable) {
        this->SetFrameFlag(0x200, StringToBOOL(resizable));
    }

    // frameStrata

    const char* frameStrata = node->GetAttributeByName("frameStrata");

    if (frameStrata && *frameStrata) {
        FRAME_STRATA strata;

        if (StringToFrameStrata(frameStrata, strata)) {
            this->SetFrameStrata(strata);
        } else {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown frame strata: %s",
                this->GetName() || "<unnamed>",
                frameStrata
            );
        }
    }

    // frameLevel

    const char* frameLevel = node->GetAttributeByName("frameLevel");

    if (frameLevel && *frameLevel) {
        int32_t level = SStrToInt(frameLevel);

        if (level > 0) {
            this->SetFrameLevel(level, 1);
        } else {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown frame level: %s",
                this->GetName() || "<unnamed>",
                frameLevel
            );
        }
    }

    // TODO alpha

    // enableMouse

    const char* enableMouse = node->GetAttributeByName("enableMouse");

    if (enableMouse && *enableMouse && StringToBOOL(enableMouse)) {
        this->EnableEvent(SIMPLE_EVENT_MOUSE, -1);
    }

    // enableKeyboard

    const char* enableKeyboard = node->GetAttributeByName("enableKeyboard");

    if (enableKeyboard && *enableKeyboard && StringToBOOL(enableKeyboard)) {
        this->EnableEvent(SIMPLE_EVENT_CHAR, -1);
        this->EnableEvent(SIMPLE_EVENT_KEY, -1);
    }

    // TODO clampedToScreen

    // TODO protected

    // depth

    const char* depth = node->GetAttributeByName("depth");

    if (depth && *depth) {
        float d = SStrToFloat(depth);

        if (fabs(d - this->m_depth) >= 0.00000023841858) {
            this->m_depth = d;
            this->UpdateDepth(false);
        }
    }

    // Children

    XMLNode* child = node->m_child;

    while (child) {
        if (!SStrCmpI(child->GetName(), "TitleRegion", 0x7FFFFFFFu)) {
            // TODO
        }

        if (!SStrCmpI(child->GetName(), "ResizeBounds", 0x7FFFFFFFu)) {
            // TODO
        }

        if (!SStrCmpI(child->GetName(), "Backdrop", 0x7FFFFFFFu)) {
            this->LoadXML_Backdrop(child, status);
        }

        if (!SStrCmpI(child->GetName(), "HitRectInsets", 0x7FFFFFFFu)) {
            // TODO
        }

        if (!SStrCmpI(child->GetName(), "Layers", 0x7FFFFFFFu)) {
            this->LoadXML_Layers(child, status);
        }

        if (!SStrCmpI(child->GetName(), "Attributes", 0x7FFFFFFFu)) {
            this->LoadXML_Attributes(child, status);
        }

        if (!SStrCmpI(child->GetName(), "Scripts", 0x7FFFFFFFu)) {
            this->LoadXML_Scripts(child, status);
        }

        child = child->m_next;
    }
}

void CSimpleFrame::PostLoadXML(XMLNode* node, CStatus* status) {
    this->m_loading = 0;

    if (this->m_visible) {
        this->SetDeferredResize(0);

        CSimpleTitleRegion* titleRegion = this->m_titleRegion;

        if (titleRegion) {
            titleRegion->SetDeferredResize(0);
        }

        CSimpleRegion* region = this->m_regions.Head();

        while (region) {
            region->SetDeferredResize(0);
            region = region->m_regionLink.Next();
        }
    }

    this->PostLoadXML_Frames(node, status);

    // TODO
    // Something to do with alpha animations
    //
    // int32_t v9 = CSimpleAlphaAnim::GetObjectType();
    // this->sub4887F0(v9, &v13, &v14);
    //
    // if (v13) {
    //     this->Vfptr26();
    //
    //     v10 = LOBYTE(v4->simpleframe_unk3);
    //     HIBYTE(v4->simpleframe_unk3) ^= (HIBYTE(v4->simpleframe_unk3) ^ 2 * v14) & 2;
    //     BYTE2(v4->simpleframe_unk3) = v10 * BYTE1(v4->simpleframe_unk3) / 255;
    //
    //     auto region = this->m_regions.Head();
    //     while (region) {
    //         region->Vfptr15(0, 1, v14);
    //         region = region->m_regionLink.Next();
    //     }
    // }

    this->RunOnLoadScript();

    if (this->m_visible) {
        this->RunOnShowScript();
    }
}

void CSimpleFrame::Raise() {
    this->m_top->RaiseFrame(this, 1);
}

void CSimpleFrame::RegisterForEvents(int32_t a2) {
    for (int32_t event = SIMPLE_EVENT_CHAR; event < NUM_SIMPLE_EVENTS; event++) {
        if (this->m_eventmask & (1 << event)) {
            this->m_top->RegisterForEvent(this, static_cast<CSimpleEventType>(event), a2, -1);
        }
    }
}

void CSimpleFrame::RunOnCharScript(const char* chr) {
    if (this->m_onChar.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushstring(L, chr);

        this->RunScript(this->m_onChar, 1, 0);
    }
}

void CSimpleFrame::RunOnEnterScript(int32_t a2) {
    if (this->m_onEnter.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushboolean(L, a2);

        this->RunScript(this->m_onEnter, 1, 0);
    }
}

void CSimpleFrame::RunOnHideScript() {
    if (this->m_onHide.luaRef && !this->m_loading) {
        this->RunScript(this->m_onHide, 0, 0);
    }
}

void CSimpleFrame::RunOnKeyDownScript(const char* key) {
    if (this->m_onKeyDown.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushstring(L, key);

        this->RunScript(this->m_onKeyDown, 1, 0);
    }
}

void CSimpleFrame::RunOnKeyUpScript(const char* key) {
    if (this->m_onKeyUp.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushstring(L, key);

        this->RunScript(this->m_onKeyUp, 1, 0);
    }
}

void CSimpleFrame::RunOnLeaveScript(int32_t a2) {
    if (this->m_onLeave.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushboolean(L, a2);

        this->RunScript(this->m_onLeave, 1, 0);
    }
}

void CSimpleFrame::RunOnLoadScript() {
    if (this->m_onLoad.luaRef) {
        this->RunScript(this->m_onLoad, 0, 0);
    }
}

void CSimpleFrame::RunOnMouseDownScript(const char* btn) {
    if (this->m_onMouseDown.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushstring(L, btn);

        this->RunScript(this->m_onMouseDown, 1, 0);
    }
}

void CSimpleFrame::RunOnMouseUpScript(const char* btn) {
    if (this->m_onMouseUp.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushstring(L, btn);

        this->RunScript(this->m_onMouseUp, 1, 0);
    }
}

void CSimpleFrame::RunOnShowScript() {
    if (this->m_onShow.luaRef && !this->m_loading) {
        this->RunScript(this->m_onShow, 0, 0);
    }
}

void CSimpleFrame::RunOnSizeChangedScript(float width, float height) {
    if (this->m_onSizeChanged.luaRef) {
        auto L = FrameScript_GetContext();

        float ddcWidth = CoordinateGetAspectCompensation() * 1024.0f * width;
        float ndcWidth = DDCToNDCWidth(ddcWidth);
        lua_pushnumber(L, ndcWidth);

        float ddcHeight = CoordinateGetAspectCompensation() * 1024.0f * height;
        float ndcHeight = DDCToNDCWidth(ddcHeight);
        lua_pushnumber(L, ndcHeight);

        this->RunScript(this->m_onSizeChanged, 2, 0);
    }
}

void CSimpleFrame::RunOnUpdateScript(float elapsedSec) {
    if (this->m_onUpdate.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushnumber(L, elapsedSec);
        this->RunScript(this->m_onUpdate, 1, nullptr);
    }
}

void CSimpleFrame::PreLoadXML(XMLNode* node, CStatus* status) {
    const char* name = node->GetAttributeByName("name");

    if (name && *name) {
        this->SetName(name);
    }

    const char* id = node->GetAttributeByName("id");

    if (id && *id) {
        int32_t idNum = SStrToInt(id);

        if (idNum >= 0) {
            this->m_id = idNum;
        }
    }

    this->m_loading = 1;

    this->SetDeferredResize(1);

    CSimpleTitleRegion* titleRegion = this->m_titleRegion;

    if (titleRegion) {
        titleRegion->SetDeferredResize(1);
    }

    CSimpleRegion* region = this->m_regions.Head();

    while (region) {
        region->SetDeferredResize(1);
        region = region->m_regionLink.Next();
    }
}

int32_t CSimpleFrame::GetBoundsRect(CRect& bounds) {
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
        if (child->frame->m_shown) {
            child->frame->GetBoundsRect(bounds);
        }
    }

    return bounds.maxY > bounds.minY && bounds.maxX > bounds.minX;
}

int32_t CSimpleFrame::GetHitRect(CRect& rect) {
    if (!(this->CLayoutFrame::m_flags & 0x1)) {
        return 0;
    }

    rect = this->m_hitRect;

    return 1;
}

FrameScript_Object::ScriptIx* CSimpleFrame::GetScriptByName(const char* name, FrameScript_Object::ScriptData& data) {
    auto parentScript = FrameScript_Object::GetScriptByName(name, data);

    if (parentScript) {
        return parentScript;
    }

    if (!SStrCmpI(name, "OnLoad", 0x7FFFFFFFu)) {
        return &this->m_onLoad;
    }

    if (!SStrCmpI(name, "OnSizeChanged", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,w,h) %s end";
        return &this->m_onSizeChanged;
    }

    if (!SStrCmpI(name, "OnUpdate", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,elapsed) %s end";
        return &this->m_onUpdate;
    }

    if (!SStrCmpI(name, "OnShow", 0x7FFFFFFFu)) {
        return &this->m_onShow;
    }

    if (!SStrCmpI(name, "OnHide", 0x7FFFFFFFu)) {
        return &this->m_onHide;
    }

    if (!SStrCmpI(name, "OnEnter", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,motion) %s end";
        return &this->m_onEnter;
    }

    if (!SStrCmpI(name, "OnLeave", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,motion) %s end";
        return &this->m_onLeave;
    }

    if (!SStrCmpI(name, "OnMouseDown", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,button) %s end";
        return &this->m_onMouseDown;
    }

    if (!SStrCmpI(name, "OnMouseUp", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,button) %s end";
        return &this->m_onMouseUp;
    }

    if (!SStrCmpI(name, "OnMouseWheel", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,delta) %s end";
        return &this->m_onMouseWheel;
    }

    if (!SStrCmpI(name, "OnDragStart", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,button) %s end";
        return &this->m_onDragStart;
    }

    if (!SStrCmpI(name, "OnDragStop", 0x7FFFFFFFu)) {
        return &this->m_onDragStop;
    }

    if (!SStrCmpI(name, "OnReceiveDrag", 0x7FFFFFFFu)) {
        return &this->m_onReceiveDrag;
    }

    if (!SStrCmpI(name, "OnChar", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,text) %s end";
        return &this->m_onChar;
    }

    if (!SStrCmpI(name, "OnKeyDown", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,key) %s end";
        return &this->m_onKeyDown;
    }

    if (!SStrCmpI(name, "OnKeyUp", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,key) %s end";
        return &this->m_onKeyUp;
    }

    if (!SStrCmpI(name, "OnAttributeChanged", 0x7FFFFFFFu)) {
        data.wrapper = "return function(self,name,value) %s end";
        return &this->m_onAttributeChange;
    }

    if (!SStrCmpI(name, "OnEnable", 0x7FFFFFFFu)) {
        return &this->m_onEnable;
    }

    if (!SStrCmpI(name, "OnDisable", 0x7FFFFFFFu)) {
        return &this->m_onDisable;
    }

    return nullptr;
}

int32_t CSimpleFrame::GetScriptMetaTable() {
    return CSimpleFrame::s_metatable;
}

bool CSimpleFrame::IsA(int32_t type) {
    return type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleFrame::EnableEvent(CSimpleEventType eventType, int32_t priority) {
    if ((1 << eventType) & this->m_eventmask) {
        return;
    }

    if (this->m_visible) {
        this->m_top->RegisterForEvent(this, eventType, 0, priority);
    }

    this->m_eventmask |= 1 << eventType;
}

void CSimpleFrame::Hide() {
    if (this->ProtectedFunctionsAllowed()) {
        this->m_shown = 0;
        this->HideThis();
    } else {
        // TODO
        // auto v2 = this->m_top->unk5[8];

        // if (v2) {
        //     v2(0);
        // }
    }
}

int32_t CSimpleFrame::HideThis() {
    if (!this->m_visible) {
        return 1;
    }

    if (!this->m_loading) {
        this->SetDeferredResize(1);

        if (this->m_titleRegion) {
            this->m_titleRegion->SetDeferredResize(1);
        }
    }

    this->m_visible = 0;
    this->m_top->HideFrame(this, 0);

    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->HideThis();
    }

    auto child = this->m_children.Head();

    while (child) {
        child->frame->HideThis();
        child = this->m_children.Link(child)->Next();
    }

    this->OnLayerHide();

    return 1;
}

void CSimpleFrame::LoadXML_Attributes(XMLNode* node, CStatus* status) {
    // TODO
}

void CSimpleFrame::LoadXML_Backdrop(XMLNode* node, CStatus* status) {
    auto m = SMemAlloc(sizeof(CBackdropGenerator), __FILE__, __LINE__, 0x0);
    auto backdrop = new (m) CBackdropGenerator();

    backdrop->LoadXML(node, status);
    this->SetBackdrop(backdrop);
}

void CSimpleFrame::LoadXML_Layers(XMLNode* node, CStatus* status) {
    if (!node->m_child) {
        return;
    }

    XMLNode* layerNode = node->m_child;

    while (layerNode) {
        if (SStrCmpI(layerNode->GetName(), "Layer", 0x7FFFFFFFu)) {
            const char* layerNodeName = layerNode->GetName();
            const char* nodeName = node->GetName();
            const char* frameName = this->GetName();

            if (!frameName) {
                frameName = "<unnamed>";
            }

            status->Add(STATUS_WARNING, "Frame %s: Unknown child node in %s element: %s", frameName, nodeName, layerNodeName);
        } else {
            int32_t drawlayer = 2;

            const char* levelStr = layerNode->GetAttributeByName("level");

            if (levelStr && *levelStr) {
                StringToDrawLayer(levelStr, drawlayer);
            }

            XMLNode* layerChild;

            for (layerChild = layerNode->m_child; layerChild; layerChild = layerChild->m_next) {
                if (SStrCmpI(layerChild->GetName(), "Texture", 0x7FFFFFFFu)) {
                    if (SStrCmpI(layerChild->GetName(), "FontString", 0x7FFFFFFFu)) {
                        const char* layerChildName = layerChild->GetName();
                        const char* layerNodeName = layerNode->GetName();
                        const char* frameName = this->GetName();

                        if (!frameName) {
                            frameName = "<unnamed>";
                        }

                        status->Add(STATUS_WARNING, "Frame %s: Unknown child node in %s element: %s", frameName, layerNodeName, layerChildName);
                    } else {
                        CSimpleFontString* layerString = LoadXML_String(layerChild, this, status);
                        layerString->SetFrame(this, drawlayer, layerString->m_shown);
                    }
                } else {
                    CSimpleTexture* layerTexture = LoadXML_Texture(layerChild, this, status);
                    layerTexture->SetFrame(this, drawlayer, layerTexture->m_shown);
                }
            }
        }

        layerNode = layerNode->m_next;
    }
}

void CSimpleFrame::LoadXML_Scripts(XMLNode* root, CStatus* status) {
    lua_State* L = FrameScript_GetContext();

    const char* scriptName;
    const char* scriptBody;
    ScriptData scriptData;
    ScriptIx* script;

    XMLNode* node = root->m_child;

    while (node) {
        scriptName = node->GetName();
        scriptBody = node->m_body;
        scriptData.wrapper = "return function(self) %s end";
        script = this->GetScriptByName(scriptName, scriptData);

        if (script) {
            if (script->luaRef) {
                luaL_unref(L, LUA_REGISTRYINDEX, script->luaRef);
            }

            if (!node->m_userData && scriptBody && *scriptBody) {
                char compileName[1024];

                SStrPrintf(compileName, 1024, "*:%s", scriptName);

                void* scriptFunction = SMemAlloc(sizeof(ScriptFunction), __FILE__, __LINE__, 0);

                if (scriptFunction) {
                    int32_t luaRef = FrameScript_CompileFunction(compileName, scriptData.wrapper, scriptBody, status);

                    scriptFunction = new (scriptFunction) ScriptFunction();
                    static_cast<ScriptFunction*>(scriptFunction)->luaRef = luaRef;
                } else {
                    scriptFunction = nullptr;
                }

                node->m_userData = scriptFunction;
            }

            const char* functionLookup = node->GetAttributeByName("function");

            // Set script to already defined function with matching name
            if (functionLookup && *functionLookup) {
                int32_t luaRef;
                const char* frameName;

                lua_pushstring(L, functionLookup);
                lua_rawget(L, LUA_GLOBALSINDEX);

                luaRef = luaL_ref(L, LUA_REGISTRYINDEX);

                if (luaRef == -1) {
                    frameName = this->GetName();

                    if (!frameName) {
                        frameName = "<unnamed>";
                    }

                    status->Add(STATUS_WARNING, "Frame %s: Unknown function %s in element %s", frameName, functionLookup, scriptName);

                    script->luaRef = 0;
                } else {
                    script->luaRef = luaRef;
                }
            // Set script to function compiled from body of script element
            // eg. <OnLoad>foo();</OnLoad>
            } else if (node->m_userData) {
                lua_rawgeti(L, LUA_REGISTRYINDEX, static_cast<ScriptFunction*>(node->m_userData)->luaRef);
                script->luaRef = luaL_ref(L, LUA_REGISTRYINDEX);
            // Disable script
            } else {
                script->luaRef = 0;
            }

            script->unk = nullptr;

            if (!SStrCmpI(node->GetName(), "OnChar", 0x7FFFFFFFu) && !(this->m_eventmask & 1)) {
                if (this->m_visible) {
                    this->m_top->RegisterForEvent(this, SIMPLE_EVENT_CHAR, 0, -1);
                }

                this->m_eventmask |= 1u;
            }

            if ((!SStrCmpI(node->GetName(), "OnKeyDown", 0x7FFFFFFFu) || !SStrCmpI(node->GetName(), "OnKeyUp", 0x7FFFFFFFu)) && !(this->m_eventmask & 2)) {
                if (this->m_visible) {
                    this->m_top->RegisterForEvent(this, SIMPLE_EVENT_KEY, 0, -1);
                }

                this->m_eventmask |= 2u;
            }

            if ((!SStrCmpI(node->GetName(), "OnEnter", 0x7FFFFFFFu) || !SStrCmpI(node->GetName(), "OnLeave", 0x7FFFFFFFu) || !SStrCmpI(node->GetName(), "OnMouseDown", 0x7FFFFFFFu) || !SStrCmpI(node->GetName(), "OnMouseUp", 0x7FFFFFFFu) || !SStrCmpI(node->GetName(), "OnDragStart", 0x7FFFFFFFu)) && !(this->m_eventmask & 4)) {
                if (this->m_visible) {
                    this->m_top->RegisterForEvent(this, SIMPLE_EVENT_MOUSE, 0, -1);
                }

                this->m_eventmask |= 4u;
            }

            if (!SStrCmpI(node->GetName(), "OnMouseWheel", 0x7FFFFFFFu) && !(this->m_eventmask & 8)) {
                if (this->m_visible) {
                    this->m_top->RegisterForEvent(this, SIMPLE_EVENT_MOUSEWHEEL, 0, -1);
                }

                this->m_eventmask |= 8u;
            }
        } else {
            const char* frameName = this->GetName();

            if (!frameName) {
                frameName = "<unnamed>";
            }

            status->Add(STATUS_WARNING, "Frame %s: Unknown script element %s", frameName, scriptName);
        }

        node = node->m_next;
    }
}

void CSimpleFrame::LockHighlight(int32_t lock) {
    if (this->m_highlightLocked == lock) {
        return;
    }

    this->m_highlightLocked = lock;

    if (lock) {
        this->EnableDrawLayer(DRAWLAYER_HIGHLIGHT);
    } else {
        if (this->m_top->m_mouseFocus != this) {
            this->DisableDrawLayer(DRAWLAYER_HIGHLIGHT);
        }
    }
}

void CSimpleFrame::NotifyDrawLayerChanged(uint32_t drawlayer) {
    if (this->m_flags & 0x2000) {
        this->m_batchDirty |= 1 << drawlayer;
    } else if (this->m_top && this->m_visible) {
        this->m_top->NotifyFrameLayerChanged(this, drawlayer);
    }

    this->NotifyScrollParent();
}

void CSimpleFrame::NotifyScrollParent() {
    if (!(this->m_flags & 0x2000) || !this->m_visible) {
        return;
    }

    auto parent = this;
    while (!(parent->m_flags & 0x4000)) {
        parent = parent->m_parent;
    }
    parent = parent->m_parent;

    if (parent && parent->m_intAC != 3) {
        static_cast<CSimpleScrollFrame*>(parent)->m_updateScrollChild = 1;
    }
}

void CSimpleFrame::OnFrameRender() {
    auto batchDirty = this->m_batchDirty;
    this->m_batchDirty = 0;

    if (batchDirty) {
        for (int32_t layer = 0; layer < NUM_SIMPLEFRAME_DRAWLAYERS; layer++) {
            auto batch = this->m_batch[layer];

            if (!batch) {
                auto mem = SMemAlloc(sizeof(CRenderBatch), __FILE__, __LINE__, 0x0);
                batch = new (mem) CRenderBatch();
                this->m_batch[layer] = batch;
            }

            this->m_renderList.Link(batch)->Unlink();

            if ((1 << layer) & batchDirty) {
                batch->Clear();
                this->OnFrameRender(batch, layer);
                batch->Finish();
            }

            if (batch->m_count) {
                this->m_renderList.LinkToTail(batch);
            }
        }
    }

    for (int32_t layer = 0; layer < NUM_SIMPLEFRAME_DRAWLAYERS; layer++) {
        if (this->m_batch[layer]) {
            CSimpleRender::DrawBatch(this->m_batch[layer]);
        }
    }

    for (auto child = this->m_children.Head(); child; child = this->m_children.Link(child)->Next()) {
        if (child->frame->m_visible  && !(child->frame->m_flags & 0x4000)) {
            child->frame->OnFrameRender();
        }
    }
}

void CSimpleFrame::OnFrameRender(CRenderBatch* batch, uint32_t layer) {
    if (!this->m_drawenabled[layer]) {
        return;
    }

    for (auto region = this->m_drawlayers[layer].Head(); region; region = this->m_drawlayers[layer].Link(region)->Next()) {
        region->Draw(batch);
    }
}

void CSimpleFrame::OnFrameSizeChanged(const CRect& rect) {
    CLayoutFrame::OnFrameSizeChanged(rect);

    this->SetHitRect();

    if (
        fabs(rect.maxX - rect.minX - (this->m_rect.maxX - this->m_rect.minX)) >= WHOA_EPSILON_1
        || fabs(rect.maxY - rect.minY - (this->m_rect.maxY - this->m_rect.minY)) >= WHOA_EPSILON_1
    ) {
        if (this->m_backdrop) {
            this->m_backdrop->Generate(&this->m_rect);
        }

        float v4 = 1.0f / this->m_layoutScale;
        float v6 = v4 * (this->m_rect.maxX - this->m_rect.minX);
        float v7 = v4 * (this->m_rect.maxY - this->m_rect.minY);

        this->OnFrameSizeChanged(v6, v7);
    }

    this->m_top->NotifyFrameMovedOrResized(this);
}

void CSimpleFrame::OnFrameSizeChanged(float width, float height) {
    this->RunOnSizeChangedScript(width, height);
    this->NotifyScrollParent();
}

int32_t CSimpleFrame::OnLayerChar(const CCharEvent& evt) {
    if (!this->m_visible) {
        return 0;
    }

    if (!this->m_onChar.luaRef) {
        return 0;
    }

    char charBuf[8];
    SUniSPutUTF8(evt.ch, charBuf);

    this->RunOnCharScript(charBuf);

    return 1;
}

void CSimpleFrame::OnLayerCursorEnter(int32_t a2) {
    // TODO
    // if (this->m_tooltip) {
    //     TODO
    // }

    if (!this->m_highlightLocked) {
        this->m_drawenabled[DRAWLAYER_HIGHLIGHT] = 1;
        this->NotifyDrawLayerChanged(DRAWLAYER_HIGHLIGHT);
    }

    this->RunOnEnterScript(a2);
}

void CSimpleFrame::OnLayerCursorExit(int32_t a2, int32_t a3) {
    // TODO
    // if (this->m_tooltip) {
    //     TODO
    // }

    if (!this->m_highlightLocked) {
        this->m_drawenabled[DRAWLAYER_HIGHLIGHT] = 0;
        this->NotifyDrawLayerChanged(DRAWLAYER_HIGHLIGHT);
    }

    if (a3) {
        this->m_mouseDown = 0;
        this->m_dragging = 0;
    }

    this->RunOnLeaveScript(a2);
}

void CSimpleFrame::OnLayerHide() {
    this->RunOnHideScript();
    this->NotifyScrollParent();
}

int32_t CSimpleFrame::OnLayerKeyDown(const CKeyEvent& evt) {
    if (!this->m_visible) {
        return 0;
    }

    if (!this->m_onKeyDown.luaRef && !this->m_onKeyUp.luaRef) {
        return 0;
    }

    const char* keyName = KeyCodeToString(evt.key);
    this->RunOnKeyDownScript(keyName);

    return 1;
}

int32_t CSimpleFrame::OnLayerKeyDownRepeat(const CKeyEvent& evt) {
    return 0;
}

int32_t CSimpleFrame::OnLayerKeyUp(const CKeyEvent& evt) {
    if (!this->m_visible || !this->m_onKeyUp.luaRef) {
        return 0;
    }

    const char* keyName = KeyCodeToString(evt.key);
    this->RunOnKeyUpScript(keyName);

    return 1;
}

int32_t CSimpleFrame::OnLayerMouseDown(const CMouseEvent& evt, const char* btn) {
    if (!btn) {
        if (this->m_lookForDrag & evt.button) {
            this->m_mouseDown = 1;
            this->m_dragging = 0;
            this->m_dragButton = evt.button;
            this->m_clickPoint.x = evt.x;
            this->m_clickPoint.y = evt.y;
        }

        btn = GetButtonName(evt.button);
    }

    // TODO
    // const char* old1234 = this->m_top->m_char1234;
    // this->m_top->m_char1234 = btn;

    this->RunOnMouseDownScript(btn);

    // TODO
    // this->m_top->m_char1234 = old1234;

    return 0;
}

int32_t CSimpleFrame::OnLayerMouseUp(const CMouseEvent& evt, const char* btn) {
    if (!btn) {
        if (this->m_lookForDrag & evt.button) {
            this->m_mouseDown = 0;

            if (this->m_dragging) {
                // TODO

                this->m_dragging = 0;

                return 1;
            }
        }

        btn = GetButtonName(evt.button);
    }

    // TODO
    // const char* old1234 = this->m_top->m_char1234;
    // this->m_top->m_char1234 = btn;

    this->RunOnMouseUpScript(btn);

    // TODO
    // this->m_top->m_char1234 = old1234;

    return 0;
}

void CSimpleFrame::OnLayerShow() {
    this->RunOnShowScript();

    // TODO
    // this->sub48FEA0();
}

int32_t CSimpleFrame::OnLayerTrackUpdate(const CMouseEvent& evt) {
    C2Vector pt = { evt.x, evt.y };

    if (this->m_mouseDown && !this->m_dragging) {
        // TODO
        // - check if dragging logic
    }

    int32_t hit = this->TestHitRect(pt);

    if (hit) {
        // TODO
        // - stereo logic
    }

    return hit;
}

void CSimpleFrame::OnLayerUpdate(float elapsedSec) {
    this->RunOnUpdateScript(elapsedSec);

    this->PreOnAnimUpdate();

    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->PreOnAnimUpdate();
    }

    CScriptRegion::OnLayerUpdate(elapsedSec);

    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->OnLayerUpdate(elapsedSec);
    }
}

void CSimpleFrame::OnScreenSizeChanged() {
    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->OnScreenSizeChanged();
    }
}

void CSimpleFrame::ParentFrame(CSimpleFrame* frame) {
    SIMPLEFRAMENODE* node = this->m_children.NewNode(2, 0, 0x8);
    node->frame = frame;
}

void CSimpleFrame::PostLoadXML_Frames(XMLNode *node, CStatus* status) {
    // Call this function with all inherited nodes

    const char* inheritNames = node->GetAttributeByName("inherits");

    if (inheritNames && *inheritNames) {
        char inheritName[1024];

        do {
            SStrTokenize(&inheritNames, inheritName, 1024, " ,", 0);

            if (!*inheritName) {
                break;
            }

            const char* tainted;
            bool locked;

            XMLNode* inheritNode = FrameXML_AcquireHashNode(inheritName, tainted, locked);

            if (inheritNode && !locked) {
                // TODO
                // FrameScript_GetContext();

                // v6 = lua_taintexpected++ == -1;
                // v12 = lua_tainted;

                // if (!v6 && !lua_taintedclosure) {
                //     lua_tainted = tainted;
                // }

                // const char* v7 = lua_gctag;

                // lua_gctag = tainted;

                this->PostLoadXML_Frames(inheritNode, status);

                // TODO
                // lua_gctag = v7;

                // if (lua_taintexpected && !lua_taintedclosure) {
                //     lua_tainted = v12;
                // }

                // if (--lua_taintexpected <= 0) {
                //     lua_taintexpected = 0;
                // }

                FrameXML_ReleaseHashNode(inheritName);
            }
        } while (*inheritName);
    }

    // Create child frames

    XMLNode* framesNode = node->GetChildByName("Frames");

    if (framesNode) {
        XMLNode* frameNode;

        for (frameNode = framesNode->m_child; frameNode; frameNode = frameNode->m_next) {
            FrameXML_CreateFrame(frameNode, this, status);
        }
    }
}

void CSimpleFrame::PreOnAnimUpdate() {
    // TODO
}

void CSimpleFrame::RegisterRegion(CSimpleRegion* region) {
    STORM_ASSERT(region);

    this->m_regions.LinkToTail(region);
}

void CSimpleFrame::RemoveFrameRegion(CSimpleRegion* region, uint32_t drawlayer) {
    this->m_drawlayers[drawlayer].UnlinkNode(region);
    this->NotifyDrawLayerChanged(drawlayer);
}

void CSimpleFrame::SetBackdrop(CBackdropGenerator* backdrop) {
    if (this->m_backdrop) {
        delete this->m_backdrop;
    }

    if (backdrop) {
        backdrop->SetOutput(this);
    }

    this->m_backdrop = backdrop;
}

void CSimpleFrame::SetBeingScrolled(int32_t a2, int32_t a3) {
    auto visible = this->m_visible;

    if (visible) {
        this->HideThis();
    }

    if (a2) {
        this->m_flags |= 0x2000;
    } else {
        this->m_flags &= ~0x2000;
    }

    if (a3 >= 0) {
        if (a3) {
            this->m_flags |= 0x4000;
        } else {
            this->m_flags &= ~0x4000;
        }
    }

    if (a2) {
        this->m_batchDirty |= 0x1F;
    }

    for (auto child = this->m_children.Head(); child; this->m_children.Link(child)->Next()) {
        if (!(child->frame->m_flags & 0x4000)) {
            child->frame->SetBeingScrolled(a2, -1);
        }
    }

    if (visible) {
        this->ShowThis();
    }
}

void CSimpleFrame::SetFrameAlpha(uint8_t alpha) {
    if (this->m_alpha == alpha) {
        return;
    }

    this->m_alpha = alpha;
    this->UpdateAlpha();
}

void CSimpleFrame::SetFrameFlag(int32_t flag, int32_t on) {
    if (on) {
        this->m_flags |= flag;
    } else {
        this->m_flags &= ~flag;
    }
}

void CSimpleFrame::SetFrameLevel(int32_t level, int32_t shiftChildren) {
    level = std::max(level, 0);

    if (this->m_level == level) {
        return;
    }

    int32_t delta = std::min(level - this->m_level, 128);

    if (this->m_visible) {
        this->m_top->HideFrame(this, 1);
    }

    this->m_level += delta;

    if (this->m_visible) {
        this->m_top->ShowFrame(this, 1);
    }

    if (shiftChildren) {
        for (auto child = this->m_children.Head(); child; child = this->m_children.Link(child)->Next()) {
            child->frame->SetFrameLevel(child->frame->m_level + delta, 1);
        }
    }
}

bool CSimpleFrame::SetFrameScale(float scale, bool a3) {
    if (scale == 0.0f || (!a3 && AreEqual(scale, this->m_frameScale, WHOA_EPSILON_1))) {
        return false;
    }

    this->m_frameScale = scale;

    return this->UpdateScale(a3);
}

void CSimpleFrame::SetFrameStrata(FRAME_STRATA strata) {
    if (this->m_strata == strata) {
        return;
    }

    if (this->m_visible) {
        this->m_top->HideFrame(this, 1);
    }

    this->m_strata = strata;

    if (this->m_visible) {
        this->m_top->ShowFrame(this, 1);
    }

    auto child = this->m_children.Head();

    while (child) {
        child->frame->SetFrameStrata(strata);
        child = this->m_children.Link(child)->Next();
    }
}

void CSimpleFrame::SetHitRect() {
    this->m_hitRect.minX = this->m_rect.minX + this->m_hitOffset.minX * this->m_layoutScale;
    this->m_hitRect.maxX = this->m_rect.maxX - this->m_hitOffset.maxX * this->m_layoutScale;
    this->m_hitRect.minY = this->m_rect.minY + this->m_hitOffset.minY * this->m_layoutScale;
    this->m_hitRect.maxY = this->m_rect.maxY - this->m_hitOffset.maxY * this->m_layoutScale;
}

void CSimpleFrame::SetParent(CSimpleFrame* parent) {
    if (this->m_parent == parent) {
        return;
    }

    if (this->m_parent) {
        // TODO
        // this->OnProtectedDetach(this->m_parent);
        // (this->m_parent->vfptr + 56)(this);
    }

    if (this->m_visible) {
        this->HideThis();
    }

    this->m_parent = parent;

    if (parent) {
        this->SetFrameStrata(parent->m_strata);
        this->SetFrameLevel(this->m_parent->m_level + 1, 1);
        this->UpdateScale(false);

        // TODO
        // alpha stuff?
        // v7 = LOBYTE(this->m_parent->simpleframe_unk3) * BYTE1(this->m_parent->simpleframe_unk3) / 255;
        // if ((_BYTE)v7 != BYTE1(this->simpleframe_unk3)) {
        //     BYTE1(this->simpleframe_unk3) = v7;
        //     (this->vfptr + 53)(this);
        // }

        this->SetBeingScrolled((this->m_parent->m_flags >> 13) & 1, -1);
    } else {
        this->SetFrameStrata(FRAME_STRATA_MEDIUM);
        this->SetFrameLevel(0, 1);
        this->UpdateScale(false);

        // TODO
        // if (BYTE1(this->simpleframe_unk3) != -1) {
        //     BYTE1(this->simpleframe_unk3) = -1;
        //     (this->vfptr + 53)(v3);
        // }

        this->SetBeingScrolled(0, -1);
    }

    if (this->m_parent) {
        this->OnProtectedAttach(this->m_parent);
        this->m_parent->ParentFrame(this);
    }

    if (this->m_shown) {
        if (!this->m_parent || this->m_parent->m_visible) {
            this->ShowThis();
        }
    }
}

void CSimpleFrame::Show() {
    if (this->ProtectedFunctionsAllowed()) {
        this->m_shown = 1;
        this->ShowThis();
    } else {
        // TODO
        // auto v2 = this->m_top->unk5[8];

        // if (v2) {
        //     v2(0);
        // }
    }
}

int32_t CSimpleFrame::ShowThis() {
    if (!this->m_shown) {
        return 0;
    }

    if (this->m_parent && !this->m_parent->m_visible) {
        return 0;
    }

    if (this->m_visible) {
        return 1;
    }

    if (!this->m_loading) {
        this->SetDeferredResize(0);

        if (this->m_titleRegion) {
            this->m_titleRegion->SetDeferredResize(0);
        }
    }

    this->m_visible = 1;
    this->m_top->ShowFrame(this, 0);

    auto region = this->m_regions.Head();

    while (region) {
        region->ShowThis();
        region = this->m_regions.Link(region)->Next();
    }

    auto child = this->m_children.Head();

    while (child) {
        child->frame->ShowThis();
        child = this->m_children.Link(child)->Next();
    }

    if (this->m_flags & 0x1) {
        this->m_top->RaiseFrame(this, 1);
    }

    this->OnLayerShow();

    return 1;
}

int32_t CSimpleFrame::TestHitRect(const C2Vector& pt) {
    if (!(static_cast<CLayoutFrame*>(this)->m_flags & 0x1)) {
        return 0;
    }

    if (!(this->m_flags & 0x2000)) {
        return this->m_hitRect.IsPointInside(pt);
    }

    CSimpleFrame* parent = this->m_parent;
    while (parent && parent->m_flags & 0x2000) {
        parent = parent->m_parent;
    }

    if (!parent) {
        return 0;
    }

    CRect rect = CRect::Intersection(parent->m_hitRect, this->m_hitRect);
    return rect.IsPointInside(pt);
}

void CSimpleFrame::UpdateDepth(bool a2) {
    // TODO
}

bool CSimpleFrame::UpdateScale(bool a2) {
    auto scale = this->m_frameScale;
    if (this->m_parent) {
        scale *= this->m_parent->m_layoutScale;
    }

    if ((!a2 && AreEqual(scale, this->m_layoutScale, WHOA_EPSILON_1)) || scale == 0.0f) {
        return false;
    }

    this->SetLayoutScale(scale, a2);

    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->SetLayoutScale(scale, a2);
    }

    for (auto child = this->m_children.Head(); child; child = this->m_children.Link(child)->Next()) {
        child->frame->UpdateScale(false);
    }

    return true;
}

void CSimpleFrame::UnregisterForEvents(int32_t a2) {
    for (int32_t event = SIMPLE_EVENT_CHAR; event < NUM_SIMPLE_EVENTS; event++) {
        if ((1 << event) & this->m_eventmask) {
            this->m_top->UnregisterForEvent(this, static_cast<CSimpleEventType>(event), a2);
        }
    }
}

void CSimpleFrame::UnregisterRegion(CSimpleRegion* region) {
    STORM_ASSERT(region);

    this->m_regions.UnlinkNode(region);
}

void CSimpleFrame::UpdateAlpha() {
    for (auto region = this->m_regions.Head(); region; region = this->m_regions.Link(region)->Next()) {
        region->OnColorChanged(false);
    }

    auto effectiveAlpha = this->m_alpha * this->alphaBD / 255;

    for (auto child = this->m_children.Head(); child; child = this->m_children.Link(child)->Next()) {
        child->frame->alphaBD = effectiveAlpha;
        child->frame->UpdateAlpha();
    }
}
