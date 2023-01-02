#include "app/mac/View.h"
#include "app/mac/EngineGLLayerView.h"
#include "app/mac/WindowCallbacks.h"
#include "gx/gll/GLWindow.h"

GLWindowCallbacks EngineViewCallbacks = {
    &MacOnResized,
    &MacOnMouseDown,
    &MacOnMouseMoved,
    &MacOnMouseUp,
    &MacOnKeyDown,
    &MacOnKeyUp
};

void AssignEngineViewCallbacks(GLWindowCallbacks* callbacks) {
    *callbacks = EngineViewCallbacks;

    // TODO
    // (callbacks + 100) = 0;

    // TODO
    // dword_12B9F54 = sub_A15850;
}

Class GetEngineViewClass() {
    return [EngineGLLayerView class];
}
