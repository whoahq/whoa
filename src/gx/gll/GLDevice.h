#ifndef GX_GLL_GL_DEVICE_H
#define GX_GLL_GL_DEVICE_H

#include "gx/gll/GL.h"
#include "gx/gll/GLAbstractWindow.h"
#include "gx/gll/GLBatch.h"
#include "gx/gll/GLBufferPool.h"
#include "gx/gll/GLContext.h"
#include "gx/gll/GLDebugMipmap2D.h"
#include "gx/gll/GLFramebuffer.h"
#include "gx/gll/GLGLSLProgram.h"
#include "gx/gll/GLMipmap.h"
#include "gx/gll/GLShader.h"
#include "gx/gll/GLTexture.h"
#include "gx/gll/GLTypes.h"
#include "gx/gll/GLVertexArray.h"
#include "gx/gll/GLWorker.h"
#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include <bc/Thread.hpp>

class GLDevice {
    public:
        // Types
        enum GLDeviceOption {
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
        static std::vector<GLDevice *, std::allocator<GLDevice*>> m_Devices;
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
        static GLDevice* Get(void);
        static void Set(GLDevice*);
        static void InitPools(void);
        static RendererInfo GetRendererInfo(void);
        static void InitRendererInfo(void);
        static void SetOption(GLDeviceOption, bool);
        static void StaticInit(void);

        // Member variables
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> m_DebugName;
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
        GLContext m_Context;
        GLBufferPool* m_PBOPool = nullptr;
        GLWorker* m_TexWorker = nullptr;
        GLTexture* m_BoundTextures[4][16] = {};
        GLVertexArray m_DefaultVertexArrayObject;
        GLDirtyRange m_DirtyVertexShaderConsts;
        GLDirtyRange m_DirtyPixelShaderConsts;
        float m_ConstantDepthBias = 0.0f;
        float m_SlopeScaledDepthBias = 0.0f;
        bool m_Init = 0;
        uint32_t m_DrawCount = 0;
        uint32_t m_ID = -1;
        std::list<GLTexture*, std::allocator<GLTexture*>> m_TextureList;
        std::list<GLTexture*>::iterator m_OldestActiveTexture;
        uint32_t m_TextureTotalSize = 0;
        uint32_t m_FrameNumber = 1;
        std::list<GLDebugMipmap2D*, std::allocator<GLDebugMipmap2D*>> m_DebugMipmaps;
        bool m_BatchViewerEnabled = 0;
        bool m_UseWindowSystemBuffer = 0;
        bool m_FlippedSystemBuffer = 0;
        bool m_ShaderCompiler = 0;
        bool m_WorkerDevice;
        GLStates m_DefaultStates;
        std::vector<GLBatch, std::allocator<GLBatch>>* m_FrameBatches;
        bool m_CaptureOnlyOneFrame = 0;
        bool m_StopCapturingBatches = 0;
        bool m_CaptureBatches = 0;
        int32_t m_IndentLevel = 0;
        GLAbstractWindow* m_FirstCapturedWindow = nullptr;

        // Member functions
        GLDevice();
        void ApplyGLBindings(const GLStates&, bool);
        void ApplyGLStates(const GLStates&, bool);
        void ApplyShaderConstants(void);
        void ApplyTransforms(void);
        void BindBuffer(GLBuffer*, GLEnum);
        void BindFramebuffer(GLFramebuffer*);
        void BindGLSLProgram(GLGLSLProgram*);
        void BindShader(GLShader*);
        void BindTexture(GLEnum, GLTexture*);
        void BindVertexArray(GLVertexArray*);
        void BlitFramebuffer(GLMipmap*, const GLRect*, GLMipmap*, const GLRect*, GLEnum, GLEnum);
        void CheckDepthTarget(void);
        void Clear(uint32_t, const GLColor4f&, double, int32_t);
        void CopyTex(uint32_t, uint32_t, GLMipmap*, const GLRect*);
        GLBuffer* CreateBuffer(GLEnum, uint32_t, const void*, GLEnum, GLEnum);
        GLShader* CreateShader(GLShader::ShaderType, const void*, int32_t, const char*);
        GLTexture* CreateTexture2D(uint32_t, uint32_t, uint32_t, GLTextureFormat, uint32_t);
        GLTexture* CreateTextureCubeMap(uint32_t, uint32_t, GLTextureFormat, uint32_t);
        void Draw(GLEnum, uint32_t, uint32_t);
        void DrawIndexed(GLEnum, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
        void DrawRect(void);
        GLFramebuffer* GetCurrentTarget(void); // invented name
        uint32_t GetID(void);
        GLShader* GetShader(GLShader::ShaderType);
        const GLStates::VertexArrayObject& GetVertexArrayStates();
        void GLLDraw(GLEnum, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
        void Init(GLAbstractWindow*, const char*, uint32_t, GLTextureFormat);
        void LoadDefaultStates(void);
        void ResetBackbuffer(uint32_t width, uint32_t height, GLTextureFormat colorFormat, GLTextureFormat depthFormat, uint32_t sampleCount);
        void Resize(uint32_t width, uint32_t height);
        void RestoreTextures(void);
        void SetActiveTexture(uint32_t);
        void SetAlphaBlend(GLEnum, GLEnum, GLEnum);
        void SetAlphaBlendEnable(bool);
        void SetAlphaTest(GLEnum, float);
        void SetAlphaTestEnable(bool);
        void SetClearColor(const GLColor4f&);
        void SetClearDepth(double);
        void SetClearStencil(int32_t);
        void SetColorWriteMask(bool red, bool green, bool blue, bool alpha, uint32_t index);
        void SetCullMode(GLEnum);
        void SetDepthBias(float constantBias, float slopeScaledBias);
        void SetDepthTestEnable(bool);
        void SetDepthTestFunc(GLEnum);
        void SetDepthWriteMask(bool);
        void SetDisplay(uint32_t, uint32_t, GLTextureFormat, GLTextureFormat, uint32_t, bool, bool, uint32_t);
        void SetFogColor(float r, float g, float b, float a);
        void SetFogEnable(bool enable);
        void SetFogParam(GLEnum param, float value);
        void SetIndexBuffer(GLBuffer*);
        void SetLightingEnable(bool enable);
        void SetModelView(GLEnum view);
        void SetScissor(bool, const GLRect&);
        void SetShader(GLShader::ShaderType, GLShader*);
        void SetShaderConstants(GLShader::ShaderType, uint32_t, const float*, uint32_t);
        void SetShaderConstantsInternal(GLShader::ShaderType, uint32_t, const float*, uint32_t);
        void SetTexture(uint32_t, GLTexture*);
        void SetTransform(GLEnum, const float*);
        void SetUnpackClientStorage(bool);
        void SetVertexBuffer(uint32_t, GLBuffer*, uint32_t, uint32_t);
        void SetVertexFormat(GLVertexFormat*);
        void SetViewport(const GLRect&, double, double);
        void Sub34BB0(GLEnum a2, GLMipmap* a3, uint32_t index);
        void Sub38460(bool);
        void Swap(void);
        void UpdateFFPTexturing(void);
};

#endif
