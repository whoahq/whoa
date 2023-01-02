#include "gx/Screen.hpp"
#include "event/Event.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Draw.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/Transform.hpp"
#include "util/Filesystem.hpp"
#include <tempest/Matrix.hpp>

int32_t Screen::s_captureScreen = 0;
float Screen::s_elapsedSec = 0.0f;
int32_t Screen::s_presentDisable = 0;
HOBJECT Screen::s_stockObjects[];
float Screen::s_stockObjectHeights[] = { 0.01953125f, 0.01953125f };
STORM_EXPLICIT_LIST(CILayer, zorderlink) Screen::s_zorderlist;

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

    CILayer* layer;

    CSRgn rgn;

    SRgnCreate(&rgn.m_handle, 0);

    RECTF baseRect;

    baseRect.left = 0.0f;
    baseRect.bottom = 0.0f;
    baseRect.right = 1.0f;
    baseRect.top = 1.0f;

    SRgnCombineRectf(&rgn.m_handle, &baseRect, 0, 2);

    layer = Screen::s_zorderlist.Head();

    while (layer) {
        SRgnGetBoundingRectf(&rgn.m_handle, &layer->visible);

        layer->visible.left = std::max(layer->visible.left, layer->rect.left);
        layer->visible.bottom = std::max(layer->visible.bottom, layer->rect.bottom);
        layer->visible.right = std::max(layer->visible.right, layer->rect.right);
        layer->visible.top = std::max(layer->visible.top, layer->rect.top);

        if (layer->flags & 0x1) {
            SRgnCombineRectf(&rgn.m_handle, &layer->rect, 0, 4);
        }

        layer = layer->zorderlink.Next();
    }

    SRgnDelete(&rgn.m_handle);

    // Save viewport
    float minX, maxX, minY, maxY, minZ, maxZ;
    GxXformViewport(minX, maxX, minY, maxY, minZ, maxZ);

    layer = Screen::s_zorderlist.Head();

    while (layer) {
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

        layer = layer->zorderlink.Next();
    }

    // Restore viewport
    GxXformSetViewport(minX, maxX, minY, maxY, minZ, maxZ);

    GxuFontUpdate();

    if (!Screen::s_presentDisable) {
        if (Screen::s_captureScreen) {
            // TODO

            GxScenePresent();

            // TODO

            return 1;
        }

        GxScenePresent();
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

    char fontFile[260];

    OsBuildFontFilePath("FRIZQT__.TTF", fontFile, 260);

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

void ScrnLayerCreate(const RECTF* rect, float zorder, unsigned long flags, void* param, void (*paintFunc)(void*, const RECTF*, const RECTF*, float), HLAYER* layer) {
    static RECTF defaultrect = { 0.0f, 0.0f, 1.0f, 1.0f };

    const RECTF* r = rect ? rect : &defaultrect;

    void* m = SMemAlloc(sizeof(CILayer), __FILE__, __LINE__, 0);

    CILayer* l = new (m) CILayer();

    l->rect.left = r->left;
    l->rect.bottom = r->bottom;
    l->rect.right = r->right;
    l->rect.top = r->top;

    l->zorder = zorder;
    l->flags = flags;
    l->param = param;
    l->paintfunc = paintFunc;

    auto node = Screen::s_zorderlist.Head();

    while (node && zorder < node->zorder) {
        node = node->zorderlink.Next();
    }

    Screen::s_zorderlist.LinkNode(l, 1, node);

    *layer = HandleCreate(l);
}

void ScrnSetStockFont(SCRNSTOCK stockID, const char* fontTexturePath) {
    if (Screen::s_stockObjects[stockID]) {
        HandleClose(Screen::s_stockObjects[stockID]);
    }

    float fontHeight = NDCToDDCHeight(Screen::s_stockObjectHeights[stockID]);
    HTEXTFONT font = TextBlockGenerateFont(fontTexturePath, 0, fontHeight);
    Screen::s_stockObjects[stockID] = font;
}

void SRgnCombineRectf(HSRGN* handle, RECTF* rect, void* param, int32_t combinemode) {
    // TODO
}

void SRgnCreate(HSRGN* handle, uint32_t reserved) {
    // TODO
}

void SRgnDelete(HSRGN* handle) {
    // TODO
}

void SRgnGetBoundingRectf(HSRGN* handle, RECTF* rect) {
    // TODO
}
