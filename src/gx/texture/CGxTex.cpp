#include "gx/texture/CGxTex.hpp"
#include "gx/Gx.hpp"
#include <algorithm>

CGxTexFlags::CGxTexFlags(EGxTexFilter filter, uint32_t wrapU, uint32_t wrapV, uint32_t force, uint32_t generateMipMaps, uint32_t renderTarget, uint32_t maxAnisotropy) {
    this->m_filter = filter;
    this->m_wrapU = wrapU;
    this->m_wrapV = wrapV;
    this->m_forceMipTracking = force;
    this->m_generateMipMaps = generateMipMaps;
    this->m_renderTarget = renderTarget;
    this->m_maxAnisotropy = std::min(maxAnisotropy, GxCaps().m_maxTexAnisotropy);

    // TODO
    this->m_bit13 = 0;
    this->m_bit14 = 0;
    this->m_bit15 = 0;
}

bool CGxTexFlags::operator==(const CGxTexFlags& texFlags) {
    return this->m_filter == texFlags.m_filter
        && this->m_wrapU == texFlags.m_wrapU
        && this->m_wrapV == texFlags.m_wrapV
        && this->m_forceMipTracking == texFlags.m_forceMipTracking
        && this->m_generateMipMaps == texFlags.m_generateMipMaps
        && this->m_renderTarget == texFlags.m_renderTarget
        && this->m_maxAnisotropy == texFlags.m_maxAnisotropy
        && this->m_bit13 == texFlags.m_bit13
        && this->m_bit14 == texFlags.m_bit14
        && this->m_bit15 == texFlags.m_bit15;
}

CGxTex::CGxTex(EGxTexTarget target, uint32_t width, uint32_t height, uint32_t depth, EGxTexFormat format, EGxTexFormat dataFormat, CGxTexFlags flags, void* userArg, void (*userFunc)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char* name) {
    this->m_updateRect = { 0, 0, static_cast<int32_t>(height), static_cast<int32_t>(width) };
    this->m_target = target;
    this->m_width = width;
    this->m_height = height;
    this->m_dataFormat = dataFormat;
    this->m_format = format;
    this->m_depth = depth;
    this->m_userFunc = userFunc;
    this->m_userArg = userArg;
    this->m_flags = flags;
    this->m_apiSpecificData = nullptr;
    this->m_apiSpecificData2 = nullptr;
    this->m_needsUpdate = 1;
    this->m_needsFlagUpdate = 1;
    this->m_needsCreation = 1;

    // TODO remaining constructor logic
}

float CGxTex::GetHeight() {
    return this->m_height;
}

float CGxTex::GetWidth() {
    return this->m_width;
}
