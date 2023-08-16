#include "console/Command.hpp"
#include "console/Line.hpp"

struct CategoryTranslation {
    CATEGORY categoryValue;
    char categoryString[20];
};

CategoryTranslation s_translation[] = {
    { DEBUG,    "debug"    },
    { GRAPHICS, "graphics" },
    { CONSOLE,  "console"  },
    { COMBAT,   "combat"   },
    { GAME,     "game"     },
    { DEFAULT,  "default"  },
    { NET,      "net"      },
    { SOUND,    "sound"    },
    { GM,       "gm"       }
};

int32_t ConsoleCommand_Help(const char* command, const char* arguments) {
    char buffer[128];
    bool showCategories = *arguments == '\0';

    auto numTranslation = std::size(s_translation);

    if (showCategories) {
        memset(buffer, 0, sizeof(buffer));

        ConsoleWrite("Console help categories: ", DEFAULT_COLOR);

        uint32_t offset = 0;

        for (size_t i = 0; i < numTranslation; i++) {
            auto& translation = s_translation[i];
            SStrPack(buffer, translation.categoryString, sizeof(buffer));

            if (i + 1 != numTranslation) {
                SStrPack(buffer, ", ", sizeof(buffer));
            }
        }

        ConsoleWrite(buffer, WARNING_COLOR);
        ConsoleWrite("For more information type 'help [command] or [category]'", WARNING_COLOR);
    } else {
        for (size_t i = 0; i < numTranslation; i++) {
            auto& translation = s_translation[i];

            if (SStrCmpI(translation.categoryString, arguments, STORM_MAX_STR) == 0) {
                if (translation.categoryValue != NONE) {
                    memset(buffer, 0, sizeof(buffer));
                    SStrPrintf(buffer, sizeof(buffer), "Commands registered for the category %s:", arguments);

                    ConsoleWrite(buffer, WARNING_COLOR);

                    buffer[0] = '\0';

                    uint32_t counter = 0;

                    for (auto cmd = g_consoleCommandHash.Head(); cmd; cmd = g_consoleCommandHash.Next(cmd)) {
                        if (cmd->m_category == translation.categoryValue) {
                            SStrPack(buffer, cmd->m_key.m_str, sizeof(buffer));
                            SStrPack(buffer, ", ", sizeof(buffer));

                            if (++counter == 8) {
                                ConsoleWrite(buffer, DEFAULT_COLOR);
                                buffer[0] = '\0';
                                counter = 0;
                            }
                        }
                    }

                    char* wr;

                    if (buffer[0]) {
                        auto comma = reinterpret_cast<char*>(SStrChrR(buffer, ','));
                        if (comma) {
                            *comma = 0x00;
                        }

                        wr = buffer;
                    } else {
                        wr = "NONE";
                    }

                    ConsoleWrite(wr, DEFAULT_COLOR);
                    break;
                }
            }
        }

        auto cmd = g_consoleCommandHash.Ptr(arguments);

        if (cmd == nullptr) {
            return 1;
        }

        SStrPrintf(buffer, 0xa5, "Help for command %s:", arguments);
        ConsoleWrite(buffer, WARNING_COLOR);

        auto help = cmd->m_helpText;
        if (help == nullptr) {
            help = "No help yet";
        }

        SStrPrintf(buffer, 0xa5, "     %s %s", arguments, help);
        ConsoleWrite(buffer, DEFAULT_COLOR);
    }

    return 1;
}
