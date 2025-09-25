#ifndef CONSOLE_COMMAND_HPP
#define CONSOLE_COMMAND_HPP

#include <storm/Hash.hpp>
#include <cstdint>

#define EXEC_BUFFER_SIZE 8192
#define CMD_BUFFER_SIZE 1024
#define MAX_CMD_LENGTH 64
#define HISTORY_DEPTH 32
#define NOHELP nullptr

enum CATEGORY {
    DEBUG,
    GRAPHICS,
    CONSOLE,
    COMBAT,
    GAME,
    DEFAULT,
    NET,
    SOUND,
    GM,
    NONE,
    LAST
};

struct CONSOLECOMMAND : TSHashObject<CONSOLECOMMAND, HASHKEY_STRI> {
    const char* command;
    int32_t (*handler)(const char*, const char*);
    const char* helpText;
    CATEGORY category;
};

extern TSHashTable<CONSOLECOMMAND, HASHKEY_STRI> g_consoleCommandHash;
extern char g_commandHistory[HISTORY_DEPTH][CMD_BUFFER_SIZE];
extern uint32_t g_commandHistoryIndex;
extern char g_ExecBuffer[EXEC_BUFFER_SIZE];

void ConsoleCommandDestroy();

char* ConsoleCommandHistory(uint32_t index);

uint32_t ConsoleCommandHistoryDepth();

void ConsoleCommandInitialize();

int32_t ConsoleCommandRegister(const char* command, int32_t (*handler)(const char*, const char*), CATEGORY category, const char* helpText);

void ConsoleCommandUnregister(const char* command);

#endif
