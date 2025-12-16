#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "gx/font/FreeType.hpp"

int main(int argc, char* argv[]) {
    // Global initialization
    FreeTypeInitialize();

    int result = Catch::Session().run( argc, argv );

    // Global cleanup
    // TODO

    return result;
}
