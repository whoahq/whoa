#include "gx/font/GxuFont.hpp"
#include <cstdint>
#include "catch.hpp"

TEST_CASE("GxuFontCreateFont", "[gx]") {
    SECTION("creates a font") {
        const char* name = "font\\FRIZQT__.TTF";
        CGxFont* face;

        auto result = GxuFontCreateFont(name, 0.5f, face, 0x0);
        REQUIRE(result != 0);
        REQUIRE(face != nullptr);

        GxuFontDestroyFont(face);
    }

    SECTION("fails to create a font that doesn't exist") {
        const char* name = "font\\DOESNOTEXIST.TTF";
        CGxFont* face;

        auto result = GxuFontCreateFont(name, 0.5f, face, 0x0);
        REQUIRE(result == 0);
        REQUIRE(face == nullptr);
    }
}

TEST_CASE("GxuDetermineQuotedCode", "[gx]") {
    SECTION("does not recognize non-quoted code") {
        const char* str = "test1";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);
    }

    SECTION("recognizes newlines") {
        const char* str = "test1\ntest2\rtest3\r\ntest4";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // \n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 1);
        str += advance;

        // \r
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 1);
        str += advance;

        // \r\n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
    }

    SECTION("recognizes quoted newlines") {
        const char* str = "test1|ntest2|n|ntest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
        str += advance;

        // |n|n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
        str += advance;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
    }

    SECTION("recognizes colors") {
        const char* str = "test1|c00123456test2|rtest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;
        CImVector color;

        // |c00123456
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, &color, flags, wide);
        REQUIRE(quotedCode == CODE_COLORON);
        REQUIRE(advance == 10);
        REQUIRE(color.value == 0xFF123456);
        str += advance;

        // |r
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_COLORRESTORE);
        REQUIRE(advance == 2);
    }

    SECTION("recognizes colors with uppercase codes") {
        const char* str = "test1|C00123456test2|Rtest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;
        CImVector color;

        // |C00123456
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, &color, flags, wide);
        REQUIRE(quotedCode == CODE_COLORON);
        REQUIRE(advance == 10);
        REQUIRE(color.value == 0xFF123456);
        str += advance;

        // |R
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_COLORRESTORE);
        REQUIRE(advance == 2);
    }

    SECTION("ignores colors when FLAG_IGNORE_COLORS is set") {
        const char* str = "test1|c00123456test2|rtest3";
        uint32_t flags = FLAG_IGNORE_COLORS;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |c00123456
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);

        // |r
        str += 15;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);
    }

    SECTION("recognizes hyperlinks") {
        const char* str = "test1|Hspell:2061:0|h[Flash Heal]|htest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |Hspell:2061:0|h
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_HYPERLINKSTART);
        REQUIRE(advance == 16);
        str += advance;

        // |h
        str += 12;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_HYPERLINKSTOP);
        REQUIRE(advance == 2);
    }

    SECTION("ignores hyperlinks when FLAG_IGNORE_HYPERLINKS is set") {
        const char* str = "test1|Hspell:2061:0|h[Flash Heal]|htest3";
        uint32_t flags = FLAG_IGNORE_HYPERLINKS;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |Hspell:2061:0|h
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);

        // |h
        str += 28;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);
    }

    SECTION("recognizes textures") {
        const char* str = "test1|TInterface\\TargetingFrame\\UI-RaidTargetingIcon_1.blp:0|ttest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |TInterface\\TargetingFrame\\UI-RaidTargetingIcon_1.blp:0|t
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_TEXTURESTART);
        REQUIRE(advance == 57);
    }

    SECTION("ignores textures when FLAG_IGNORE_TEXTURES is set") {
        const char* str = "test1|TInterface\\TargetingFrame\\UI-RaidTargetingIcon_1.blp:0|ttest3";
        uint32_t flags = FLAG_IGNORE_TEXTURES;

        int32_t advance;
        uint32_t wide;
        QUOTEDCODE quotedCode;

        // |TInterface\\TargetingFrame\\UI-RaidTargetingIcon_1.blp:0|t
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, wide);
        REQUIRE(quotedCode == CODE_INVALIDCODE);
    }
}
