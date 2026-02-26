#include "gx/RenderTarget.hpp"
#include "gx/Device.hpp"

void GxRenderTargetGet(EGxBuffer buffer, CGxTex*& gxTex) {
    g_theGxDevicePtr->RenderTargetGet(buffer, gxTex);
}
