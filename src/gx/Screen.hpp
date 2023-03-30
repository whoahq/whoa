#ifndef GX_SCREEN_HPP
#define GX_SCREEN_HPP

#include <cstdint>
#include <common/Handle.hpp>
#include <storm/List.hpp>
#include <storm/Region.hpp>

typedef HOBJECT HLAYER;

enum SCRNSTOCK {
    STOCK_SYSFONT = 0,
    STOCK_PERFFONT = 1,
    SCRNSTOCKOBJECTS = 2
};

class CILayer : public CHandleObject {
    public:
        RECTF rect;
        RECTF visible;
        float zorder;
        uint32_t flags;
        void* param;
        void (*paintfunc)(void*, const RECTF*, const RECTF*, float);
        TSLink<CILayer> zorderlink;
};

class CSRgn {
    public:
        HSRGN m_handle;
};

namespace Screen {
    extern int32_t s_captureScreen;
    extern float s_elapsedSec;
    extern int32_t s_presentDisable;
    extern HOBJECT s_stockObjects[SCRNSTOCKOBJECTS];
    extern float s_stockObjectHeights[SCRNSTOCKOBJECTS];
    extern STORM_EXPLICIT_LIST(CILayer, zorderlink) s_zorderlist;
}

void ILayerInitialize(void);

void IStockInitialize(void);

void ScrnInitialize(int32_t);

void ScrnLayerCreate(const RECTF*, float, unsigned long, void*, void (*)(void*, const RECTF*, const RECTF*, float), HLAYER*);

void ScrnSetStockFont(SCRNSTOCK, const char*);

#endif
