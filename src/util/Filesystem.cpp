#include "util/Filesystem.hpp"
#include <cstring>
#include <storm/String.hpp>
#include <StormLib.h>

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

void OpenArchives()
{

}
