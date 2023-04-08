#include "util/Filesystem.hpp"
#include <cstring>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <storm/String.hpp>
#include <StormLib.h>

static const std::vector<std::string> s_languages = {
    "enUS", "enGB", "enTW", "zhTW", "esES",
    "ruRU", "koKR", "ptPT", "esMX", "itIT",
    "deDE", "frFR", "enCN", "zhCN", "ptBR"
};

void* g_mpqHandle = nullptr;

void OsCreateDirectory(const char* pathName, int32_t recursive) {
    // TODO
}

void OsBuildFontFilePath(const char* fileName, char* buffer, size_t size) {
    SStrPrintf(buffer, size, "%s\\%s", "Fonts", fileName);
}

char* OsPathFindExtensionWithDot(char* pathName) {
    char* v1;
    char* result;

    v1 = strrchr(pathName, '\\');

    if (!v1) {
        v1 = strrchr(pathName, '/');
    }

    result = strrchr(pathName, '.');

    if (!result || (v1 && v1 >= result)) {
        result = (char*)&pathName[strlen(pathName)];
    }

    return result;
}

void OpenPatches(const std::string& language, const std::string& fileName) {
    std::string path("Data/" + language + "/");
    std::string fullPath;
    for (int i = 1; i < 20; ++i) {
        if (i < 2) {
            fullPath = path + fileName + ".MPQ";
        } else {
            fullPath = path + fileName + "-" + std::to_string(i) + ".MPQ";
        }
        if (!SFileOpenPatchArchive(g_mpqHandle, fullPath.c_str(), nullptr, 0))
            return;
    }
}

void OpenArchives()
{
    struct stat info = { 0 };

    std::string dataPath("Data/");
    if (stat(dataPath.c_str(), &info) != 0 || (info.st_mode & S_IFDIR) == 0)
        return;

    if (!SFileOpenArchive((dataPath + "common.MPQ").c_str(), 0, STREAM_FLAG_READ_ONLY, &g_mpqHandle))
        return;

    if (!SFileOpenPatchArchive(g_mpqHandle, (dataPath + "common-2.MPQ").c_str(), nullptr, 0))
        return;

    if (!SFileOpenPatchArchive(g_mpqHandle, (dataPath + "expansion.MPQ").c_str(), nullptr, 0))
        return;

    if (!SFileOpenPatchArchive(g_mpqHandle, (dataPath + "lichking.MPQ").c_str(), nullptr, 0))
        return;

    std::string language;

    for (size_t i = 0; i < s_languages.size(); ++i) {
        if (stat((dataPath + s_languages[i]).c_str(), &info) == 0 && (info.st_mode & S_IFDIR) != 0) {
            language = s_languages[i];
            break;
        }
    }

    OpenPatches(language, "locale-" + language);
    OpenPatches(language, "speech-" + language);
    OpenPatches(language, "expansion-locale-" + language);
    OpenPatches(language, "lichking-locale-" + language);
    OpenPatches(language, "expansion-speech-" + language);
    OpenPatches(language, "lichking-speech-" + language);
    OpenPatches(language, "patch-" + language);
    OpenPatches(language, "patch");
    OpenPatches(".", "patch");
}
