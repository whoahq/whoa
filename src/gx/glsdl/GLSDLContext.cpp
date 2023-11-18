#include "gx/glsdl/GLSDLContext.hpp"
#include <bc/Debug.hpp>
#include <storm/Error.hpp>

static bool s_GLEW_Initialized = false;

void GLSDLContext::Create(GLSDLWindow* window) {
    BLIZZARD_ASSERT(this->m_sdlGLContext == nullptr);

    this->m_sdlGLContext = SDL_GL_CreateContext(window->m_sdlWindow);

    BLIZZARD_ASSERT(this->m_sdlGLContext != nullptr);

    if (s_GLEW_Initialized == false) {
        glewExperimental = true;

        auto glewError = glewInit();

        if (glewError != GLEW_OK) {
            SErrDisplayAppFatal("Error initializing GLEW: %s\n", glewGetErrorString(glewError));
        }

        s_GLEW_Initialized = true;
    }
}

void GLSDLContext::Destroy() {
    BLIZZARD_ASSERT(this->m_sdlGLContext != nullptr);

    SDL_GL_DeleteContext(this->m_sdlGLContext);
    this->m_sdlGLContext = nullptr;
}

bool GLSDLContext::IsCurrentContext() {
    return this->m_sdlGLContext == SDL_GL_GetCurrentContext();
}

void GLSDLContext::MakeCurrent(GLSDLWindow* window) {
    auto status = SDL_GL_MakeCurrent(window->m_sdlWindow, this->m_sdlGLContext);
    BLIZZARD_ASSERT(status == 0);
}

int32_t GLSDLContext::GetSampleCount() {
    int samples;
    auto status = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &samples);
    BLIZZARD_ASSERT(status == 0);
    return static_cast<int32_t>(samples);
}
