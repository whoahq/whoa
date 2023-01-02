#include "gx/RenderState.hpp"
#include "gx/Shader.hpp"
#include "gx/Device.hpp"
#include "gx/Types.hpp"
#include "gx/texture/CGxTex.hpp"
#include <storm/Error.hpp>

void GxRsPop() {
    g_theGxDevicePtr->RsPop();
}

void GxRsPush() {
    g_theGxDevicePtr->RsPush();
}

void GxRsSet(EGxRenderState which, int32_t value) {
    STORM_ASSERT(which < GxRenderStates_Last);
    g_theGxDevicePtr->RsSet(which, value);
}

void GxRsSet(EGxRenderState which, CGxShader* value) {
    g_theGxDevicePtr->RsSet(which, value);
}

void GxRsSet(EGxRenderState which, CGxTex* value) {
    g_theGxDevicePtr->RsSet(which, value);
}

void GxRsSetAlphaRef() {
    g_theGxDevicePtr->RsSetAlphaRef();
}
