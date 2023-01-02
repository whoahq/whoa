#ifndef APP_MAC_VIEW_H
#define APP_MAC_VIEW_H

#include <objc/objc-runtime.h>

struct GLWindowCallbacks;

void AssignEngineViewCallbacks(GLWindowCallbacks* callbacks);

Class GetEngineViewClass();

#endif
