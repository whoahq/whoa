#include "client/Client.hpp"
#include "bc/os/CommandLine.hpp"

int main(int argc, char* argv[]) {
    // TODO

    OsSetCommandLine(argc, argv);

    CommonMain();

    // TODO

    return 0;
}
