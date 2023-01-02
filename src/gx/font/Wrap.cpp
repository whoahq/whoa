#include "gx/font/Wrap.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/Font.hpp"
#include <cmath>
#include <cwctype>
#include <storm/Unicode.hpp>

void CalcWrapPoint(CGxFont* face, const char* currentText, float fontHeight, float blockWidth, uint32_t* numBytes, float* extent, const char** nextText, float a8, uint32_t flags, bool* a10, float* a11, float scale) {
    if (fontHeight < 0.0f || blockWidth <= 0.0f || !currentText || !*currentText) {
        *numBytes = 0;
        *extent = 0.0f;
        *nextText = nullptr;

        return;
    }

    if (flags & 0x80) {
        CalcWrapPointBillboarded(
            currentText,
            flags,
            face,
            fontHeight,
            numBytes,
            extent,
            nextText,
            a11,
            scale
        );
    } else {
        CalcWrapPointNonBillboarded(
            currentText,
            face,
            fontHeight,
            blockWidth,
            numBytes,
            extent,
            nextText,
            a8,
            flags,
            a10,
            a11,
            scale
        );
    }
}

void CalcWrapPointBillboarded(const char* currentText, uint32_t flags, CGxFont* face, float fontHeight, uint32_t* numBytes, float* extent, const char** nextText, float* a8, float scale) {
    // TODO
}

void CalcWrapPointNonBillboarded(const char* currentText, CGxFont* face, float fontHeight, float blockWidth, uint32_t* numBytes, float* extent, const char** nextText, float a8, uint32_t flags, bool* a10, float* a11, float scale) {
    if (fontHeight == 0.0f || flags & 0x4) {
        fontHeight = GxuFontGetOneToOneHeight(face);
    }

    float v46 = 0.0f;

    if (a8 > 0.0f) {
        v46 += ceil(ScreenToPixelWidth(0, a8));
    }

    if (face->m_flags & 0x8) {
        v46 += 4.0f;
    } else if (face->m_flags & 0x1) {
        v46 += 2.0f;
    }

    float pixelFontHeight = ScreenToPixelHeight(0, fontHeight);
    float v41 = static_cast<float>(face->GetPixelSize()) / pixelFontHeight;
    float pixelBlockWidth = ceil(v41 * blockWidth * static_cast<float>(GetScreenPixelWidth()));

    if (a11) {
        *a11 = 0.0f;
    }

    uint32_t v18 = 0;
    uint32_t prevCode = 0;
    float v42 = 0.0f;
    float v56 = 0.0f;
    auto startText = currentText;
    int32_t advance = 0;
    QUOTEDCODE quotedCode = CODE_NEWLINE;

    while (*currentText) {
        uint32_t code;
        quotedCode = GxuDetermineQuotedCode(currentText, advance, 0, flags, code);

        if (quotedCode == CODE_NEWLINE) {
            currentText += advance;
            v18 = currentText - startText;
            v56 = v42 + v46;

            break;
        }

        if (
            quotedCode == CODE_COLORON
            || quotedCode == CODE_COLORRESTORE
            || quotedCode == CODE_HYPERLINKSTART
            || quotedCode == CODE_HYPERLINKSTOP
            || quotedCode == CODE_TEXTURESTOP
        ) {
            currentText += advance;

            continue;
        }

        if (quotedCode == CODE_TEXTURESTART) {
            GXUEMBEDDEDTEXTUREINFO textureInfo;

            // TODO
            // if (!ParseEmbeddedTexture(currentText, textureInfo, pixelFontHeight, scale, fontHeight)) {
            //     currentText += advance;
            //
            //     continue;
            // }

            float v21 = textureInfo.float1C + v46;

            if (pixelBlockWidth >= v21) {
                v46 = v21;

                if (a11 && *a11 < textureInfo.float18) {
                    *a11 = textureInfo.float18;
                }

                currentText += advance;

                continue;
            }

            prevCode = code;
            advance = 0;
        }

        if (face->NewCodeDesc(code)) {
            float step = 0.0f;

            if (prevCode) {
                if (flags & 0x10) {
                    step = face->ComputeStepFixedWidth(prevCode, code);
                } else {
                    step = face->ComputeStep(prevCode, code);
                }
            }

            if (CanWrapBetween(code, prevCode)) {
                if (*a10 || v18) {
                    v18 = currentText - startText;
                }

                *a10 = true;
                v56 = v42 + v46;
            }

            float v38 = GetCharacterWidth(&currentText[advance], flags, code, face, fontHeight);
            float v24 = step + v46;
            if (pixelBlockWidth < v38 + v24) {
                break;
            }

            prevCode = code;
            v46 = v24;
            v42 = v38;
        }

        currentText += advance;
    }

    v46 = (v42 + v46) / static_cast<float>(GetScreenPixelWidth());
    float v57 = v56 / static_cast<float>(GetScreenPixelWidth());
    v41 = pixelFontHeight / static_cast<float>(face->GetPixelSize());

    if (quotedCode == CODE_NEWLINE || !*currentText) {
        *extent = v46 * v41;
        *nextText = currentText;
        *numBytes = currentText - startText;
    } else {
        if (!v18 || flags & 0x2) {
            *a10 = flags & 0x40;
            *numBytes = currentText - startText;
            *extent = v46 * v41;
        } else {
            *numBytes = v18;
            *extent = v57 * v41;
            currentText = &startText[v18];
        }

        while (*currentText) {
            auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(currentText), &advance);

            if (!iswspace(code)) {
                break;
            }

            currentText += advance;
        }

        *nextText = currentText;
    }
}

int32_t CanWrapBetween(uint32_t codeA, uint32_t codeB) {
    if (!codeB) {
        return 0;
    }

    if (codeB != '-' && codeB != ';' && codeB != '/' && codeA != ('|') && codeA != '\xFF\xFF\xFF\xFF') {
        if (iswspace(codeB)) {
            return 0;
        }

        if (!iswspace(codeA)) {
            bool v3, v5;

            if (codeB > 0x3008) {
                if (codeB <= 0x3014) {
                    if (codeB != 0x3014) {
                        switch (codeB) {
                            case 0x300A:
                            case 0x300C:
                            case 0x300E:
                            case 0x3010:
                                return 0;

                            default:
                                goto LABEL_18;
                        }
                    }

                    return 0;
                }

                if (codeB > 0xFF08) {
                    switch (codeB) {
                        case 0xFF3B:
                        case 0xFF5B:
                        case 0xFFE1:
                        case 0xFFE5:
                        case 0xFFE6:
                            return 0;

                        default:
                            goto LABEL_18;
                    }
                }

                if (codeB == 0xFF08) {
                    return 0;
                }

                if (codeB > 0xFE5B) {
                    if (codeB == 0xFE5D) {
                        return 0;
                    }

                    v3 = codeB == 0xFF04;
                } else {
                    if (codeB == 0xFE5B || codeB == 0x301D) {
                        return 0;
                    }

                    v3 = codeB == 0xFE59;
                }
            } else {
                if (codeB == 0x3008) {
                    return 0;
                }

                if (codeB <= '{') {
                    if (codeB != '{') {
                        switch (codeB) {
                            case '$':
                            case '(':
                            case '[':
                            case '\\':
                                return 0;

                            default:
                                goto LABEL_18;
                        }
                    }

                    return 0;
                }

                if (codeB == 0x2018 || codeB == 0x201C) {
                    return 0;
                }

                v3 = codeB == 0x2035;
            }

            if (v3) {
                return 0;
            }

LABEL_18:
            if (codeA <= 0x2014) {
                if (codeA < 0x2013) {
                    switch (codeA) {
                        case '!':
                        case '%':
                        case ')':
                        case ',':
                        case '.':
                        case ':':
                        case ';':
                        case '?':
                        case ']':
                        case '}':
                        case 0xB0:
                        case 0xB7:
                            return 0;

                        default:
                            return codeB == 0x3002
                                || codeB == 0xFF0C
                                || (codeA >= 0x1100 && codeA <= 0x11FF)
                                || (codeA >= 0x3000 && codeA <= 0xD7AF)
                                || (codeA >= 0xF900 && codeA <= 0xFAFF)
                                || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                                || codeA - 0xFFA0 <= 0x3C;
                    }
                }

                return 0;
            }

            if (codeA > 0xFE30) {
                if (codeA > 0xFF5D) {
                    switch (codeA) {
                        case 0xFF70:
                        case 0xFF9E:
                        case 0xFF9F:
                        case 0xFFE0:
                            return 0;

                        default:
                            return codeB == 0x3002
                                || codeB == 0xFF0C
                                || (codeA >= 0x1100 && codeA <= 0x11FF)
                                || (codeA >= 0x3000 && codeA <= 0xD7AF)
                                || (codeA >= 0xF900 && codeA <= 0xFAFF)
                                || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                                || codeA - 0xFFA0 <= 0x3C;
                    }
                }

                if (codeA != 0xFF5D) {
                    switch (codeA) {
                        case 0xFE50:
                        case 0xFE51:
                        case 0xFE52:
                        case 0xFE54:
                        case 0xFE55:
                        case 0xFE56:
                        case 0xFE57:
                        case 0xFE5A:
                        case 0xFE5C:
                        case 0xFE5E:
                        case 0xFF01:
                        case 0xFF05:
                        case 0xFF09:
                        case 0xFF0C:
                        case 0xFF0E:
                        case 0xFF1A:
                        case 0xFF1B:
                        case 0xFF1F:
                        case 0xFF3D:
                            return 0;

                        default:
                            return codeB == 0x3002
                                || codeB == 0xFF0C
                                || (codeA >= 0x1100 && codeA <= 0x11FF)
                                || (codeA >= 0x3000 && codeA <= 0xD7AF)
                                || (codeA >= 0xF900 && codeA <= 0xFAFF)
                                || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                                || codeA - 0xFFA0 <= 0x3C;
                    }
                }

                return 0;
            }

            if (codeA == 0xFE30) {
                return 0;
            }

            if (codeA > 0x3009) {
                switch (codeA) {
                    case 0x300B:
                    case 0x300D:
                    case 0x300F:
                    case 0x3011:
                    case 0x3015:
                    case 0x301E:
                    case 0x30FC:
                        return 0;

                    default:
                        return codeB == 0x3002
                            || codeB == 0xFF0C
                            || (codeA >= 0x1100 && codeA <= 0x11FF)
                            || (codeA >= 0x3000 && codeA <= 0xD7AF)
                            || (codeA >= 0xF900 && codeA <= 0xFAFF)
                            || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                            || codeA - 0xFFA0 <= 0x3C;
                }
            }

            if (codeA == 0x3009) {
                return 0;
            }

            if (codeA <= 0x2027) {
                if (codeA >= 0x2026 || codeA == 0x2019 || codeA == 0x201D || codeA == 0x2022) {
                    return 0;
                }

                return codeB == 0x3002
                    || codeB == 0xFF0C
                    || (codeA >= 0x1100 && codeA <= 0x11FF)
                    || (codeA >= 0x3000 && codeA <= 0xD7AF)
                    || (codeA >= 0xF900 && codeA <= 0xFAFF)
                    || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                    || codeA - 0xFFA0 <= 0x3C;
            }

            if (codeA > 0x2103) {
                if (codeA < 0x3001) {
                    return codeB == 0x3002
                        || codeB == 0xFF0C
                        || (codeA >= 0x1100 && codeA <= 0x11FF)
                        || (codeA >= 0x3000 && codeA <= 0xD7AF)
                        || (codeA >= 0xF900 && codeA <= 0xFAFF)
                        || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                        || codeA - 0xFFA0 <= 0x3C;
                }

                v5 = codeA <= 0x3002;
            } else {
                if (codeA == 0x2103) {
                    return 0;
                }

                if (codeA < 0x2032) {
                    return codeB == 0x3002
                        || codeB == 0xFF0C
                        || (codeA >= 0x1100 && codeA <= 0x11FF)
                        || (codeA >= 0x3000 && codeA <= 0xD7AF)
                        || (codeA >= 0xF900 && codeA <= 0xFAFF)
                        || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                        || codeA - 0xFFA0 <= 0x3C;
                }

                v5 = codeA <= 0x2033;
            }

            if (v5) {
                return 0;
            }

            return codeB == 0x3002
                || codeB == 0xFF0C
                || (codeA >= 0x1100 && codeA <= 0x11FF)
                || (codeA >= 0x3000 && codeA <= 0xD7AF)
                || (codeA >= 0xF900 && codeA <= 0xFAFF)
                || (codeA >= 0xFF00 && codeA <= 0xFF9F)
                || codeA - 0xFFA0 <= 0x3C;
        }
    }

    return 1;
}
