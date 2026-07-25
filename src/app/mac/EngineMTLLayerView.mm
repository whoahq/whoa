#include "app/mac/EngineMTLLayerView.h"
#import <QuartzCore/CAMetalLayer.h>

@implementation EngineMTLLayerView

- (CALayer*)makeBackingLayer {
    return [CAMetalLayer layer];
}

- (id)initWithFrame:(NSRect)frame glWindow:(GLWindow*)window {
    self = [super initWithFrame:frame glWindow:window];

    if (self) {
        [self setWantsLayer:YES];
    }

    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Rendering is driven by CGxDeviceMTL.
}

- (void)update {
    [super update];

    if (![self.layer isKindOfClass:[CAMetalLayer class]]) {
        return;
    }

    CAMetalLayer* layer = (CAMetalLayer*)self.layer;
    CGSize size = [self convertSizeToBacking:self.bounds.size];
    layer.drawableSize = size;
}

@end
