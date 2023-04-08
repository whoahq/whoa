#ifndef UTIL_FILESYSTEM_HPP
#define UTIL_FILESYSTEM_HPP

#include <cstddef>
#include <cstdint>

void OsCreateDirectory(const char*, int32_t);

void OsBuildFontFilePath(const char*, char*, size_t);

char* OsPathFindExtensionWithDot(char*);

void OsFileToNativeSlashes(char* path, size_t size = (size_t) -1);
void OsFileToForwardSlashes(char* path, size_t size = (size_t) -1);
void OsFileToBackSlashes(char* path, size_t size = (size_t) -1);

void OpenArchives();

extern void* g_mpqHandle;

#endif
