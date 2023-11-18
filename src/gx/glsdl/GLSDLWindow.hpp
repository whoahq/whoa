#ifndef GX_GL_SDL_GL_SDL_WINDOW_HPP
#define GX_GL_SDL_GL_SDL_WINDOW_HPP

#include <cstdint>
#include <SDL3/SDL.h>

#include "gx/glsdl/GLTypes.hpp"

class GLSDLWindowRect {
    public:
        struct Point {
            int32_t x = 0;
            int32_t y = 0;
        };

        struct Size {
            int32_t width = 0;
            int32_t height = 0;
        };

        Point origin;
        Size  size;
};

class GLSDLWindow {
    public:
        SDL_Window*     m_sdlWindow    = nullptr;

        // Create an SDL window with the requested OpenGL attributes
        void Create(const char* title, const GLSDLWindowRect& rect, GLTextureFormat depthFormat, uint32_t sampleCount);
        void Destroy();
        void Swap();
        void Resize(const GLSDLWindowRect& rect);

        GLSDLWindowRect GetRect();
        GLSDLWindowRect GetBackingRect();

        int32_t GetWidth();
        int32_t GetHeight();


};

#endif
