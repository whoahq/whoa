#ifndef GX_GL_SDL_GL_SDL_DEVICE_HPP
#define GX_GL_SDL_GL_SDL_DEVICE_HPP

#include "gx/glsdl/GL.hpp"
#include "gx/glsdl/GLSDLWindow.hpp"
#include "gx/glsdl/GLBatch.hpp"
#include "gx/glsdl/GLBufferPool.hpp"
#include "gx/glsdl/GLSDLContext.hpp"
#include "gx/glsdl/GLDebugMipmap2D.hpp"
#include "gx/glsdl/GLFramebuffer.hpp"
#include "gx/glsdl/GLGLSLProgram.hpp"
#include "gx/glsdl/GLMipmap.hpp"
#include "gx/glsdl/GLShader.hpp"
#include "gx/glsdl/GLTexture.hpp"
#include "gx/glsdl/GLTypes.hpp"
#include "gx/glsdl/GLVertexArray.hpp"
#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include <bc/Thread.hpp>

class GLSDLDevice {
    public:
        // Types
        enum GLSDLDeviceOption {
            eUseMTGL                = 0,
            eUseVertexArray         = 1,
            eUseGLSL                = 2,
            eCheckGLStates          = 3,
            eFlushBeforeDraw        = 4,
            eDeviceOption5          = 5,
            eUseHybridShader        = 6,
            eDeviceOption7          = 7,
            eDeviceOption8          = 8,
            eShaderConstantBindings = 9
        };

        struct RendererInfo {
            uint8_t init = 0;
            uint32_t vendor_id;
            uint32_t renderer_id;
            uint32_t max_color_attachments;
            uint32_t unk36;     // max clip planes
            uint32_t unk100;
        };

        // Static variables
        static Blizzard::Thread::TLSSlot m_CurrentDevice;
        static std::vector<GLSDLDevice*> m_Devices;
        static bool m_ExtARBShadow;
        static bool m_ExtColorMaskIndexed;
        static RendererInfo m_RendererInfo;
        static bool m_ShaderConstantBindings;
        static int32_t m_StaticResourcesRefCount;
        static bool m_UseHybridShader;
        static GLBuffer* m_BlitQuadVBO;
        static GLShader* m_DeviceShaders[];
        static GLTexture* m_DeviceTextures[];
        static GLVertexFormat m_NormalBlitVF;
        static GLVertexFormat m_InvertedBlitVF;
        static GLFramebuffer* m_F8330C;

        // Static functions
        static GLSDLDevice* Get();
        static void Set(GLSDLDevice* device);
        static void InitPools();
        static RendererInfo GetRendererInfo();
        static void InitRendererInfo();
        static void SetOption(GLSDLDeviceOption option, bool enable);
        static void StaticInit();

        // Member variables
        std::string m_DebugName;
        GLStates m_States;
        GLTexture* m_Textures[16] = {};
        GLShader* m_PixelShader = nullptr;
        GLShader* m_VertexShader = nullptr;
        GLGLSLProgram* m_GLSLProgram = nullptr;
        GLVertexArray* m_VertexArrayObject = &m_DefaultVertexArrayObject;
        GLFramebuffer* m_SystemTarget = nullptr;
        GLFramebuffer* m_FBOTarget = nullptr;
        GLFramebuffer* m_CurrentTarget = nullptr;
        GLMipmap* m_CurrentTargetColor[4] = {};
        GLMipmap* m_CurrentTargetDepth = nullptr;
        GLMipmap* m_CurrentTargetStencil = nullptr;
        GLMipmap* m_CurrentDepthBuffer = nullptr;
        GLTexture2D* m_BackBufferColor = nullptr;
        GLTexture2D* m_BackBufferDepth = nullptr;
        GLTexture2D* m_BackBufferStencil = nullptr;
        GLSDLWindow* m_Window = nullptr;
        GLBufferPool* m_PBOPool = nullptr;
        GLSDLContext m_Context;
        GLTexture* m_BoundTextures[4][16] = {};
        GLVertexArray m_DefaultVertexArrayObject;
        GLDirtyRange m_DirtyVertexShaderConsts;
        GLDirtyRange m_DirtyPixelShaderConsts;
        float m_ConstantDepthBias = 0.0f;
        float m_SlopeScaledDepthBias = 0.0f;
        bool m_Init = 0;
        uint32_t m_DrawCount = 0;
        uint32_t m_ID = -1;
        std::list<GLTexture*> m_TextureList;
        std::list<GLTexture*>::iterator m_OldestActiveTexture;
        uint32_t m_TextureTotalSize = 0;
        uint32_t m_FrameNumber = 1;
        std::list<GLDebugMipmap2D*> m_DebugMipmaps;
        bool m_BatchViewerEnabled = 0;
        bool m_UseWindowSystemBuffer = 0;
        bool m_FlippedSystemBuffer = 0;
        bool m_ShaderCompiler = 0;
        bool m_WorkerDevice;
        GLStates m_DefaultStates;
        std::vector<GLBatch>* m_FrameBatches;
        bool m_CaptureOnlyOneFrame = 0;
        bool m_StopCapturingBatches = 0;
        bool m_CaptureBatches = 0;
        int32_t m_IndentLevel = 0;

        // Member functions
        GLSDLDevice();
        void ApplyGLBindings(const GLStates& states, bool a3);
        void ApplyGLStates(const GLStates& states, bool force);
        void ApplyShaderConstants();
        void ApplyTransforms();
        void BindBuffer(GLBuffer* buffer, GLEnum target);
        void BindFramebuffer(GLFramebuffer* framebuffer);
        void BindGLSLProgram(GLGLSLProgram* a2);
        void BindShader(GLShader* shader);
        void BindTexture(GLEnum textureType, GLTexture* texture);
        void BindVertexArray(GLVertexArray* a2);
        void BlitFramebuffer(GLMipmap* src, const GLRect* srcRect, GLMipmap* dst, const GLRect* dstRect, GLEnum mask, GLEnum filter);
        void CheckDepthTarget();
        void Clear(uint32_t clearMask, const GLColor4f& clearColor, double clearDepth, int32_t clearStencil);
        void CopyTex(uint32_t a2, uint32_t a3, GLMipmap* dst, const GLRect* framebufferRect);
        GLBuffer* CreateBuffer(GLEnum type, uint32_t a3, const void* a4, GLEnum usage, GLEnum format);
        GLShader* CreateShader(GLShader::ShaderType type, const void* buf, int32_t codeLen, const char* name);
        GLTexture* CreateTexture2D(uint32_t width, uint32_t height, uint32_t numMipMap, GLTextureFormat format, uint32_t flags);
        GLTexture* CreateTextureCubeMap(uint32_t size, uint32_t numMipMap, GLTextureFormat format, uint32_t flags);
        void Draw(GLEnum primitive, uint32_t a3, uint32_t a4);
        void DrawIndexed(GLEnum primitive, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t count);
        void DrawRect();
        GLFramebuffer* GetCurrentTarget(); // invented name
        uint32_t GetID();
        GLShader* GetShader(GLShader::ShaderType shaderType);
        const GLStates::VertexArrayObject& GetVertexArrayStates();
        void GLSDLDraw(GLEnum mode, uint32_t start, uint32_t end, uint32_t a5, uint32_t a6, uint32_t count);
        void Init(GLSDLWindow* a2, const char* a3, uint32_t a4);
        void LoadDefaultStates();
        void ResetBackbuffer(uint32_t width, uint32_t height, GLTextureFormat colorFormat, GLTextureFormat depthFormat,uint32_t sampleCount);
        void Resize(uint32_t width, uint32_t height);
        void RestoreTextures();
        void SetActiveTexture(uint32_t a2);
        void SetAlphaBlend(GLEnum srcBlend, GLEnum dstBlend, GLEnum blendOp);
        void SetAlphaBlendEnable(bool enable);
        void SetAlphaTest(GLEnum func, float ref);
        void SetAlphaTestEnable(bool enable);
        void SetClearColor(const GLColor4f& clearColor);
        void SetClearDepth(double clearDepth);
        void SetClearStencil(int32_t clearStencil);
        void SetColorWriteMask(bool red, bool green, bool blue, bool alpha, uint32_t index);
        void SetCullMode(GLEnum cullMode);
        void SetDepthBias(float constantBias, float slopeScaledBias);
        void SetDepthTestEnable(bool enable);
        void SetDepthTestFunc(GLEnum func);
        void SetDepthWriteMask(bool enable);
        void SetDisplay(uint32_t width, uint32_t height, GLTextureFormat colorFormat, GLTextureFormat depthFormat, uint32_t refreshRate, bool windowed, bool captureDisplay, uint32_t sampleCount);
        void SetFogColor(float r, float g, float b, float a);
        void SetFogEnable(bool enable);
        void SetFogParam(GLEnum param, float value);
        void SetIndexBuffer(GLBuffer* buffer);
        void SetLightingEnable(bool enable);
        void SetModelView(GLEnum transform);
        void SetScissor(bool a2, const GLRect& a3);
        void SetShader(GLShader::ShaderType shaderType, GLShader* shader);
        void SetShaderConstants(GLShader::ShaderType shaderType, uint32_t index, const float* constants, uint32_t count);
        void SetShaderConstantsInternal(GLShader::ShaderType shaderType, uint32_t index, const float* constants, uint32_t count);
        void SetTexture(uint32_t stage, GLTexture* texture);
        void SetTransform(GLEnum transform, const float* a3);
        void SetUnpackClientStorage(bool enable);
        void SetVertexBuffer(uint32_t index, GLBuffer* buffer, uint32_t offset, uint32_t stride);
        void SetVertexFormat(GLVertexFormat* format);
        void SetViewport(const GLRect& viewport, double zNear, double zFar);
        void Sub34BB0(GLEnum a2, GLMipmap* a3, uint32_t index);
        void Sub38460(bool a2);
        void Swap();
        void UpdateFFPTexturing();
};

#endif
