#ifndef APP_MAC_MAIN_APP_H
#define APP_MAC_MAIN_APP_H

#include "app/mac/MacClient.h"
#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>

@interface MainApp : NSObject

@property (retain) NSTimer* m_pollTimer;
@property bool isPolling;

@property (retain) IBOutlet NSMenuItem* captureFrameMenuItem;
@property (retain) IBOutlet NSMenuItem* showBatchViewerMenuItem;
@property (retain) IBOutlet NSMenuItem* showGLLayerSetupMenuItem;

@end

#endif
