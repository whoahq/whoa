#ifndef GX_GLL_GL_WINDOW_H
#define GX_GLL_GL_WINDOW_H

#include "gx/gll/GLAbstractWindow.h"
#include <ApplicationServices/ApplicationServices.h>

#ifdef __OBJC__
    #include <AppKit/AppKit.h>

    @class GLLayerView;
    @class GLFullscreenWindow;
    @class NSEvent;
#else
    #include <objc/objc-runtime.h>

    typedef struct objc_object GLLayerView;
    typedef struct objc_object GLFullscreenWindow;
    typedef struct objc_object NSView;
    typedef struct objc_object NSWindow;
    typedef struct objc_object NSEvent;
#endif

class GLContext;

struct GLWindowCallbacks {
    void (*OnResized)(int32_t, int32_t, bool);
    void (*OnMouseDown)(int16_t, int32_t, int32_t);
    void (*OnMouseMoved)(int32_t, int32_t);
    void (*OnMouseUp)(int16_t, int32_t, int32_t);
    void (*OnKeyDown)(NSEvent*);
    void (*OnKeyUp)(NSEvent*);
};

class GLWindow : public GLAbstractWindow {
    public:
        // Member variables
        // void** var0; // _vptr$GLAbstractWindow
        GLContext* m_Context = nullptr;
        NSWindow* m_Window = nullptr;
        bool m_Shown = 0;
        GLLayerView* m_View = nullptr;
        Class m_ViewClass;
        GLFullscreenWindow* m_FullscreenWindow = nullptr;
        GLWindowCallbacks* m_Callbacks;
        GLWindowCallbacks* m_ActiveCallbacks;

        // Member functions
        bool CanEnterFullscreenMode(void);
        void CreateView(void);
        CGRect GetBackingRect();
        NSView* GetNSView(void);
        CGRect GetRect(void);
        void ExitFullscreenMode(void);
        void Init(const CGRect&, GLWindowCallbacks*);
        void Resize(uint32_t, uint32_t);
        void SetOpenGLContext(GLContext*);
        void SetViewClass(Class);
        void Show(void);
        void Sub70760(void);

        // Virtual member functions
        virtual void SetCallbacks(GLWindowCallbacks*);
        virtual void SetTitle(const char*);
};

#endif
