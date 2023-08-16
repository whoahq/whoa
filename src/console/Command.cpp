#include "console/Command.hpp"
#include "console/Line.hpp"

#include <storm/Error.hpp>
#include <storm/Unicode.hpp>

#include <cctype>
#include <algorithm>

int32_t ValidateFileName(const char* filename) {
    if (SStrStr(filename, "..") || SStrStr(filename, "\\")) {
        // TODO
        // ConsoleWrite("File Name cannot contain '\\' or '..'", ERROR_COLOR);
        return 0;
    }

    const char* extension = SStrChrR(filename, '.');

    if (extension && SStrCmpI(extension, ".wtf", -1)) {
        // TODO
        // ConsoleWrite("Only .wtf extensions are allowed", ERROR_COLOR);
        return 0;
    }

    return 1;
}

TSHashTable<CONSOLECOMMAND, HASHKEY_STRI> g_consoleCommandHash;
char g_commandHistory[CONSOLE_HISTORY_DEPTH][CONSOLE_CMD_BUFFER_SIZE];
uint32_t g_commandHistoryIndex;
char g_ExecBuffer[CONSOLE_EXEC_BUFFER_SIZE] = {0};
EXECMODE g_ExecCreateMode = EM_NOTACTIVE;

int32_t AddLineToExecFile(const char* currentLine) {
    char stringToWrite[STORM_MAX_PATH];

    if (g_ExecCreateMode != EM_PROMPTOVERWRITE) {
        if (!SStrCmpI(currentLine, "end", STORM_MAX_STR)) {
            if (g_ExecCreateMode != EM_APPEND) {
                g_ExecCreateMode = EM_WRITEFILE;
            }

            return 1;
        }

        SStrPrintf(stringToWrite, sizeof(stringToWrite), "%s\n", currentLine);

        if (((sizeof(g_ExecBuffer)-1) - SStrLen(g_ExecBuffer)) != SStrLen(stringToWrite)){
            SStrPack(g_ExecBuffer, stringToWrite, sizeof(g_ExecBuffer));
        }

        return 0;
    }

    if (currentLine[0] == 'n') {
        ConsoleWrite("Canceled File Creation", ECHO_COLOR);
        g_ExecCreateMode = EM_NOTACTIVE;
        return 0;
    }

    if (currentLine[0] != 'y') {
        ConsoleWrite("You must type 'y' to confirm overwrite. Process aborted!", ERROR_COLOR);
        g_ExecCreateMode = EM_NOTACTIVE;
        return 0;
    }

    // TODO

    return 1;
}


void ConsoleCommandDestroy() {
    g_consoleCommandHash.Clear();
}

char* ConsoleCommandHistory(uint32_t index) {
    // Return a pointer to the buffer at the specified index
    return g_commandHistory[((g_commandHistoryIndex + (CONSOLE_HISTORY_DEPTH - 1) - index) & (CONSOLE_HISTORY_DEPTH - 1))];
}

void AddToHistory(const char* command) {
    SStrCopy(g_commandHistory[g_commandHistoryIndex], command, CONSOLE_LINE_LENGTH);
    g_commandHistoryIndex = (g_commandHistoryIndex + 1) & (CONSOLE_HISTORY_DEPTH-1);
}

uint32_t ConsoleCommandHistoryDepth() {
    return HISTORY_DEPTH;
}

void ConsoleCommandInitialize() {
    ConsoleCommandRegister("help", ConsoleCommand_Help, CONSOLE, "Provides help information about a command.");
}

int32_t ConsoleCommandRegister(const char* command, COMMANDHANDLER handler, CATEGORY category, const char* helpText) {
    STORM_ASSERT(command);
    STORM_ASSERT(handler);

    if (SStrLen(command) > (MAX_CMD_LENGTH - 1) || g_consoleCommandHash.Ptr(command)) {
        // The command name exceeds MAX_CMD_LENGTH, minus the null terminator
        // or it has already been registered
        return 0;
    }

    // Register the new command
    auto commandPtr = g_consoleCommandHash.New(command, 0, 0);
    commandPtr->m_handler = handler;
    commandPtr->m_helpText = helpText;
    commandPtr->m_category = category;

    return 1;
}

void ConsoleCommandUnregister(const char* command) {
    if (command) {
        auto commandPtr = g_consoleCommandHash.Ptr(command);
        if (commandPtr) {
            g_consoleCommandHash.Delete(commandPtr);
        }
    }
}

CONSOLECOMMAND* ParseCommand(const char* commandLine, const char** command, char* arguments, size_t argsize) {
    STORM_ASSERT(commandLine);

    auto string = commandLine;

    static char cmd[32] = { 0 };

    auto cmdptr = &cmd[0];

    int32_t end = CONSOLE_MAX_CMD_LENGTH;
    int32_t i = 0;

    while (i < end) {
        int32_t chars;

        auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);

        if (code == -1 || code == ' ' || chars > CONSOLE_MAX_CMD_LENGTH) {
            break;
        }

        if (chars) {
            for (size_t c = 0; c < chars; c++) {
                *cmdptr = *string;
                cmdptr += chars;
                string += chars;
            }
        }

        i += chars;
    }

    *cmdptr = '\0';

    if (command) {
        *command = cmd;
    }

    auto argptr = arguments;

    if (arguments) {
        int32_t chars;

        auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);

        // Discard space
        while(code != -1 && code == ' ') {
            string += chars;
            code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);
        }

        SStrCopy(argptr, string, argsize);

        auto len = SStrLen(argptr);

        while (len > 0 && (argptr[len-1] == ' ')) {
            len--;
            argptr[len] = '\0';
        }

    }

    return g_consoleCommandHash.Ptr(cmd);
}

void MakeCommandCurrent(CONSOLELINE* lineptr, char* command) {
    auto len = lineptr->inputstart;
    lineptr->inputpos = len;
    lineptr->chars = len;
    lineptr->buffer[len] = '\0';

    len = SStrLen(command);
    ReserveInputSpace(lineptr, len);

    SStrCopy(lineptr->buffer + lineptr->inputpos, command, STORM_MAX_STR);

    len = lineptr->inputpos + len;
    lineptr->inputpos = len;
    lineptr->chars = len;
}

void ConsoleCommandExecute(char* commandLine, int32_t addToHistory) {
    auto em = g_ExecCreateMode;

    if (em == EM_RECORDING || em == EM_PROMPTOVERWRITE || em == EM_APPEND) {
        AddLineToExecFile(commandLine);
        return;
    }

    auto history = ConsoleCommandHistory(0);

    if (addToHistory && (history == nullptr || SStrCmp(commandLine, history, STORM_MAX_STR))) {
        AddToHistory(commandLine);
    }

    const char* command = nullptr;
    auto arguments = reinterpret_cast<char*>(SMemAlloc(CMD_BUFFER_SIZE, __FILE__, __LINE__, 0));

    auto cmd = ParseCommand(commandLine, &command, arguments, CMD_BUFFER_SIZE);

    if (cmd) {
        cmd->m_handler(command, arguments);
    } else {
        ConsoleWrite("Unknown command", DEFAULT_COLOR);
    }

    if (arguments) {
        SMemFree(arguments, __FILE__, __LINE__, 0);
    }
}

static ConsoleCommandList s_consoleSpecificCommands[] = {
    { "fontcolor",        ConsoleCommand_FontColor,       "[ColorClassName] [Red 0-255] [Green 0-255] [Blue 0-255]"      },
    { "bgcolor",          ConsoleCommand_BackGroundColor, "[alpha 0-255] [Red 0-255] [Green 0-255] [Blue 0-255]"         },
    { "highlightcolor",   ConsoleCommand_HighLightColor,  "[alpha 0-255] [Red 0-255] [Green 0-255] [Blue 0-255]"         },
    { "fontsize",         ConsoleCommand_FontSize,        "[15-50] arbitrary font size"                                  },
    { "font",             ConsoleCommand_Font,            "[fontname] make sure to use the .ttf file name"               },
    { "consolelines",     ConsoleCommand_BufferSize,      "[number] number of lines to show in the console"              },
    { "clear",            ConsoleCommand_ClearConsole,    "Clears the console buffer"                                    },
    { "proportionaltext", ConsoleCommand_Proportional,    "Toggles fixed-width text characters"                          },
    { "spacing",          ConsoleCommand_CharSpacing,     "[float] specifies inter-character spacing, in pixels"         },
    { "settings",         ConsoleCommand_CurrentSettings, "Shows current font and console settings"                      },
    { "default",          ConsoleCommand_DefaultSettings, "Resets all the font and console settings"                     },
    { "closeconsole",     ConsoleCommand_CloseConsole,    "Closes the Console window"                                    },
    { "repeat",           ConsoleCommand_RepeatHandler,   "Repeats a command"                                            },
    { "AppendLogToFile",  ConsoleCommand_AppendLogToFile, "[filename = ConsoleLogs/Log<Timestamp>.txt] [numLines = all]" }
};

static ConsoleCommandList s_commonCommands[] = {
    { "quit",   ConsoleCommand_Quit,   nullptr },
    { "ver",    ConsoleCommand_Ver,    nullptr },
    { "setmap", ConsoleCommand_SetMap, nullptr }
};

void RegisterConsoleCommandList(CATEGORY category, ConsoleCommandList list[], size_t count) {
    size_t i = 0;

    while (i < count) {
        auto& cmd = list[i];
        ConsoleCommandRegister(cmd.m_command, cmd.m_handler, category, cmd.m_helpText);
        i++;
    }
}

void ConsoleInitializeScreenCommand() {
    CONSOLE_REGISTER_LIST(CONSOLE, s_consoleSpecificCommands);
}

void ConsoleCommandInitialize() {
    ConsoleCommandRegister("help", ConsoleCommand_Help, CONSOLE, "Provides help information about a command.");
}

void ConsoleInitializeCommonCommand() {
    CONSOLE_REGISTER_LIST(DEFAULT, s_commonCommands);
}

void ConsoleInitializeDebugCommand() {
    // TODO
}
