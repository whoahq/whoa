#include "gx/LoadingScreen.hpp"
#include "gx/Screen.hpp"

static HLAYER s_loadingScreenLayer;

bool LoadingScreenDrawing() {
    return s_loadingScreenLayer != nullptr;
}
