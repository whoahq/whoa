#ifndef UTIL_FILESYSTEM_HPP
#define UTIL_FILESYSTEM_HPP

#include <cstddef>
#include <cstdint>

void OsCreateDirectory(const char*, int32_t);

void OsBuildFontFilePath(const char*, char*, size_t);

char* OsPathFindExtensionWithDot(char*);

#endif
