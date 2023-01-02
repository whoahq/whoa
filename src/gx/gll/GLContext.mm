#include "gx/gll/GLContext.h"
#include "gx/gll/GLDevice.h"
#include "util/Autorelease.hpp"

NSOpenGLContext* GLContext::s_MainContext;
Blizzard::Thread::TLSSlot GLContext::s_CurrentContext;
Blizzard::Thread::TLSSlot GLContext::s_CurrentGLContext;
CFDictionaryRef GLContext::s_DesktopMode;

void* Sub2A1E0(void* ptr) {
    NSOpenGLContext** ptrptr = new NSOpenGLContext*;
    *ptrptr = nullptr;
    return ptrptr;
}

void Sub2A200(void* ptr) {
    delete static_cast<NSOpenGLContext**>(ptr);
}

void* Sub720A0(void* ptr) {
    GLContext** ptrptr = new GLContext*;
    *ptrptr = nullptr;
    return ptrptr;
}

void Sub720C0(void* ptr) {
    delete static_cast<GLContext**>(ptr);
}

GLContext::Context::~Context() {
    if (this->context) {
        glFlush();

        [this->context clearDrawable];

        if (GLContext::GetCurrentContext() == context) {
            [NSOpenGLContext clearCurrentContext];
            GLContext::SetCurrentContext(nullptr);
        }

        [this->context release];
    }

    [this->pixelFormat release];
}

NSOpenGLContext* GLContext::GetNSOpenGLCurrentContext() {
    return [NSOpenGLContext currentContext];
}

NSOpenGLContext* GLContext::GetCurrentContext() {
    return *static_cast<NSOpenGLContext**>(
        Blizzard::Thread::RegisterLocalStorage(&GLContext::s_CurrentContext, Sub2A1E0, 0, Sub2A200)
    );
}

void GLContext::SetCurrentContext(NSOpenGLContext* context) {
    *static_cast<NSOpenGLContext**>(
        Blizzard::Thread::RegisterLocalStorage(&GLContext::s_CurrentContext, Sub2A1E0, 0, Sub2A200)
    ) = context;
}

GLContext* GLContext::GetCurrentGLContext() {
    return *static_cast<GLContext**>(
        Blizzard::Thread::RegisterLocalStorage(&GLContext::s_CurrentGLContext, Sub720A0, 0, Sub720C0)
    );
}

void GLContext::SetCurrentGLContext(GLContext* context) {
    *static_cast<GLContext**>(
        Blizzard::Thread::RegisterLocalStorage(&GLContext::s_CurrentGLContext, Sub720A0, 0, Sub720C0)
    ) = context;
}

GLContext::GLContext(GLDevice* a2, const char* a3) {
    this->m_Context = nullptr;
    this->m_Window = nullptr;
    this->m_Windowed = false;
    this->m_MTGLEnabled = false;
    this->m_Device = a2;
    this->m_Width = 0;
    this->m_Height = 0;
    this->m_RefreshRate = 0;
}

int32_t GLContext::GetBackingWidth() {
    if (this->m_Windowed) {
        return this->m_Window->GetBackingWidth();
    } else {
        return this->m_Width;
    }
}

int32_t GLContext::GetBackingHeight() {
    if (this->m_Windowed) {
        return this->m_Window->GetBackingHeight();
    } else {
        return this->m_Height;
    }
}

int32_t GLContext::GetWidth() {
    if (this->m_Windowed) {
        return this->m_Window->GetWidth();
    } else {
        return this->m_Width;
    }
}

int32_t GLContext::GetHeight() {
    if (this->m_Windowed) {
        return this->m_Window->GetHeight();
    } else {
        return this->m_Height;
    }
}

bool GLContext::IsCurrentContext() {
    return GLContext::GetCurrentGLContext() == this;
}

void GLContext::MakeCurrent(bool a2) {
    BLIZZARD_ASSERT(this->m_Context->context != nullptr);

    if (a2) {
        NSOpenGLContext* v6 = GLContext::GetNSOpenGLCurrentContext();
        GLContext::SetCurrentContext(v6);
    }

    if (this->m_Context->context != GLContext::GetCurrentContext()) {
        int32_t mtglEnabled = 0;

        if (GLContext::GetCurrentContext()) {
            mtglEnabled = this->m_MTGLEnabled;

            // TODO
            // this->ToggleMTGL(0);
            // GLOcclusionQuery::DeleteAllQueries();
            // GLFence::DeleteAllFences();

            glFlush();
        }

        [this->m_Context->context makeCurrentContext];

        GLContext::SetCurrentContext(this->m_Context->context);
        GLContext::SetCurrentGLContext(this);

        // TODO
        // GLOcclusionQuery::RecreateAllQueries();
        // GLFence::RecreateAllFences();
        // this->ToggleMTGL(mtglEnabled);

        GLDevice* device = GLDevice::Get();

        if (device && this->m_Contexts.size() > 1) {
            device->ApplyGLStates(device->m_States, 1);
            device->ApplyGLBindings(device->m_States, 1);
            device->m_DefaultVertexArrayObject.ApplyGLStates(device->m_DefaultVertexArrayObject.m_GLStates);
        }
    }
}

void GLContext::SetContextFormat(GLTextureFormat a2, uint32_t sampleCount) {
    System_Autorelease::ScopedPool autorelease;

    uint32_t v61 = sampleCount | (a2 << 8);

    if (this->m_Contexts.find(v61) != this->m_Contexts.end()) {
        this->m_Context = &this->m_Contexts[v61];

        if (this->m_Window) {
            this->m_Window->SetOpenGLContext(this);
        }
    } else {
        auto& context = this->m_Contexts[v61];

        CGDirectDisplayID v6 = CGMainDisplayID();

        NSOpenGLPixelFormatAttribute formatAttributes[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADepthSize,       0,
            NSOpenGLPFAStencilSize,     0,
            NSOpenGLPFAColorSize,       32,
            NSOpenGLPFAWindow,
            NSOpenGLPFAFullScreen,
            NSOpenGLPFAScreenMask,      CGDisplayIDToOpenGLDisplayMask(v6),
            0, 0, 0, 0, 0
        };

        switch (a2) {
            case GLTF_INVALID:
                break;

            case GLTF_D32:
                formatAttributes[4] = 32;
                break;

            case GLTF_D24:
                formatAttributes[4] = 24;
                break;

            case GLTF_D16:
                formatAttributes[4] = 16;
                break;

            case GLTF_D24S8:
                formatAttributes[4] = 24;
                formatAttributes[6] = 8;
                break;

            default:
                BLIZZARD_ASSERT(false);
                break;
        }

        if (sampleCount > 1) {
            BLIZZARD_ASSERT(sampleCount % 2 == 0);
            BLIZZARD_ASSERT(formatAttributes[13] == 0);
            BLIZZARD_ASSERT(formatAttributes[14] == 0);
            BLIZZARD_ASSERT(formatAttributes[15] == 0);
            BLIZZARD_ASSERT(formatAttributes[16] == 0);
            BLIZZARD_ASSERT(formatAttributes[17] == 0);

            formatAttributes[13] = NSOpenGLPFASampleBuffers;
            formatAttributes[14] = 1;
            formatAttributes[15] = NSOpenGLPFASamples;
            formatAttributes[16] = sampleCount;
        }

        context.sampleCount = sampleCount;

        context.pixelFormat = [[NSOpenGLPixelFormat alloc]
            initWithAttributes: formatAttributes
        ];

        BLIZZARD_ASSERT(context.pixelFormat != nullptr);

        context.context = [[NSOpenGLContext alloc]
            initWithFormat: context.pixelFormat
            shareContext: GLContext::s_MainContext
        ];

        BLIZZARD_ASSERT(context.context != nullptr);

        this->m_Context = &context;

        auto contextObj = [context.context CGLContextObj];

        int32_t vsyncEnabled = this->m_VSyncEnabled;
        CGLSetParameter(contextObj, kCGLCPSwapInterval, &vsyncEnabled);

        auto result = this->m_MTGLEnabled
            ? CGLEnable(contextObj, kCGLCEMPEngine)
            : CGLDisable(contextObj, kCGLCEMPEngine);
        BLIZZARD_ASSERT(result == kCGLNoError);

        if (this->m_Window) {
            this->m_Window->SetOpenGLContext(this);
        }
    }

    if (this->m_Context->context != GLContext::GetCurrentContext()) {
        this->MakeCurrent(0);
    }
}

void GLContext::SetFullscreenMode(uint32_t, uint32_t, uint32_t, bool) {
    // TODO
}

void GLContext::SetWindow(GLAbstractWindow* a2, bool a3) {
    if (!a2) {
        if (this->m_Window) {
            this->m_Window->SetOpenGLContext(nullptr);
        }

        this->m_Window = nullptr;

        return;
    }

    if (this->m_Windowed && a2 == this->m_Window) {
        if (a3) {
            a2->Show();
        }

        return;
    }

    NSView* v5;

    if (this->m_Window) {
        v5 = this->m_Window->GetNSView();

        if (v5 && this->m_Windowed && a2 != this->m_Window) {
            this->m_Window = a2;
            this->m_Window->SetOpenGLContext(this);

            return;
        }
    } else {
        v5 = nullptr;
    }

    this->m_Window = a2;

    if (!a2->GetNSView()) {
        this->m_Window->CreateView();
    }

    float v15;
    bool v16 = false;
    CGDisplayFadeReservationToken v17 = 0;

    if (v5 && GLDevice::GetRendererInfo().unk100 > 2639 && this->m_Window->CanEnterFullscreenMode()) {
        if (!this->m_Windowed) {
            if (!GLContext::s_DesktopMode) {
                // TODO
                // Blizzard::Debug::Assert(
                //     "s_DesktopMode",
                //     "/Users/Shared/BuildServer/wow2.old/work/WoW-code/branches/wow-patch-3_3_5-BNet/WoW/Source/Mac/../../../"
                //     "Engine/Source/Gx/CGxDeviceGLL/GLLayer/GLContext.cpp",
                //     215
                // );
            }

            v15 = 1.25;
            v16 = false;
            CGDisplayFadeReservationToken v8 = 0;

            CGError v7 = CGAcquireDisplayFadeReservation(2.25, &v17);

            if (!v7) {
                v8 = v17;
            }

            v17 = v8;

            if (v8) {
                v16 = CGDisplayFade(v8, 0.25, 0, 1.0, 0, 0, 0, 1) == kCGErrorSuccess;
            }

            CGDirectDisplayID v10 = CGMainDisplayID();
            CGDisplaySwitchToMode(v10, GLContext::s_DesktopMode);

            this->m_Window->ExitFullscreenMode();

            // TODO
            // SetSystemUIMode(0, 0);
        }

        CGReleaseAllDisplays();
    }

    if (a3) {
        this->m_Window->Show();
    }

    this->m_Windowed = 1;
    CGDisplayRestoreColorSyncSettings();

    if (v17) {
        if (v16) {
            float v6 = v15;

            if (v15 > 1.0) {
                usleep(1000);
                v6 = v15 - 1.0;
            }

            CGDisplayFade(v17, v6, 1.0, 0, 0, 0, 0, 0);
        }

        CGReleaseDisplayFadeReservation(v17);
    }

    return;
}

void GLContext::Swap() {
    [this->m_Context->context flushBuffer];
}

void GLContext::Update() {
    [this->m_Context->context update];
}
