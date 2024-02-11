#ifndef CONSOLE_SCREEN_HPP
#define CONSOLE_SCREEN_HPP

#define HIGHLIGHT_COPY_SIZE 128

#include "console/Line.hpp"
#include <storm/region/Types.hpp>

enum HIGHLIGHTSTATE {
    HS_NONE = 0,
    HS_HIGHLIGHTING = 1,
    HS_ENDHIGHLIGHT = 2,
    NUM_HIGHLIGHTSTATES
};

void ConsoleScreenAnimate(float elapsedSec);

void ConsoleScreenInitialize(const char* title);

void SetInputString(char* buffer);

void ResetHighlight();

void UpdateHighlight();

HIGHLIGHTSTATE GetHighlightState();

void SetHighlightState(HIGHLIGHTSTATE hs);

void SetHighlightCopyText(char* text);

char* GetHighlightCopyText();

void ResetHighlightCopyText();

void SetHighlightStart(float start);
void SetHighlightEnd(float end);

RECTF& GetHighlightRect();

void CutHighlightToClipboard();
void PasteClipboardToHighlight();

void GenerateNodeString(CONSOLELINE* node);

#endif
