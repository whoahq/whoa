#include "app/mac/WoWApplication.h"
#include "app/mac/MacClient.h"
#include "client/Client.hpp"
#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>

int32_t main(int32_t argc, char* argv[]) {
    // TODO
    // MacClient::SetupCommandLine(argc, argv, v10);

    if (MacClient::IsUsingGLLayer()) {
        // TODO
        // GxSetRequestedApi(3);

        // TODO
        // OsInputSetIsUsingCocoaEventLoop(1);

        [WoWApplication sharedApplication];

        #if WHOA_SYSTEM_VERSION < WHOA_MACOS_10_8
            [NSBundle
                loadNibNamed: @"MainMenu"
                owner: NSApp];
        #endif

        #if WHOA_SYSTEM_VERSION >= WHOA_MACOS_10_8
            [[NSBundle mainBundle]
                loadNibNamed: @"MainMenu"
                owner: NSApp
                topLevelObjects: nil];
        #endif

        [NSRunLoop currentRunLoop];

        [NSApp mainMenu];

        CommonMain();
    }

    return 0;
}
