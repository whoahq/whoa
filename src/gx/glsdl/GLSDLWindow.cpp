#include "gx/glsdl/GLSDLWindow.hpp"

#include <bc/Debug.hpp>

static bool s_GLSDL_Initialized = false;

void GLSDLWindow::Create(const char* title, const GLSDLWindowRect& rect, GLTextureFormat depthFormat, uint32_t sampleCount) {
    BLIZZARD_ASSERT(this->m_sdlWindow == nullptr);

    if (!s_GLSDL_Initialized) {
        // Initialize SDL video context
        SDL_Init(SDL_INIT_VIDEO);

        // Set GL version profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        s_GLSDL_Initialized = true;
    }

    // Set depth and stencil size according to format
    uint32_t depthSize = 0;
    uint32_t stencilSize = 0;

    switch (depthFormat) {
    case GLTF_INVALID:
        break;
    case GLTF_D32:
        depthSize = 32;
        break;
    case GLTF_D24:
        depthSize = 24;
        break;
    case GLTF_D16:
        depthSize = 16;
        break;
    case GLTF_D24S8:
        depthSize = 24;
        stencilSize = 8;
        break;
    default:
        BLIZZARD_ASSERT(false);
        break;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthSize);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilSize);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Set multisampling
    if (sampleCount >= 1) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sampleCount);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    }

    this->m_sdlWindow = SDL_CreateWindow(
        title,
        static_cast<int>(rect.size.width), static_cast<int>(rect.size.height),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    BLIZZARD_ASSERT(this->m_sdlWindow != nullptr);
}

void GLSDLWindow::Swap() {
    SDL_GL_SwapWindow(this->m_sdlWindow);
}

void GLSDLWindow::Destroy() {
    SDL_DestroyWindow(this->m_sdlWindow);
    this->m_sdlWindow = nullptr;
}

GLSDLWindowRect GLSDLWindow::GetRect() {
    // Default rectangle
    GLSDLWindowRect rect;

    int origin_x = 0;
    int origin_y = 0;
    if (SDL_GetWindowPosition(this->m_sdlWindow, &origin_x, &origin_y) == 0) {
        rect.origin.x = static_cast<int32_t>(origin_x);
        rect.origin.y = static_cast<int32_t>(origin_y);
    }

    int width  = 0;
    int height = 0;
    if (SDL_GetWindowSize(this->m_sdlWindow, &width, &height) == 0) {
        rect.size.width  = static_cast<int32_t>(width);
        rect.size.height = static_cast<int32_t>(height);
    }

    return rect;
}

GLSDLWindowRect GLSDLWindow::GetBackingRect() {
    // Default rectangle
    GLSDLWindowRect rect;

    // Query backing width/height
    int width  = 0;
    int height = 0;
    if (SDL_GetWindowSizeInPixels(this->m_sdlWindow, &width, &height) == 0) {
        rect.size.width  = static_cast<int32_t>(width);
        rect.size.height = static_cast<int32_t>(height);
    }

    return rect;
}

void GLSDLWindow::Resize(const GLSDLWindowRect& rect) {
    auto current = this->GetBackingRect();

    if (current.size.width != rect.size.width || current.size.height != rect.size.width) {
        auto status = SDL_SetWindowSize(this->m_sdlWindow, rect.size.width, rect.size.height);
        BLIZZARD_ASSERT(status == 0);
    }
}

int32_t GLSDLWindow::GetWidth() {
    return this->GetBackingRect().size.width;
}

int32_t GLSDLWindow::GetHeight() {
    return this->GetBackingRect().size.height;
}

