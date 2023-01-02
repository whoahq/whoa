#ifndef GX_GLL_C_GX_DEVICE_GLL_HPP
#define GX_GLL_C_GX_DEVICE_GLL_HPP

#include "gx/CGxDevice.hpp"
#include "gx/gll/GLDevice.h"
#include "gx/gll/GLWindow.h"
#include <ApplicationServices/ApplicationServices.h>

class CGxBatch;
class CGxShader;

class CGxDeviceGLL : public CGxDevice {
    public:
        // Static variables
        static GLEnum s_glCubeMapFaces[];
        static GLEnum s_glDstBlend[];
        static GLEnum s_glSrcBlend[];
        static GLTextureFormat s_gxTexFmtToGLLFmt[];
        static GLEnum s_poolTarget2BufferFormat[];
        static GLEnum s_poolTarget2BufferType[];
        static GLEnum s_poolUsage2BufferUsage[];
        static GLEnum s_primitiveConversion[];

        // Member variables
        GLDevice m_glDevice;
        GLWindow m_glWindow;
        GLVertexFormat m_glFormats[GxVertexBufferFormats_Last] = {};

        // Virtual member functions
        virtual void ITexMarkAsUpdated(CGxTex*);
        virtual void IRsSendToHw(EGxRenderState);
        virtual int32_t DeviceCreate(long (*)(void*, uint32_t, uint32_t, long), const CGxFormat&);
        virtual int32_t DeviceSetFormat(const CGxFormat&);
        virtual void CapsWindowSize(CRect&);
        virtual void CapsWindowSizeInScreenCoords(CRect& dst);
        virtual void ScenePresent(void);
        virtual void SceneClear(uint32_t, CImVector);
        virtual void XformSetProjection(const C44Matrix&);
        virtual void XformSetView(const C44Matrix&);
        virtual void Draw(CGxBatch*, int32_t);
        virtual void PoolSizeSet(CGxPool*, uint32_t);
        virtual char* BufLock(CGxBuf*);
        virtual int32_t BufUnlock(CGxBuf*, uint32_t);
        virtual void TexDestroy(CGxTex* texId);
        virtual void IShaderCreate(CGxShader*);
        virtual void ShaderCreate(CGxShader*[], EGxShTarget, const char*, const char*, int32_t);
        virtual int32_t StereoEnabled(void);

        // Member functions
        CGxDeviceGLL();
        char* IBufLock(CGxBuf*);
        int32_t IBufUnlock(CGxBuf*);
        void ISceneBegin();
        void ISetCaps(void);
        void IShaderBindPixel(CGxShader*);
        void IShaderBindVertex(CGxShader*);
        void IShaderConstantsFlush(void);
        void IShaderCreatePixel(CGxShader*);
        void IShaderCreateVertex(CGxShader*);
        void IStateSetGLLDefaults();
        void IStateSync(void);
        void IStateSyncEnables(void);
        void IStateSyncIndexPtr(void);
        void IStateSyncLights(void);
        void IStateSyncMaterial(void);
        void IStateSyncScissorRect(void);
        void IStateSyncVertexPtrs(void);
        void IStateSyncXforms(void);
        void ITexCreate(CGxTex*);
        void ITexSetFlags(CGxTex*);
        void ITexUpload(CGxTex*);
        void IXformSetProjection(const C44Matrix&);
        void IXformSetView(const C44Matrix&);
        void IXformSetViewport(void);
        void PatchPixelShader(CGxShader*);
        void PatchVertexShader(CGxShader*);
        void Resize(uint32_t width, uint32_t height);
};

#endif
