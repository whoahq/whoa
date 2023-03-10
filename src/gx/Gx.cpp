#include "gx/Gx.hpp"
#include "gx/Device.hpp"

const char* vsProfileNames[] = {
    "none", "vs_1_1", "vs_2_0", "vs_3_0", "vs_4_0", "vs_5_0", "arbvp1", "arbvp1_cg12", "nvvp", "nvvp2", "nvvp3", "glsl"
};

const char* hsProfileNames[] = {
    "none", "hs_5_0"
};

const char* dsProfileNames[] = {
    "none", "ds_5_0"
};

const char* gsProfileNames[] = {
    "none", "gs_4_0", "gs_5_0"
};

const char* psProfileNames[] = {
    "none", "ps_1_1", "ps_1_4", "ps_2_0", "ps_3_0", "ps_4_0", "ps_5_0", "nvrc", "nvts", "nvts2", "nvts3", "nvfp2", "arbfp1", "glsl"
};

const char* csProfileNames[] = {
    "none", "cs_5_0", "cs_cuda", "cs_ocl"
};

const char** g_gxShaderProfileNames[GxShTargets_Last] = {
    vsProfileNames,
    hsProfileNames,
    dsProfileNames,
    gsProfileNames,
    psProfileNames,
    csProfileNames
};

const CGxCaps& GxCaps() {
    return g_theGxDevicePtr->Caps();
}

bool GxCapsWindowHasFocus(int32_t a1) {
    // TODO
    return true;
}

void GxCapsWindowSize(CRect& rect) {
    g_theGxDevicePtr->CapsWindowSize(rect);
}

void GxFormatColor(CImVector& color) {
    if (GxCaps().m_colorFormat == GxCF_rgba) {
        CImVector formattedColor = {
            color.r,
            color.g,
            color.b,
            color.a
        };

        color = formattedColor;
    }
}
