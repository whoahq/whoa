#ifndef GX_GLL_GL_ABSTRACT_WINDOW_H
#define GX_GLL_GL_ABSTRACT_WINDOW_H

#ifdef __OBJC__
    #include <AppKit/AppKit.h>
#else
    #include <objc/runtime.h>
    typedef struct objc_object NSWindow;
    typedef struct objc_object NSView;
#endif

struct CGRect;
class GLContext;

class GLAbstractWindow {
    public:
        // Virtual member functions
        virtual int32_t GetWidth(void);
        virtual int32_t GetHeight(void);
        virtual CGRect GetRect(void) = 0;
        virtual void Show(void) = 0;
        virtual void Resize(uint32_t, uint32_t) = 0;
        virtual void SetTitle(const char*) = 0;
        virtual void CreateView(void) = 0;
        virtual void SetOpenGLContext(GLContext*) = 0;
        // virtual NSWindow* GetNSWindow(void) = 0;
        virtual NSView* GetNSView(void) = 0;
        virtual bool CanEnterFullscreenMode(void) = 0;
        // virtual void EnterFullscreenMode(uint32_t, bool) = 0;
        virtual void ExitFullscreenMode(void) = 0;
        virtual CGRect GetBackingRect() = 0;
        virtual int32_t GetBackingWidth();
        virtual int32_t GetBackingHeight();
};

#endif
