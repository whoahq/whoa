#ifndef UTIL_LOG_HPP
#define UTIL_LOG_HPP

#include <cstdint>

enum SYSMSG_TYPE {
    SYSMSG_INFO         = 0x0,
    SYSMSG_WARNING      = 0x1,
    SYSMSG_ERROR        = 0x2,
    SYSMSG_FATAL        = 0x3,
    SYSMSG_NUMTYPES     = 0x4
};

struct HSLOG {
    int32_t unused;
};

bool SLogCreate(const char*, uint32_t, void*);

void SysMsgPrintf(SYSMSG_TYPE, const char*, ...);

#endif
