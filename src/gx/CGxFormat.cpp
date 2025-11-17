#include "gx/CGxFormat.hpp"

const char* CGxFormat::formatToBitsString[] = {
    "16", // Fmt_Rgb565
    "24", // Fmt_ArgbX888
    "24", // Fmt_Argb8888
    "30", // Fmt_Argb2101010
    "16", // Fmt_Ds160
    "24", // Fmt_Ds24X
    "24", // Fmt_Ds248
    "32", // Fmt_Ds320
};

int32_t CGxFormat::formatToBitsUint[] = {
    16, // Fmt_Rgb565
    24, // Fmt_ArgbX888
    24, // Fmt_Argb8888
    30, // Fmt_Argb2101010
    16, // Fmt_Ds160
    24, // Fmt_Ds24X
    24, // Fmt_Ds248
    32, // Fmt_Ds320
};
