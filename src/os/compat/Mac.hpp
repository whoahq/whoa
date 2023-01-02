#ifndef OS_COMPAT_MAC_HPP
#define OS_COMPAT_MAC_HPP

#ifdef __OBJC__
    #include <AppKit/AppKit.h>

    #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_12
        #define NSEventTypeApplicationDefined NSApplicationDefined
    #endif
#endif

#endif
