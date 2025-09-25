#include "console/Command.hpp"
#include "console/CommandHandlers.hpp"
#include <storm/Error.hpp>

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
char g_commandHistory[HISTORY_DEPTH][CMD_BUFFER_SIZE];
uint32_t g_commandHistoryIndex;

void ConsoleCommandDestroy() {
    g_consoleCommandHash.Clear();
}

char* ConsoleCommandHistory(uint32_t index) {
    // Return a pointer to the buffer at the specified index
    return g_commandHistory[((g_commandHistoryIndex + (HISTORY_DEPTH - 1) - index) & (HISTORY_DEPTH - 1))];
}

uint32_t ConsoleCommandHistoryDepth() {
    return HISTORY_DEPTH;
}

void ConsoleCommandInitialize() {
    ConsoleCommandRegister("help", ConsoleCommand_Help, CONSOLE, "Provides help information about a command.");
}

int32_t ConsoleCommandRegister(const char* command, int32_t (*handler)(const char*, const char*), CATEGORY category, const char* helpText) {
    STORM_ASSERT(command);
    STORM_ASSERT(handler);

    if (SStrLen(command) > (MAX_CMD_LENGTH - 1) || g_consoleCommandHash.Ptr(command)) {
        // The command name exceeds MAX_CMD_LENGTH, minus the null terminator
        // or it has already been registered
        return 0;
    }

    // Register the new command
    auto commandPtr = g_consoleCommandHash.New(command, 0, 0);
    commandPtr->command = command;
    commandPtr->handler = handler;
    commandPtr->helpText = helpText;
    commandPtr->category = category;

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
