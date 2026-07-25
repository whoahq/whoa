#ifndef GX_MTL_C_GX_DEVICE_MTL_HPP
#define GX_MTL_C_GX_DEVICE_MTL_HPP

#include "gx/CGxDevice.hpp"
#include "gx/gll/GLWindow.h"

class CGxBatch;
class CGxShader;

class CGxDeviceMTL : public CGxDevice {
    public:
        // Member variables
        GLWindow m_window;

        // Virtual member functions
        void ITexMarkAsUpdated(CGxTex*) override;
        void IRsSendToHw(EGxRenderState) override;
        int32_t DeviceCreate(int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat&) override;
        int32_t DeviceSetFormat(const CGxFormat&) override;
        void* DeviceWindow() override;
        void DeviceWM(EGxWM wm, uintptr_t param1, uintptr_t param2) override {};
        void CapsWindowSize(CRect&) override;
        void CapsWindowSizeInScreenCoords(CRect& dst) override;
        void ScenePresent() override;
        void SceneClear(uint32_t, CImVector) override;
        void Draw(CGxBatch* batch, int32_t indexed) override;
        void PoolSizeSet(CGxPool*, uint32_t) override;
        char* BufLock(CGxBuf*) override;
        int32_t BufUnlock(CGxBuf*, uint32_t) override;
        void BufData(CGxBuf* buf, const void* data, size_t size, uintptr_t offset) override;
        void TexDestroy(CGxTex* texId) override;
        void IShaderCreate(CGxShader*) override;
        void ShaderCreate(CGxShader*[], EGxShTarget, const char*, const char*, int32_t) override;
        int32_t StereoEnabled(void) override;
        void XformSetProjection(const C44Matrix& matrix) override;

        // Member functions
        CGxDeviceMTL();
        void Resize(uint32_t width, uint32_t height);

    private:
        void ISetCaps(const CGxFormat& format);
        void EnsureLibrary();
        void BeginFrame();
        void* GetPipeline(EGxVertexBufferFormat format, bool useColor, bool useSkin, bool useTex, int32_t blendMode);
        void* GetPoolBuffer(CGxPool* pool);
        void ITexCreate(CGxTex* texId);
        void ITexUpload(CGxTex* texId);
        void* GetTexture(CGxTex* texId);
        void* GetSampler(CGxTex* texId);
        void EnsureFallbackTexture();
        void EnsureDepthTexture(uint32_t width, uint32_t height);
        void* GetDepthState(bool depthTest, bool depthWrite, uint32_t depthFunc);
        void* m_device = nullptr;
        void* m_commandQueue = nullptr;
        void* m_layer = nullptr;
        void* m_shaderLibrary = nullptr;
        void* m_pipelineColor[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSolid[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSkin[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineColorTex[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSolidTex[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSkinTex[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineColorTex2[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSolidTex2[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_pipelineSkinTex2[GxVertexBufferFormats_Last][GxBlends_Last] = {};
        void* m_frameCommandBuffer = nullptr;
        void* m_frameEncoder = nullptr;
        void* m_frameDrawable = nullptr;
        uint32_t m_frameHasDraw = 0;
        uint32_t m_clearMask = 0;
        uint32_t m_clearColor = 0;
        void* m_fallbackTexture = nullptr;
        void* m_fallbackSampler = nullptr;
        void* m_depthTexture = nullptr;
        uint32_t m_depthWidth = 0;
        uint32_t m_depthHeight = 0;
        void* m_depthStates[2][2][4] = {};
};

#endif
