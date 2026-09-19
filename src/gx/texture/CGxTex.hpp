#ifndef GX_TEXTURE_C_GX_TEX_HPP
#define GX_TEXTURE_C_GX_TEX_HPP

#include "gx/Types.hpp"
#include <cstdint>
#include <tempest/Rect.hpp>

class CGxTexFlags {
    public:
        // Member variables
        uint32_t m_filter : 3;
        uint32_t m_wrapU : 1;
        uint32_t m_wrapV : 1;
        uint32_t m_forceMipTracking : 1;
        uint32_t m_generateMipMaps : 1;
        uint32_t m_renderTarget : 1;
        uint32_t m_maxAnisotropy : 5;
        uint32_t m_bit13 : 1;
        uint32_t m_bit14 : 1;
        uint32_t m_bit15 : 1;

        // Member functions
        CGxTexFlags()
            : CGxTexFlags(GxTex_Linear, 0, 0, 0, 0, 0, 1)
            {};
        CGxTexFlags(EGxTexFilter, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
        bool operator==(const CGxTexFlags& texFlags);
};

class CGxTexParms {
    public:
        // Member variables
        EGxTexTarget target;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        EGxTexFormat format;
        EGxTexFormat dataFormat;
        CGxTexFlags flags = CGxTexFlags(GxTex_Linear, 0, 0, 0, 0, 0, 1);
        void* userArg;
        void (*userFunc)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&);
        const char* name = "";
};

class CGxTex {
    public:
        // Member variables
        CiRect m_updateRect = { 0, 0, 0, 0 };
        int16_t m_updatePlaneMin = -1;
        int16_t m_updatePlaneMax = -1;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_depth;
        EGxTexTarget m_target;
        EGxTexFormat m_format;
        EGxTexFormat m_dataFormat;
        CGxTexFlags m_flags = CGxTexFlags(GxTex_Linear, 0, 0, 0, 0, 0, 1);
        void* m_userArg;
        void (*m_userFunc)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&);
        void* m_apiSpecificData;
        void* m_apiSpecificData2; // invented name
        uint8_t m_needsUpdate;
        uint8_t m_needsCreation;
        uint8_t m_needsFlagUpdate;
        char m_name[260];

        // Member functions
        CGxTex(EGxTexTarget, uint32_t, uint32_t, uint32_t, EGxTexFormat, EGxTexFormat, CGxTexFlags, void*, void (*)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char*);
        float GetHeight(void);
        float GetWidth(void);
};

#endif
