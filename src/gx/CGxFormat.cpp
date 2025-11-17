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

int32_t AdapterFormatSort(const void* a, const void* b) {
    auto formatA = static_cast<const CGxFormat*>(a);
    auto formatB = static_cast<const CGxFormat*>(b);

    auto sizeSort = (formatA->size.x * formatA->size.y) - (formatB->size.x * formatB->size.y);

    if (sizeSort != 0) {
        return sizeSort;
    }

    auto colorSort = formatA->colorFormat - formatB->colorFormat;

    if (colorSort != 0) {
        return colorSort;
    }

    auto multisampleSort = formatA->multisampleCount - formatB->multisampleCount;

    return multisampleSort;
}
