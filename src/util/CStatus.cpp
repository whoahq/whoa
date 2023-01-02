#include "util/CStatus.hpp"
#include <cstdarg>
#include <cstdio>

CStatus CStatus::s_errorList;

void CStatus::Add(const CStatus& status) {
    // TODO
}

void CStatus::Add(STATUS_TYPE severity, const char* format, ...) {
    // TODO
    // Remove temporary console debug logging
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

CStatus& GetGlobalStatusObj() {
    return CStatus::s_errorList;
}
