#include "gx/Screen.hpp"
#include "event/Event.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Draw.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/Transform.hpp"
#include "util/Filesystem.hpp"
#include <storm/String.hpp>
#include <tempest/Matrix.hpp>

int32_t Screen::s_captureScreen = 0;
float Screen::s_elapsedSec = 0.0f;
int32_t Screen::s_presentDisable = 0;
static HOBJECT s_stockObjects[SCRNSTOCKOBJECTS];
static float s_stockObjectHeights[SCRNSTOCKOBJECTS] = { 0.01953125f, 0.01953125f };
static STORM_EXPLICIT_LIST(CILayer, zorderlink) s_zOrderList;

int32_t OnIdle(const EVENT_DATA_IDLE* data, void* a2) {
    Screen::s_elapsedSec = data->elapsedSec + Screen::s_elapsedSec;

    return 1;
}

int32_t OnPaint(const void* a1, void* a2) {
    // TODO
    // if (!g_theGxDevicePtr || !g_theGxDevicePtr->CapsHasContext(-1) || !g_theGxDevicePtr->CapsIsWindowVisible(-1)) {
    //     // TODO
    //     // - sound engine logic
    //
    //     return 1;
    // }

    CSRgn rgn;
    SRgnCreate(&rgn.m_handle, 0);

    RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
    SRgnCombineRectf(rgn.m_handle, &baseRect, nullptr, 2);

    // Walk the layer list backward (highest z-order to lowest) to establish visibility rects
    for (auto layer = s_zOrderList.Tail(); layer; layer = layer->zorderlink.Prev()) {
        SRgnGetBoundingRectf(rgn.m_handle, &layer->visible);

        layer->visible.left = std::max(layer->visible.left, layer->rect.left);
        layer->visible.bottom = std::max(layer->visible.bottom, layer->rect.bottom);
        layer->visible.right = std::min(layer->visible.right, layer->rect.right);
        layer->visible.top = std::min(layer->visible.top, layer->rect.top);

        if (!(layer->flags & 0x1)) {
            SRgnCombineRectf(rgn.m_handle, &layer->rect, nullptr, 4);
        }
    }

    SRgnDelete(rgn.m_handle);

    // Save viewport
    float minX, maxX, minY, maxY, minZ, maxZ;
    GxXformViewport(minX, maxX, minY, maxY, minZ, maxZ);

    // Walk the layer list forward (lowest z-order to highest) to paint visible layers
    for (auto layer = s_zOrderList.Head(); layer; layer = layer->zorderlink.Next()) {
        if (layer->visible.right > layer->visible.left && layer->visible.top > layer->visible.bottom) {
            if (layer->flags & 0x4) {
                GxXformSetViewport(
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f,
                    0.0f,
                    1.0f
                );
            } else {
                GxXformSetViewport(
                    layer->visible.left,
                    layer->visible.right,
                    layer->visible.bottom,
                    layer->visible.top,
                    0.0f,
                    1.0f
                );
            }

            if (layer->flags & 0x2) {
                C44Matrix identity;
                GxXformSetView(identity);

                C44Matrix orthoProj;
                GxuXformCreateOrtho(
                    layer->visible.left,
                    layer->visible.right,
                    layer->visible.bottom,
                    layer->visible.top,
                    0.0f,
                    500.0f,
                    orthoProj
                );
                GxXformSetProjection(orthoProj);
            }

            layer->paintfunc(
                layer->param,
                &layer->rect,
                &layer->visible,
                Screen::s_elapsedSec
            );
        }
    }

    // Restore viewport
    GxXformSetViewport(minX, maxX, minY, maxY, minZ, maxZ);

    GxuFontUpdate();

    if (!Screen::s_presentDisable) {
        if (Screen::s_captureScreen) {
            // TODO

            GxSub682A00();

            // TODO

            return 1;
        }

        GxSub682A00();
    }

    Screen::s_elapsedSec = 0.0f;

    return 1;
}

void ILayerInitialize() {
    EventRegister(EVENT_ID_IDLE, reinterpret_cast<EVENTHANDLERFUNC>(OnIdle));
    EventRegister(EVENT_ID_PAINT, &OnPaint);
}

void IStockInitialize() {
    GxuFontInitialize();

    char fontFile[STORM_MAX_PATH];
    OsBuildFontFilePath("FRIZQT__.TTF", fontFile, sizeof(fontFile));

    if (*fontFile) {
        ScrnSetStockFont(STOCK_SYSFONT, fontFile);
    } else {
        // TODO
        // SErrSetLastError(0x57u);
    }

    if (*fontFile) {
        ScrnSetStockFont(STOCK_PERFFONT, fontFile);
    } else {
        // TODO
        // SErrSetLastError(0x57u);
    }
}

void ScrnInitialize(int32_t a1) {
    ILayerInitialize();

    // TODO
    // consoleInitialized = a1;

    IStockInitialize();
}

void ScrnLayerCreate(const RECTF* rect, float zOrder, uint32_t flags, void* param, void (*paintFunc)(void*, const RECTF*, const RECTF*, float), HLAYER* layerPtr) {
    static RECTF defaultrect = { 0.0f, 0.0f, 1.0f, 1.0f };
    const RECTF* r = rect ? rect : &defaultrect;

    auto m = SMemAlloc(sizeof(CILayer), __FILE__, __LINE__, 0x0);
    auto layer = new (m) CILayer();

    layer->rect.left = r->left;
    layer->rect.bottom = r->bottom;
    layer->rect.right = r->right;
    layer->rect.top = r->top;

    layer->zorder = zOrder;
    layer->flags = flags;
    layer->param = param;
    layer->paintfunc = paintFunc;

    auto node = s_zOrderList.Head();

    while (node && zOrder < node->zorder) {
        node = node->zorderlink.Next();
    }

    s_zOrderList.LinkNode(layer, 1, node);

    *layerPtr = HandleCreate(layer);
}

void ScrnLayerSetRect(HLAYER layer, const RECTF* rect) {
    static_cast<CILayer*>(HandleDereference(layer))->rect = *rect;
}

void ScrnSetStockFont(SCRNSTOCK stockID, const char* fontTexturePath) {
    if (s_stockObjects[stockID]) {
        HandleClose(s_stockObjects[stockID]);
    }

    float fontHeight = NDCToDDCHeight(s_stockObjectHeights[stockID]);
    HTEXTFONT font = TextBlockGenerateFont(fontTexturePath, 0, fontHeight);
    s_stockObjects[stockID] = font;
}
