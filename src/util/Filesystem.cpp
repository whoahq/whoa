#include "util/Filesystem.hpp"
#include <cstring>
#include <sys/stat.h>
#include <storm/String.hpp>
#include <StormLib.h>

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

void OsFileToNativeSlashes(char* path, size_t size) {
#ifdef WHOA_SYSTEM_WIN
    OsFileToBackSlashes(path, size);
#else
    OsFileToForwardSlashes(path, size);
#endif
}

void OsFileToForwardSlashes(char* path, size_t size) {
    if (!path)
        return;

    for (size_t i = 0; i < size; ++i) {
        if (!path[i])
            return;

        if (path[i] == '\\')
            path[i] = '/';
    }
}

void OsFileToBackSlashes(char* path, size_t size) {
    if (!path)
        return;

    for (size_t i = 0; i < size; ++i) {
        if (!path[i])
            return;

        if (path[i] == '/')
            path[i] = '\\';
    }
}

void OpenExtraArchives(const char* basePath, const char* language, const char* fileNamePrefix) {
    char path[STORM_MAX_PATH] = { 0 };

    for (unsigned int i = 1; i < 20; ++i) {
        if (language) {
            if (i > 1) {
                SStrPrintf(path, sizeof(path), "%s/%s-%s-%u.MPQ", basePath, fileNamePrefix, language, i);
            } else {
                SStrPrintf(path, sizeof(path), "%s/%s-%s.MPQ", basePath, fileNamePrefix, language);
            }
        } else {
            if (i > 1) {
                SStrPrintf(path, sizeof(path), "%s/%s-%u.MPQ", basePath, fileNamePrefix, i);
            } else {
                SStrPrintf(path, sizeof(path), "%s/%s.MPQ", basePath, fileNamePrefix);
            }
        }

        OsFileToNativeSlashes(path);
        if (!SFileOpenPatchArchive(g_mpqHandle, path, nullptr, MPQ_OPEN_NO_LISTFILE))
            return;
    }
}

void OpenArchives()
{
    struct stat info = { 0 };

    const char* languages[] = {
        "enUS", "enGB", "enTW", "zhTW", "esES",
        "ruRU", "koKR", "ptPT", "esMX", "itIT",
        "deDE", "frFR", "enCN", "zhCN", "ptBR"
    };

    const char* baseFiles[] = {
        "common.MPQ",
        "common-2.MPQ",
        "expansion.MPQ",
        "lichking.MPQ"
    };

    const char* extraFiles[] = {
        "locale",
        "speech",
        "expansion-locale",
        "lichking-locale",
        "expansion-speech",
        "lichking-speech",
        "patch"
    };

    char path[STORM_MAX_PATH] = { 0 };

    SStrCopy(path, "Data", sizeof(path));
    if (stat(path, &info) != 0 || (info.st_mode & S_IFDIR) == 0)
        return;

    for (size_t i = 0; i < sizeof(baseFiles) / sizeof(baseFiles[0]); ++i) {
        SStrCopy(path, "Data/", sizeof(path));
        SStrPack(path, baseFiles[i], sizeof(path));
        OsFileToNativeSlashes(path);

        if (i == 0) {
            if (!SFileOpenArchive(path, 0, MPQ_OPEN_NO_LISTFILE, &g_mpqHandle))
                return;
        } else if (!SFileOpenPatchArchive(g_mpqHandle, path, nullptr, MPQ_OPEN_NO_LISTFILE)) {
            return;
        }
    }

    const char* language = nullptr;
    for (size_t i = 0; i < sizeof(languages) / sizeof(languages[0]); ++i) {
        SStrCopy(path, "Data/", sizeof(path));
        SStrPack(path, languages[i], sizeof(path));
        OsFileToNativeSlashes(path);

        if (stat(path, &info) == 0 && (info.st_mode & S_IFDIR) != 0) {
            language = languages[i];
            break;
        }
    }

    if (language) {
        SStrCopy(path, "Data/", sizeof(path));
        SStrPack(path, language, sizeof(path));

        for (size_t i = 0; i < sizeof(extraFiles) / sizeof(extraFiles[0]); ++i)
            OpenExtraArchives(path, language, extraFiles[i]);

        // Special case
        OpenExtraArchives(path, nullptr, "patch");
    }

    SStrCopy(path, "Data", sizeof(path));
    OpenExtraArchives(path, nullptr, "patch");
}
