#ifndef GX_C_GX_CAPS_HPP
#define GX_C_GX_CAPS_HPP

#include "gx/Types.hpp"
#include <cstdint>

class CGxCaps {
    public:
        int32_t m_numTmus = 0;
        int32_t m_pixelCenterOnEdge = 0;
        int32_t m_texelCenterOnEdge = 0;
        int32_t m_numStreams = 0;
        int32_t int10 = 0;
        EGxColorFormat m_colorFormat = GxCF_argb;
        uint32_t m_maxIndex = 0;
        int32_t m_generateMipMaps = 0;
        int32_t m_texFmt[GxTexFormats_Last] = { 0 };
        int32_t m_texTarget[GxTexTargets_Last];
        uint32_t m_texMaxSize[GxTexTargets_Last];
        EGxShVS m_vertexShaderTarget = GxShVS_none;
        EGxShPS m_pixelShaderTarget = GxShPS_none;
        int32_t m_texFilterTrilinear = 0;
        int32_t m_texFilterAnisotropic = 0;
        uint32_t m_maxTexAnisotropy = 0;
        int32_t m_depthBias = 0;
        int32_t int130 = 1;
        int32_t int134 = 0;
        int32_t int138 = 0;
};

#endif
