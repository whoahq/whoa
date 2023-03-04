#include "gx/gll/GLWindow.h"
#include "gx/gll/GLContext.h"
#include "gx/gll/GLLayerView.h"
#include "util/Autorelease.hpp"
#include <cmath>
#include <bc/Debug.hpp>

GLWindowCallbacks DefaultCallbacks = {
    // TODO
};

bool GLWindow::CanEnterFullscreenMode() {
    return true;
}

void GLWindow::CreateView() {
    System_Autorelease::ScopedPool autorelease;

    BLIZZARD_ASSERT(this->m_View == nullptr);

    GLLayerView* v1 = [this->m_ViewClass alloc];
    NSView* v2 = [this->m_Window contentView];

    NSRect v6 = v2.frame;

    this->m_View = [v1 initWithFrame: v6 glWindow: this];

    [this->m_View setAutoresizingMask: 18];

    [[this->m_Window contentView] addSubview: this->m_View];
    [this->m_Window makeFirstResponder: this->m_View];
    [this->m_Window setDelegate: this->m_View];
}

CGRect GLWindow::GetBackingRect() {
    NSRect viewFrame = [this->m_Window contentView].frame;
    NSRect backingFrame = [this->m_Window convertRectToBacking:viewFrame];

    return CGRectMake(
        0.0f,
        0.0f,
        backingFrame.size.width,
        backingFrame.size.height
    );
}

NSView* GLWindow::GetNSView() {
    return (NSView*)this->m_View;
}

CGRect GLWindow::GetRect() {
    NSRect screenFrame = [[NSScreen screens] objectAtIndex: 0].frame;
    NSRect viewFrame = [this->m_Window contentView].frame;
    NSRect windowFrame = this->m_Window.frame;

    return CGRectMake(
        windowFrame.origin.x,
        screenFrame.size.height - (viewFrame.size.height + windowFrame.origin.y),
        viewFrame.size.width,
        viewFrame.size.height
    );
}

void GLWindow::ExitFullscreenMode() {
    // TODO
}

void GLWindow::Init(const CGRect& a2, GLWindowCallbacks* a3) {
    System_Autorelease::ScopedPool autorelease;

    this->SetCallbacks(a3);

    BLIZZARD_ASSERT(this->m_Window == nullptr);

    NSScreen* primaryScreen = [[NSScreen screens] objectAtIndex:0];

    NSRect frame = primaryScreen.frame;

    NSRect contentRect = NSMakeRect(
        a2.origin.x,
        (frame.origin.y + frame.size.height) - (a2.origin.y + a2.size.height),
        a2.size.width,
        a2.size.height
    );

    NSWindow* window = [NSWindow alloc];

    [window
        initWithContentRect: contentRect
        styleMask: 15
        backing: NSBackingStoreBuffered
        defer: NO];

    this->m_Window = window;

    [window
        setAcceptsMouseMovedEvents: YES];

    [window
        setReleasedWhenClosed: NO];

    [window
        setBackgroundColor: [NSColor blackColor]];

    BLIZZARD_ASSERT(this->m_Window != nullptr);

    // TODO
    // return 1;
}

void GLWindow::Resize(uint32_t width, uint32_t height) {
    auto rect = this->GetRect();

    if (std::floor(rect.size.width) != width || std::floor(rect.size.height) != height) {
        auto size = CGSizeMake(
            (static_cast<float>(width / 65536) * 65536.0f) + static_cast<uint16_t>(width),
            (static_cast<float>(height / 65536) * 65536.0f) + static_cast<uint16_t>(height)
        );

        [this->m_Window setContentSize: size];
    }
}

void GLWindow::SetCallbacks(GLWindowCallbacks* callbacks) {
    if (callbacks) {
        this->m_Callbacks = callbacks;
        this->m_ActiveCallbacks = callbacks;
    } else {
        this->m_Callbacks = &DefaultCallbacks;
        this->m_ActiveCallbacks = &DefaultCallbacks;
    }
}

void GLWindow::SetOpenGLContext(GLContext* a2) {
    BLIZZARD_ASSERT(this->m_View != nullptr);

    System_Autorelease::ScopedPool autorelease;

    if (a2) {
        if ([a2->m_Context->context view]) {
            [a2->m_Context->context clearDrawable];
        }

        if (this->m_Context) {
            [this->m_Context->m_Context->context clearDrawable];
        }

        NSOpenGLContext* currentContext = GLContext::GetNSOpenGLCurrentContext();

        if (currentContext) {
            [currentContext clearDrawable];
        }

        [a2->m_Context->context setView: this->m_View];

        if (![a2->m_Context->context view]) {
            puts("*** INVALID VIEW ***");
        }

        a2->Update();

        this->m_Context = a2;
    } else {
        if (!this->m_Context) {
            this->m_Context = a2;
        } else {
            [this->m_Context->m_Context->context clearDrawable];
            this->m_Context = nullptr;
        }
    }
}

void GLWindow::SetTitle(const char* title) {
    // TODO
    // sub_BD280(v5);

    [this->m_Window
        setTitle: [NSString stringWithUTF8String: title]];


    // TODO
    // sub_BD2C0(v5);
}

void GLWindow::SetViewClass(Class viewClass) {
    if (![viewClass isSubclassOfClass: [GLLayerView class]]) {
        // TODO
        // sub_C2080(v3);
    }

    this->m_ViewClass = viewClass;
}

void GLWindow::Show() {
    if (this->m_FullscreenWindow) {
        [this->m_FullscreenWindow
            makeKeyAndOrderFront: 0];
        [this->m_FullscreenWindow
            makeFirstResponder: (NSView*)this->m_View];

        this->m_Shown = true;
    } else {
        this->Sub70760();
    }
}

void GLWindow::Sub70760() {
    // TODO
    // sub_BD280(v4);

    [this->m_Window
        makeKeyAndOrderFront: 0];

    if (this->GetNSView()) {
        NSView* view = this->GetNSView();

        [this->m_Window
            makeFirstResponder: view];
    }

    this->m_Shown = true;

    // TODO
    // sub_BD2C0(v4);
}
