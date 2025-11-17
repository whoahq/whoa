#ifndef GX_C_GX_FORMAT_HPP
#define GX_C_GX_FORMAT_HPP

#include <cstdint>
#include <tempest/Vector.hpp>

class CGxFormat {
    public:
        // Types
        enum Format {
            Fmt_Rgb565 = 0,
            Fmt_ArgbX888 = 1,
            Fmt_Argb8888 = 2,
            Fmt_Argb2101010 = 3,
            Fmt_Ds160 = 4,
            Fmt_Ds24X = 5,
            Fmt_Ds248 = 6,
            Fmt_Ds320 = 7,
            Formats_Last = 8
        };

        // Static variables
        static int32_t formatToBitsUint[Formats_Last];
        static const char* formatToBitsString[Formats_Last];

        // Member variables
        bool hwTnL;
        int8_t window;
        int32_t maximize;
        Format depthFormat;
        C2iVector size;
        uint32_t multisampleCount;
        Format colorFormat;
        uint32_t refreshRate;
        uint32_t vsync;
        C2iVector pos;
};

#endif
