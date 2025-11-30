#ifndef UI_TYPES_HPP
#define UI_TYPES_HPP

#include <cstdint>

struct lua_State;
class CSimpleFrame;

enum DRAWLAYER {
    DRAWLAYER_BACKGROUND        = 0,
    DRAWLAYER_BACKGROUND_BORDER = 1,
    DRAWLAYER_ARTWORK           = 2,
    DRAWLAYER_ARTWORK_OVERLAY   = 3,
    DRAWLAYER_HIGHLIGHT         = 4,
    NUM_SIMPLEFRAME_DRAWLAYERS  = 5
};

enum CSimpleEventType {
    SIMPLE_EVENT_CHAR           = 0,
    SIMPLE_EVENT_KEY            = 1,
    SIMPLE_EVENT_MOUSE          = 2,
    SIMPLE_EVENT_MOUSEWHEEL     = 3,
    SIMPLE_EVENT_4              = 4,
    NUM_SIMPLE_EVENTS           = 5
};

enum FONT_UPDATE_FLAGS {
    FLAG_FONT_UPDATE            = 0x1,
    FLAG_STYLE_UPDATE           = 0x2,
    FLAG_COLOR_UPDATE           = 0x4,
    FLAG_SHADOW_UPDATE          = 0x8,
    FLAG_SPACING_UPDATE         = 0x10,

    FLAG_COMPLETE_UPDATE        = FLAG_FONT_UPDATE
                                | FLAG_STYLE_UPDATE
                                | FLAG_COLOR_UPDATE
                                | FLAG_SHADOW_UPDATE
                                | FLAG_SPACING_UPDATE,
};

enum FRAMEPOINT {
    FRAMEPOINT_TOPLEFT          = 0,
    FRAMEPOINT_TOP              = 1,
    FRAMEPOINT_TOPRIGHT         = 2,
    FRAMEPOINT_LEFT             = 3,
    FRAMEPOINT_CENTER           = 4,
    FRAMEPOINT_RIGHT            = 5,
    FRAMEPOINT_BOTTOMLEFT       = 6,
    FRAMEPOINT_BOTTOM           = 7,
    FRAMEPOINT_BOTTOMRIGHT      = 8,
    FRAMEPOINT_NUMPOINTS        = 9
};

enum FRAME_STRATA {
    FRAME_STRATA_WORLD              = 0,
    FRAME_STRATA_BACKGROUND         = 1,
    FRAME_STRATA_LOW                = 2,
    FRAME_STRATA_MEDIUM             = 3,
    FRAME_STRATA_HIGH               = 4,
    FRAME_STRATA_DIALOG             = 5,
    FRAME_STRATA_FULLSCREEN         = 6,
    FRAME_STRATA_FULLSCREEN_DIALOG  = 7,
    FRAME_STRATA_TOOLTIP            = 8,
    NUM_FRAME_STRATA                = 9
};

enum FRAMESCRIPT_GENDER {
    GENDER_NOT_APPLICABLE   = 0,
    GENDER_NONE             = 1,
    GENDER_MALE             = 2,
    GENDER_FEMALE           = 3,
    GENDER_MALE_PLURAL      = 4,
    GENDER_FEMALE_PLURAL    = 5,
    GENDER_MIXED_PLURAL     = 6
};

enum HTML_TEXT_TYPE {
    HTML_TEXT_NORMAL        = 0,
    HTML_TEXT_HEADER1       = 1,
    HTML_TEXT_HEADER2       = 2,
    HTML_TEXT_HEADER3       = 3,
    NUM_HTML_TEXT_TYPES     = 4,
};

enum PLURAL_RULE {
    PLURAL_RULE_0   = 0,
    PLURAL_RULE_1   = 1,
    PLURAL_RULE_2   = 2
};

enum SLIDER_ORIENTATION {
    SLIDER_HORIZONTAL       = 0,
    SLIDER_VERTICAL         = 1,
};

enum TextureImageMode {
    ImageMode_UI            = 0,
    ImageMode_Desaturate    = 1
};

struct FRAMEPRIORITY {
    CSimpleFrame* frame;
    uint32_t priority;
};

struct FrameScript_Method {
    const char* name;
    int32_t (*method)(lua_State*);
};

#endif
