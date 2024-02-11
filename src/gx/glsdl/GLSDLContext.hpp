#ifndef GX_GL_SDL_GL_SDL_CONTEXT_HPP

#include <SDL3/SDL.h>

#include "gx/glsdl/GLSDLWindow.hpp"
#include "gx/glsdl/GLTypes.hpp"

class GLSDLContext {
    public:
        SDL_GLContext m_sdlGLContext = nullptr;

        GLSDLContext() = default;
        void Create(GLSDLWindow* window);
        void Destroy();
        bool IsCurrentContext();
        void MakeCurrent(GLSDLWindow* window);
        int32_t GetSampleCount();
};

#endif
