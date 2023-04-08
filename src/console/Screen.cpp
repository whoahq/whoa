#include "console/Screen.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/Screen.hpp"
#include <storm/String.hpp>
#include <tempest/Rect.hpp>

static CGxStringBatch* s_batch;
static float s_caretpixwidth;
static float s_caretpixheight;
static float s_fontHeight = 0.02f;
static char s_fontName[STORM_MAX_PATH];
static HLAYER s_layerBackground;
static HLAYER s_layerText;
static RECTF s_rect = { 0.0f, 1.0f, 1.0f, 1.0f };
static HTEXTFONT s_textFont;

void PaintBackground(void* param, const RECTF* rect, const RECTF* visible, float elapsedSec) {
    // TODO
}

void PaintText(void* param, const RECTF* rect, const RECTF* visible, float elapsedSec) {
    // TODO
}

void RegisterHandlers() {
    // TODO
}

void ConsoleScreenInitialize(const char* title) {
    CRect windowSize;
    GxCapsWindowSize(windowSize);

    auto width = windowSize.maxX - windowSize.minX;
    auto height = windowSize.maxY - windowSize.minY;
    s_caretpixwidth = width == 0.0f ? 1.0f : 1.0f / width;
    s_caretpixheight = height == 0.0f ? 1.0f : 1.0f / height;

    SStrCopy(s_fontName, "Fonts\\ARIALN.ttf", sizeof(s_fontName));
    s_textFont = TextBlockGenerateFont(s_fontName, 0, NDCToDDCHeight(s_fontHeight));

    ScrnLayerCreate(&s_rect, 6.0f, 0x1 | 0x2, nullptr, PaintBackground, &s_layerBackground);
    ScrnLayerCreate(&s_rect, 7.0f, 0x1 | 0x2, nullptr, PaintText, &s_layerText);

    RegisterHandlers();

    // TODO register commands

    // TODO EventSetConfirmCloseCallback(EventCloseCallback, 0);

    // TODO ConsoleCommandExecute("ver", 1);

    s_batch = GxuFontCreateBatch(false, false);
}
