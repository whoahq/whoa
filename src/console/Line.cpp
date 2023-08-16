
#include "console/Line.hpp"
#include "console/Types.hpp"
#include "console/Console.hpp"
#include "console/Screen.hpp"
#include "gx/Device.hpp"

#include <storm/List.hpp>
#include <storm/thread/SCritSect.hpp>

static SCritSect s_critsect;
// In this list:
// The head = the input line.
// The tail = the oldest line printed.
static STORM_LIST(CONSOLELINE) s_linelist;
// Pointer to the current line. Determines what region of the console history gets rendered.
static CONSOLELINE* s_currlineptr = nullptr;
static uint32_t s_NumLines = 0;

void EnforceMaxLines() {
    if (s_NumLines <= CONSOLE_LINES_MAX) {
        return;
    }

    // Pop oldest line off the list
    auto lineptr = s_linelist.Tail();

    if (lineptr == nullptr) {
        lineptr = s_currlineptr;
    }

    if (lineptr == nullptr) {
        return;
    }

    // Clean up oldest line.
    s_linelist.UnlinkNode(lineptr);
    s_linelist.DeleteNode(lineptr);

    s_NumLines--;
}

CONSOLELINE* GetInputLine() {
    auto head = s_linelist.Head();

    // If the list is empty, or the list's head is an entered input-line,
    // Create a fresh input line, with "> " prefixed before the caret.
     if (!head || head->inputpos == 0) {
        auto l = SMemAlloc(sizeof(CONSOLELINE), __FILE__, __LINE__, 0);
        auto line = new(l) CONSOLELINE();
        line->buffer = reinterpret_cast<char*>(SMemAlloc(CONSOLE_LINE_PREALLOC, __FILE__, __LINE__, 0));
        line->charsalloc = CONSOLE_LINE_PREALLOC;

        s_linelist.LinkToHead(line);

        SStrCopy(line->buffer, "> ", line->charsalloc);
        SetInputString(line->buffer);
        auto chars = SStrLen(line->buffer);
        s_NumLines++;
        line->inputstart = chars;
        line->inputpos = chars;
        line->chars = chars;
        line->colorType = INPUT_COLOR;

        s_currlineptr = line;

        EnforceMaxLines();

        return line;
    }

    return head;
}

CONSOLELINE* GetLineAtMousePosition(float y) {
    // Loop through linelist to find line at mouse position
    int32_t linePos = static_cast<int32_t>((ConsoleGetHeight() - (1.0 - y)) / ConsoleGetFontHeight());

    if (linePos == 1) {
        return s_linelist.Head();
    }

    if (s_currlineptr != s_linelist.Head()) {
        linePos--;
    }

    CONSOLELINE* line = s_currlineptr;

    while (linePos > 1) {
        linePos--;

        if (!line) {
            line = s_linelist.Head();
        }

        if (line == nullptr) {
            return nullptr;
        }

        line = line->Next();
    }

    return line;
}

void ReserveInputSpace(CONSOLELINE* line, size_t len) {
    size_t newsize = line->chars + len;
    if (newsize >= line->charsalloc) {
        while (line->charsalloc <= newsize) {
            line->charsalloc += CONSOLE_LINE_PREALLOC;
        }

        auto buffer = reinterpret_cast<char*>(SMemAlloc(line->charsalloc, __FILE__, __LINE__, 0));
        SStrCopy(buffer, line->buffer, line->charsalloc);
        SMemFree(line->buffer, __FILE__, __LINE__, 0x0);
        line->buffer = buffer;
    }
}

void ConsoleWrite(const char* str, COLOR_T color) {
    if (g_theGxDevicePtr == nullptr || str[0] == '\0') {
        return;
    }

    s_critsect.Enter();

    auto l = reinterpret_cast<char*>(SMemAlloc(sizeof(CONSOLELINE), __FILE__, __LINE__, 0));
    auto lineptr = new(l) CONSOLELINE();

    auto head = s_linelist.Head();

    if (head == nullptr || head->inputpos == 0) {
        // Attach console line to head
        s_linelist.LinkToHead(lineptr);
    } else {
        // Attach console line between head and head-1
        s_linelist.LinkNode(lineptr, 1, head->Prev());
    }

    size_t len = SStrLen(str) + 1;
    lineptr->chars = len;
    lineptr->charsalloc = len;
    lineptr->buffer = reinterpret_cast<char*>(SMemAlloc(len, __FILE__, __LINE__, 0));
    lineptr->colorType = color;

    SStrCopy(lineptr->buffer, str, STORM_MAX_STR);

    GenerateNodeString(lineptr);

    s_NumLines++;

    EnforceMaxLines();
    //

    s_critsect.Leave();
}

void ConsoleWriteA(const char* str, COLOR_T color, ...) {
    char buffer[1024] = {0};

    if (str != nullptr && str[0] != '\0') {
        va_list list;
        va_start(list, color);
        vsnprintf(buffer, sizeof(buffer), str, list);
        va_end(list);

        ConsoleWrite(buffer, color);
    }
}

void MoveLinePtr(int32_t direction, int32_t modifier) {
    CONSOLELINE* lineptr = s_currlineptr;

    auto anyControl = (1 << KEY_LCONTROL) | (1 << KEY_RCONTROL);

    if (modifier & anyControl) {
        for (int32_t i = 0; i < 10 && lineptr != nullptr; i++) {
            CONSOLELINE* next;

            if (direction == 1) {
                next = lineptr->m_link.Next();
            } else {
                next = lineptr->m_link.Prev();
            }

            if (next != nullptr) {
                lineptr = next;
            }
        }
    } else {
        // if (s_currlineptr == s_linelist.Head()) {
        //     s_currlineptr = s_currlineptr->Prev();
        // }

        if (direction == 1) {
            lineptr = lineptr->m_link.Next();
        } else {
            lineptr = lineptr->m_link.Prev();
        }
    }

    if (lineptr) {
        s_currlineptr = lineptr;
    }
}

void BackspaceLine(CONSOLELINE* line) {
    if (line->inputstart <= line->inputpos && line->inputpos != line->inputstart) {
        if (line->inputpos < line->chars) {
            memmove(line->buffer + line->inputpos + -1, line->buffer + line->inputpos, (line->chars - line->inputpos) + 1);
        } else {
            line->buffer[line->inputpos - 1] = '\0';
        }
        line->chars--;
        line->inputpos--;

        SetInputString(line->buffer);
    }
}

CONSOLELINE* GetCurrentLine() {
    return s_currlineptr;
}

CONSOLELINE::~CONSOLELINE() {
    if (this->buffer) {
        SMemFree(this->buffer, __FILE__, __LINE__, 0);
    }

    if (this->fontPointer) {
        GxuFontDestroyString(this->fontPointer);
    }
}

void ConsoleClear() {
    s_NumLines = 0;

    auto ptr = s_linelist.Head();

    while (ptr) {
        s_linelist.UnlinkNode(ptr);
        s_linelist.DeleteNode(ptr);
        ptr = s_linelist.Head();
    }
}
