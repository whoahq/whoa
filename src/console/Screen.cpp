#include "console/Screen.hpp"
#include "console/Console.hpp"
#include "console/Command.hpp"
#include "console/Handlers.hpp"
#include "console/Line.hpp"
#include "console/Types.hpp"
#include "gx/Buffer.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
#include "gx/Draw.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/RenderState.hpp"
#include "gx/Screen.hpp"
#include <bc/Debug.hpp>
#include <storm/String.hpp>
#include <tempest/Rect.hpp>
#include <algorithm>

static CGxStringBatch* s_batch;
static uint32_t s_baseTextFlags = 0x8;
static int32_t s_caret = 0;
static float s_caretpixwidth;
static float s_caretpixheight;
static float s_charSpacing = 0.0f;
static CGxString* s_inputString = nullptr;

static char s_fontName[STORM_MAX_PATH];
static HLAYER s_layerBackground;
static HLAYER s_layerText;
static RECTF s_rect = { 0.0f, 1.0f, 1.0f, 1.0f };
static HTEXTFONT s_textFont;

static HIGHLIGHTSTATE s_highlightState = HS_NONE;
static RECTF s_hRect = { 0.0f, 0.0f, 0.0f, 0.0f };
static float s_highlightHStart = 0.0f;
static float s_highlightHEnd = 0.0f;
static uint32_t s_highlightLeftCharIndex = 0;
static uint32_t s_highlightRightCharIndex = 0;
static int32_t s_highlightInput = 0;
static char s_copyText[HIGHLIGHT_COPY_SIZE] = { 0 };

static CImVector s_colorArray[] = {
    { 0xFF, 0xFF, 0xFF, 0xFF }, // DEFAULT_COLOR
    { 0xFF, 0xFF, 0xFF, 0xFF }, // INPUT_COLOR
    { 0x80, 0x80, 0x80, 0xFF }, // ECHO_COLOR
    { 0x00, 0x00, 0xFF, 0xFF }, // ERROR_COLOR
    { 0x00, 0xFF, 0xFF, 0xFF }, // WARNING_COLOR
    { 0xFF, 0xFF, 0xFF, 0xFF }, // GLOBAL_COLOR
    { 0xFF, 0xFF, 0xFF, 0xFF }, // ADMIN_COLOR
    { 0xFF, 0xFF, 0xFF, 0x80 }, // HIGHLIGHT_COLOR
    { 0x00, 0x00, 0x00, 0xC0 }, // BACKGROUND_COLOR
};

void DrawBackground() {
    uint16_t indices[] = {
        0, 1, 2, 3
    };

    C3Vector position[] = {
        { s_rect.left,  s_rect.bottom,  0.0f },
        { s_rect.right, s_rect.bottom,  0.0f },
        { s_rect.left,  s_rect.top,     0.0f },
        { s_rect.right, s_rect.top,     0.0f }
    };

    GxRsPush();

    GxRsSet(GxRs_Lighting, 0);
    GxRsSet(GxRs_Fog, 0);
    GxRsSet(GxRs_DepthTest, 0);
    GxRsSet(GxRs_DepthWrite, 0);
    GxRsSet(GxRs_Culling, 0);
    GxRsSet(GxRs_PolygonOffset, 0.0f);
    GxRsSet(GxRs_BlendingMode, GxBlend_Alpha);
    GxRsSet(GxRs_AlphaRef, CGxDevice::s_alphaRef[GxBlend_Alpha]);

    GxPrimLockVertexPtrs(4, position, sizeof(C3Vector), nullptr, 0, &s_colorArray[BACKGROUND_COLOR], 0, nullptr, 0, nullptr, 0, nullptr, 0);
    GxDrawLockedElements(GxPrim_TriangleStrip, 4, indices);
    GxPrimUnlockVertexPtrs();

    GxRsPop();
}

void DrawHighLight() {
    uint16_t indices[] = {
        0, 1, 2, 3
    };

    C3Vector position[] = {
        { s_hRect.left,  s_hRect.bottom,  0.0f },
        { s_hRect.right, s_hRect.bottom,  0.0f },
        { s_hRect.left,  s_hRect.top,     0.0f },
        { s_hRect.right, s_hRect.top,     0.0f }
    };

    GxRsPush();

    GxRsSet(GxRs_Lighting, 0);
    GxRsSet(GxRs_BlendingMode, GxBlend_Alpha);
    GxRsSet(GxRs_AlphaRef, CGxDevice::s_alphaRef[GxBlend_Alpha]);

    GxPrimLockVertexPtrs(4, position, sizeof(C3Vector), nullptr, 0, &s_colorArray[HIGHLIGHT_COLOR], 0, nullptr, 0, nullptr, 0, nullptr, 0);
    GxDrawLockedElements(GxPrim_TriangleStrip, 4, indices);
    GxPrimUnlockVertexPtrs();

    GxRsPop();
}

void DrawCaret(C3Vector& caretpos) {
    uint16_t indices[] = {
        0, 1, 2, 3
    };

    float minX = caretpos.x;
    float minY = caretpos.y;

    float maxX = caretpos.x + (s_caretpixwidth * 2);
    float maxY = caretpos.y + ConsoleGetFontHeight();

    C3Vector position[] = {
        { minX, minY, 0.0f },
        { maxX, minY, 0.0f },
        { minX, maxY, 0.0f },
        { maxX, maxY, 0.0f }
    };

    GxRsPush();

    GxRsSet(GxRs_Lighting, 0);
    GxRsSet(GxRs_Fog, 0);
    GxRsSet(GxRs_DepthTest, 0);
    GxRsSet(GxRs_DepthWrite, 0);
    GxRsSet(GxRs_Culling, 0);
    GxRsSet(GxRs_PolygonOffset, 0.0f);
    GxRsSet(GxRs_BlendingMode, GxBlend_Alpha);
    GxRsSet(GxRs_AlphaRef, CGxDevice::s_alphaRef[GxBlend_Alpha]);

    GxPrimLockVertexPtrs(4, position, sizeof(C3Vector), nullptr, 0, &s_colorArray[INPUT_COLOR], 0, nullptr, 0, nullptr, 0, nullptr, 0);
    GxDrawLockedElements(GxPrim_TriangleStrip, 4, indices);
    GxPrimUnlockVertexPtrs();

    GxRsPop();
}

void PaintBackground(void* param, const RECTF* rect, const RECTF* visible, float elapsedSec) {
    if (s_rect.bottom < 1.0f) {
        DrawBackground();

        if (s_highlightState) {
            DrawHighLight();
        }
    }
}

void SetInputString(char* buffer) {
    // s_highlightState = HS_NONE;
    // s_hRect = { 0.0f, 0.0f, 0.0f, 0.0f };
    // s_highlightLeftCharIndex = 0;
    // s_highlightRightCharIndex = 0;
    // s_highlightInput = 0;

    if (s_inputString) {
        GxuFontDestroyString(s_inputString);
    }

    s_inputString = nullptr;

    auto fontHeight = ConsoleGetFontHeight();

    if (buffer && buffer[0] != '\0') {
        C3Vector pos = { 0.0f, 0.0f, 1.0f };

        auto font = TextBlockGetFontPtr(s_textFont);

        GxuFontCreateString(font, buffer, fontHeight, pos, 1.0f, fontHeight, 0.0f, s_inputString, GxVJ_Middle, GxHJ_Left, s_baseTextFlags, s_colorArray[INPUT_COLOR], s_charSpacing, 1.0f);
    }
}

void PasteInInputLine(char* characters) {
    auto len = SStrLen(characters);

    if (!len) {
        return;
    }

    auto line = GetInputLine();

    ReserveInputSpace(line, len);

    if (line->inputpos < line->chars) {
        if (len <= 1) {
            memmove(&line->buffer[line->inputpos + 1], &line->buffer[line->inputpos], line->chars - (line->inputpos + 1));

            line->buffer[line->inputpos] = *characters;

            line->inputpos++;
            line->chars++;
        } else {
            auto input = reinterpret_cast<char*>(SMemAlloc(line->charsalloc, __FILE__, __LINE__, 0x0));
            SStrCopy(input, &line->buffer[line->inputpos], STORM_MAX_STR);

            auto buffer = reinterpret_cast<char*>(SMemAlloc(line->charsalloc, __FILE__, __LINE__, 0x0));
            SStrCopy(buffer, line->buffer, STORM_MAX_STR);
            buffer[line->inputpos] = '\0';

            SStrPack(buffer, characters, line->charsalloc);

            auto len = SStrLen(buffer);

            line->inputpos = len;

            SStrPack(buffer, input, line->charsalloc);
            SStrCopy(line->buffer, buffer, STORM_MAX_STR);

            line->chars = SStrLen(line->buffer);

            if (input) {
                SMemFree(input, __FILE__, __LINE__, 0);
            }

            if (buffer) {
                SMemFree(input, __FILE__, __LINE__, 0);
            }
        }
    } else {
        for (int32_t i = 0; i < len; i++) {
            line->buffer[line->inputpos++] = characters[i];
        }

        line->buffer[line->inputpos] = '\0';
        line->chars = line->inputpos;
    }

    SetInputString(line->buffer);
}

void GenerateNodeString(CONSOLELINE* node) {
    auto font = TextBlockGetFontPtr(s_textFont);

    if (font && node && node->buffer && node->buffer[0] != '\0') {
        if (node->fontPointer) {
            GxuFontDestroyString(node->fontPointer);
        }

        C3Vector pos = {
            0.0f, 0.0f, 1.0f
        };

        auto fontHeight = ConsoleGetFontHeight();

        GxuFontCreateString(font, node->buffer, fontHeight, pos, 1.0f, fontHeight, 0.0f, node->fontPointer, GxVJ_Middle, GxHJ_Left, s_baseTextFlags, s_colorArray[node->colorType], s_charSpacing, 1.0f);
        BLIZZARD_ASSERT(node->fontPointer);
    }
}

void PaintText(void* param, const RECTF* rect, const RECTF* visible, float elapsedSec) {
    if (s_rect.bottom >= 1.0f) {
        return;
    }

    static float carettime = 0.0f;
    static C3Vector caretpos = { 0.0f, 0.0f, 0.0f };

    //
    carettime += elapsedSec;
    if ((!s_caret && carettime > 0.2) || (carettime > 0.3)) {
        s_caret = !s_caret;
        carettime = 0;
    }

    auto line = GetInputLine();

    C3Vector pos = {
        s_rect.left,
        (ConsoleGetFontHeight() * 0.75f) + s_rect.bottom,
        1.0f
    };

    GxuFontClearBatch(s_batch);

    if (s_inputString) {
        GxuFontSetStringPosition(s_inputString, pos);
        GxuFontAddToBatch(s_batch, s_inputString);
    }

    auto font = TextBlockGetFontPtr(s_textFont);

    if (line->inputpos) {
        caretpos = pos;

        GxuFontGetTextExtent(font, line->buffer, line->inputpos, ConsoleGetFontHeight(), &caretpos.x, 0.0f, 1.0f, s_charSpacing, s_baseTextFlags);

        DrawCaret(caretpos);
    }

    pos.y += ConsoleGetFontHeight();

    for (auto lineptr = GetCurrentLine(); (lineptr && pos.y < 1.0); lineptr = lineptr->Next()) {
        if (lineptr != line) {
            if (lineptr->fontPointer == nullptr) {
                GenerateNodeString(lineptr);
            }

            GxuFontSetStringPosition(lineptr->fontPointer, pos);
            GxuFontAddToBatch(s_batch, lineptr->fontPointer);
            pos.y += ConsoleGetFontHeight();
        }
    }

    GxuFontRenderBatch(s_batch);
}

void UpdateHighlight() {
    auto font = TextBlockGetFontPtr(s_textFont);
    BLIZZARD_ASSERT(font);

    auto len = SStrLen(s_copyText);

    float left = std::min(s_highlightHStart, s_highlightHEnd);
    float right = std::max(s_highlightHStart, s_highlightHEnd);

    auto chars = GxuFontGetMaxCharsWithinWidth(font, s_copyText, ConsoleGetFontHeight(), left, len, &s_hRect.left, 0.0f, 1.0f, s_charSpacing, s_baseTextFlags);

    s_highlightLeftCharIndex = chars;

    if (chars) {
        s_highlightRightCharIndex = chars - 1;
    }

    if (s_hRect.left < 0.015f) {
        s_hRect.left = 0.0f;
    }

    s_highlightRightCharIndex = GxuFontGetMaxCharsWithinWidth(font, s_copyText, ConsoleGetFontHeight(), right, len, &s_hRect.right, 0.0f, 1.0f, s_charSpacing, s_baseTextFlags);
}

void ResetHighlight() {
    s_highlightState = HS_NONE;
    s_hRect = { 0.0f, 0.0f, 0.0f, 0.0f };
}

HIGHLIGHTSTATE GetHighlightState() {
    return s_highlightState;
}

void SetHighlightState(HIGHLIGHTSTATE hs) {
    s_highlightState = hs;
}

char* GetHighlightCopyText() {
    return s_copyText;
}

void SetHighlightCopyText(char* text) {
    SStrCopy(s_copyText, text, HIGHLIGHT_COPY_SIZE);
}

void ResetHighlightCopyText() {
    s_copyText[0] = '\0';
}

RECTF& GetHighlightRect() {
    return s_hRect;
}

void SetHighlightStart(float start) {
    s_highlightHStart = start;
}

void SetHighlightEnd(float end) {
    s_highlightHEnd = end;
}

void CutHighlightToClipboard() {
    char buffer[HIGHLIGHT_COPY_SIZE];

    if (s_copyText[0] != '\0') {
        uint32_t size = s_highlightRightCharIndex - s_highlightLeftCharIndex;
        uint32_t capsize = HIGHLIGHT_COPY_SIZE-1;
        size = std::min(size, capsize);

        SStrCopy(buffer, &s_copyText[s_highlightLeftCharIndex], size);

        buffer[size] = '\0';

        // OsClipboardPutString(buffer);
    }

    ResetHighlight();
}

void PasteClipboardToHighlight() {
    // auto buffer = OsClipboardGetString();
    // PasteInInputLine(buffer);
    // SMemFree(buffer, __FILE__, __LINE__, 0);
    // ResetHighlight();
}

void ConsoleScreenAnimate(float elapsedSec) {
    auto finalPos = ConsoleGetActive() ? std::min(1.0f - ConsoleGetHeight(), 1.0f) : 1.0f;
    finalPos = std::max(finalPos, 0.0f);

    if (s_rect.bottom == finalPos) {
        return;
    }

    auto currentPos = finalPos;

    if (ConsoleGetResizeState() == CS_NONE) {
        auto direction = s_rect.bottom <= finalPos ? 1.0f : -1.0f;

        currentPos = s_rect.bottom + direction * elapsedSec * 5.0f;
        currentPos = ConsoleGetActive() ? std::max(currentPos, finalPos) : std::min(currentPos, finalPos);
    }

    s_rect.bottom = currentPos;

    ScrnLayerSetRect(s_layerBackground, &s_rect);
    ScrnLayerSetRect(s_layerText, &s_rect);
}

void ConsoleScreenInitialize(const char* title) {
    CRect windowSize;
    GxCapsWindowSize(windowSize);

    auto width = windowSize.maxX - windowSize.minX;
    auto height = windowSize.maxY - windowSize.minY;
    s_caretpixwidth = width == 0.0f ? 1.0f : 1.0f / width;
    s_caretpixheight = height == 0.0f ? 1.0f : 1.0f / height;

    SStrCopy(s_fontName, "Fonts\\ARIALN.ttf", sizeof(s_fontName));
    s_textFont = TextBlockGenerateFont(s_fontName, 0, NDCToDDCHeight(ConsoleGetFontHeight()));

    ScrnLayerCreate(&s_rect, 6.0f, 0x1 | 0x2, nullptr, PaintBackground, &s_layerBackground);
    ScrnLayerCreate(&s_rect, 7.0f, 0x1 | 0x2, nullptr, PaintText, &s_layerText);

    RegisterHandlers();

    // TODO register commands
    ConsoleInitializeScreenCommand();

    // TODO EventSetConfirmCloseCallback(EventCloseCallback, 0);

    ConsoleCommandExecute("ver", 1);

    s_batch = GxuFontCreateBatch(false, false);
}
