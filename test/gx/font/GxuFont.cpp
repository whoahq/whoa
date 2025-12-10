#include "gx/font/GxuFont.hpp"
#include <cstdint>
#include "catch.hpp"

TEST_CASE("GxuDetermineQuotedCode", "[gx]") {
    SECTION("does not recognize non-quoted code") {
        const char* str = "test1";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t code;
        QUOTEDCODE quotedCode;

        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_INVALIDCODE);
        REQUIRE(advance == 1);
    }

    SECTION("recognizes newlines") {
        const char* str = "test1\ntest2\rtest3\r\ntest4";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t code;
        QUOTEDCODE quotedCode;

        // \n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 1);
        str += advance;

        // \r
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 1);
        str += advance;

        // \r\n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
    }

    SECTION("recognizes quoted newlines") {
        const char* str = "test1|ntest2|n|ntest3";
        uint32_t flags = 0x0;

        int32_t advance;
        uint32_t code;
        QUOTEDCODE quotedCode;

        // |n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
        str += advance;

        // |n|n
        str += 5;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
        str += advance;
        quotedCode = GxuDetermineQuotedCode(str, advance, nullptr, flags, code);
        REQUIRE(quotedCode == CODE_NEWLINE);
        REQUIRE(advance == 2);
    }
}
