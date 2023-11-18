#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLPool.hpp"
#include "gx/glsdl/GLUtil.hpp"
#include "gx/glsdl/GL.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <type_traits>
#include <bc/Debug.hpp>

#define GL_MAX_STREAM 4

class GLPixelShader;
class GLVertexShader;

Blizzard::Thread::TLSSlot GLSDLDevice::m_CurrentDevice;
std::vector<GLSDLDevice *, std::allocator<GLSDLDevice*>> GLSDLDevice::m_Devices;
bool GLSDLDevice::m_ExtColorMaskIndexed = false;
int32_t GLSDLDevice::m_StaticResourcesRefCount = 0;
GLSDLDevice::RendererInfo GLSDLDevice::m_RendererInfo;
bool GLSDLDevice::m_UseHybridShader = 0;
bool GLSDLDevice::m_ExtARBShadow = 0;
bool GLSDLDevice::m_ShaderConstantBindings = 1;
GLBuffer* GLSDLDevice::m_BlitQuadVBO = nullptr;
GLShader* GLSDLDevice::m_DeviceShaders[11] = {};
GLTexture* GLSDLDevice::m_DeviceTextures[4] = {};
GLVertexFormat GLSDLDevice::m_NormalBlitVF;
GLVertexFormat GLSDLDevice::m_InvertedBlitVF;
GLFramebuffer* GLSDLDevice::m_F8330C = nullptr;

const char* GLSDLBlitVsCode = R"(!!ARBvp1.0
PARAM c0 = { 1.0 };
MOV result.position.xyz, vertex.attrib[0];
MOV result.position.w, c0.x;
MOV result.texcoord[0].xyz, vertex.attrib[1];
END)";

const char* GLSDLDummyVsCode = R"(!!ARBvp1.0
PARAM c = {0.0, 0.0, 0.0, 1.0};
MOV result.position, c;
END)";

const char* GLSDLBlitPsCode = R"(!!ARBfp1.0
PARAM c0 = { 1.0 };
TEX result.color.rgba, fragment.texcoord[0], texture[0], 2D;
END)";

inline void COPY_TRANSFORM(GLTransform& dst, const GLTransform& src) {
    if (src.isIdentity) {
        dst.isIdentity = true;
        dst.isDirty = true;
    } else {
        dst.isIdentity = false;
        dst.isDirty = true;
        memcpy(dst.m, src.m, sizeof(float) * 16);
    }
}

void* Sub1D210(void* ptr) {
    GLSDLDevice** ptrptr = new GLSDLDevice*;
    *ptrptr = nullptr;
    return ptrptr;
}

void Sub1D230(void* ptr) {
    delete static_cast<GLSDLDevice**>(ptr);
}

GLSDLDevice* GLSDLDevice::Get() {
    return *static_cast<GLSDLDevice**>(
        Blizzard::Thread::RegisterLocalStorage(&GLSDLDevice::m_CurrentDevice, Sub1D210, 0, Sub1D230)
    );
}

void GLSDLDevice::Set(GLSDLDevice* device) {
    *static_cast<GLSDLDevice**>(
        Blizzard::Thread::RegisterLocalStorage(&GLSDLDevice::m_CurrentDevice, Sub1D210, 0, Sub1D230)
    ) = device;
}

GLSDLDevice::RendererInfo GLSDLDevice::GetRendererInfo() {
    if (!GLSDLDevice::m_RendererInfo.init) {
        GLSDLDevice::InitRendererInfo();
    }

    return GLSDLDevice::m_RendererInfo;
}

void GLSDLDevice::InitRendererInfo() {
    // TODO

    GLSDLDevice::m_RendererInfo.init = 1;
}

void GLSDLDevice::InitPools() {
    // TODO
    // - init all pools

    GLPool<GLBuffer>::Init();
    GLPool<GLBuffer>::Get()->SetNextName(1);

    GLPool<GLTexture2D>::Init();
    GLPool<GLTexture2D>::Get()->SetNextName(0x21);

    GLPool<GLVertexShader>::Init();
    GLPool<GLVertexShader>::Get()->SetNextName(0x1);

    GLPool<GLPixelShader>::Init();
    GLPool<GLPixelShader>::Get()->SetNextName(0x2001);

    GLPool<GLFramebuffer>::Init();
    GLPool<GLFramebuffer>::Get()->SetNextName(0x1);
}

void GLSDLDevice::SetOption(GLSDLDeviceOption option, bool enable) {
    switch (option) {
        case eUseMTGL: {
            break;
        }

        case eUseVertexArray: {
            break;
        }

        case eUseGLSL: {
            break;
        }

        case eCheckGLStates: {
            break;
        }

        case eFlushBeforeDraw: {
            break;
        }

        case eDeviceOption5: {
            break;
        }

        case eUseHybridShader: {
            break;
        }

        case eDeviceOption7: {
            break;
        }

        case eDeviceOption8: {
            break;
        }

        case eShaderConstantBindings: {
            GLSDLDevice::m_ShaderConstantBindings = enable;
            break;
        }

        default: {
            BLIZZARD_ASSERT(false);
        }
    }
}

void GLSDLDevice::StaticInit() {
    static float blitQuad[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };

    GLSDLDevice::m_BlitQuadVBO = GLBuffer::Create(
        GL_ARRAY_BUFFER,
        sizeof(blitQuad),
        blitQuad,
        GL_STATIC_DRAW,
        0
    );

    GLSDLDevice::m_InvertedBlitVF.m_Attribs[0] = { 0, 0, GLVT_FLOAT3, 0 };
    GLSDLDevice::m_InvertedBlitVF.m_Attribs[1] = { 0, 1, GLVT_FLOAT2, 12 };
    GLSDLDevice::m_InvertedBlitVF.m_Size = 2;

    GLSDLDevice::m_NormalBlitVF.m_Attribs[0] = { 0, 0, GLVT_FLOAT3, 0 };
    GLSDLDevice::m_NormalBlitVF.m_Attribs[1] = { 0, 1, GLVT_FLOAT2, 20 };
    GLSDLDevice::m_NormalBlitVF.m_Size = 2;

    GLSDLDevice::m_F8330C = GLFramebuffer::Create(0);

    GLSDLDevice::m_DeviceShaders[0] = GLShader::Create(
        GLShader::eVertexShader,
        GLSDLDevice::m_UseHybridShader,
        false,
        "",
        GLSDLBlitVsCode,
        strlen(GLSDLBlitVsCode),
        "",
        "GL SDL Blit VS",
        nullptr
    );

    GLSDLDevice::m_DeviceShaders[0]->Compile(nullptr);

    GLSDLDevice::m_DeviceShaders[1] = GLShader::Create(
        GLShader::eVertexShader,
        GLSDLDevice::m_UseHybridShader,
        false,
        "",
        GLSDLDummyVsCode,
        strlen(GLSDLDummyVsCode),
        "",
        "GL SDL Dummy VS",
        nullptr
    );

    GLSDLDevice::m_DeviceShaders[1]->Compile(nullptr);

    GLSDLDevice::m_DeviceShaders[2] = GLShader::Create(
        GLShader::ePixelShader,
        GLSDLDevice::m_UseHybridShader,
        false,
        "",
        GLSDLBlitPsCode,
        strlen(GLSDLBlitPsCode),
        "",
        "GL SDL Blit PS",
        nullptr
    );

    GLSDLDevice::m_DeviceShaders[2]->Compile(nullptr);

    // TODO
    // - remaining device shaders

    // Device textures

    uint32_t v30 = 0;
    uint32_t v31 = 0;

    auto texture0 = GLTexture2D::Create(1, 1, 1, GLTF_RGBA8888, 0x0);
    *static_cast<char*>(texture0->Map(0, nullptr, v30, GL_WRITE_ONLY)) = 0xFF;
    texture0->Unmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    GLSDLDevice::m_DeviceTextures[0] = texture0;

    // TODO
    // auto texture1 = GLTexture3D::Create(1, 1, 1, 1, GLTF_RGBA8888, 0x0);
    // *texture1->Map(0, nullptr, v30, v31, GL_WRITE_ONLY) = 0xFF;
    // texture1->Unmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    // GLSDLDevice::m_DeviceTextures[1] = texture1;

    // TODO texture2

    auto texture3 = GLTexture2D::Create(1, 1, 1, GLTF_D32, 0x0);
    *static_cast<char*>(texture3->Map(0, nullptr, v30, GL_WRITE_ONLY)) = 0xFF;
    texture3->Unmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    GLSDLDevice::m_DeviceTextures[3] = texture3;
}

GLSDLDevice::GLSDLDevice() : m_Context(), m_DefaultVertexArrayObject(true) {
    // TODO
    // - fill in remaining initializers
}

void GLSDLDevice::ApplyGLBindings(const GLStates& states, bool a3) {
    static GLenum texTarget[4] = {
        GL_TEXTURE_2D,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_RECTANGLE
    };

    for (int32_t i = 0; i < 16; ++i) {
        for (int32_t t = 0; t < 4; ++t) {
            if (this->m_States.binding.texture[t][i] != states.binding.texture[t][i] || a3) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(texTarget[t], states.binding.texture[t][i]);
            }
        }
    }

    glActiveTexture(GL_TEXTURE0 + states.binding.currentActiveTexture);

    if (this->m_States.binding.framebuffer != states.binding.framebuffer || a3) {
        glBindFramebufferEXT(GL_FRAMEBUFFER, states.binding.framebuffer);
    }

    if (this->m_States.binding.renderbuffer != states.binding.renderbuffer || a3) {
        glBindRenderbufferEXT(GL_RENDERBUFFER, states.binding.renderbuffer);
    }

    if (this->m_States.binding.vertexArrayObject != states.binding.vertexArrayObject || a3) {
        glBindVertexArray(states.binding.vertexArrayObject);
    }

    if (this->m_States.binding.vertexProgram != states.binding.vertexProgram || a3) {
        glBindProgramARB(GL_VERTEX_PROGRAM_ARB, states.binding.vertexProgram);
    }

    if (this->m_States.binding.pixelProgram != states.binding.pixelProgram || a3) {
        glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, states.binding.pixelProgram);
    }

    if (this->m_States.binding.glslProgram != states.binding.glslProgram || a3) {
        glUseProgram(states.binding.glslProgram);
    }

    memcpy(&this->m_States.binding, &states.binding, sizeof(this->m_States.binding));
}

void GLSDLDevice::ApplyGLStates(const GLStates& states, bool force) {
    if (force) {
        for (int32_t i = 0; i < 8; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            float sPlane[] = { 1.0, 0.0, 0.0, 0.0 };
            float tPlane[] = { 0.0, 1.0, 0.0, 0.0 };
            float rPlane[] = { 0.0, 0.0, 1.0, 0.0 };
            float qPlane[] = { 0.0, 0.0, 0.0, 1.0 };

            glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
            glTexGenfv(GL_T, GL_EYE_PLANE, tPlane);
            glTexGenfv(GL_R, GL_EYE_PLANE, rPlane);
            glTexGenfv(GL_Q, GL_EYE_PLANE, qPlane);
            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
            glTexGenfv(GL_R, GL_OBJECT_PLANE, rPlane);
            glTexGenfv(GL_Q, GL_OBJECT_PLANE, qPlane);

            glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, 1);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (GLfloat*)&GLColor4f::WHITE);
        }

        glActiveTexture(GL_TEXTURE0 + states.binding.currentActiveTexture);
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR_EXT);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }

    if (this->m_States.depth.testEnable != states.depth.testEnable || force) {
        if (states.depth.testEnable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    if (this->m_States.depth.compareFunc != states.depth.compareFunc || force) {
        glDepthFunc(states.depth.compareFunc);
    }

    if (this->m_States.depth.writeMask != states.depth.writeMask || force) {
        glDepthMask(states.depth.writeMask);
    }

    if (this->m_States.stencil.testEnable != states.stencil.testEnable || force) {
        if (states.stencil.testEnable) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    if (this->m_States.stencil.front.compareFunc != states.stencil.front.compareFunc
        || this->m_States.stencil.back.compareFunc != states.stencil.back.compareFunc
        || this->m_States.stencil.ref != states.stencil.ref
        || this->m_States.stencil.mask != states.stencil.mask
        || force)
    {
        glStencilFuncSeparate(
            states.stencil.front.compareFunc,
            states.stencil.back.compareFunc,
            states.stencil.ref,
            states.stencil.mask
        );
    }

    if (this->m_States.stencil.front.opFail != states.stencil.front.opFail
        || this->m_States.stencil.front.opZFail != states.stencil.front.opZFail
        || this->m_States.stencil.front.opZPass != states.stencil.front.opZPass
        || force)
    {
        glStencilOpSeparate(
            states.stencil.useTwoSidedStencil ? GL_FRONT : GL_FRONT_AND_BACK,
            states.stencil.front.opFail,
            states.stencil.front.opZFail,
            states.stencil.front.opZPass
        );
    }

    if (states.stencil.useTwoSidedStencil) {
        if (this->m_States.stencil.back.opFail != states.stencil.back.opFail
            || this->m_States.stencil.back.opZFail != states.stencil.back.opZFail
            || this->m_States.stencil.back.opZPass != states.stencil.back.opZPass
            || force)
        {
            glStencilOpSeparate(
                GL_BACK,
                states.stencil.back.opFail,
                states.stencil.back.opZFail,
                states.stencil.back.opZPass
            );
        }
    }

    if (this->m_States.stencil.writeMask != states.stencil.writeMask || force) {
        glStencilMask(states.stencil.writeMask);
    }

    if (this->m_States.rasterizer.cullFaceMode != states.rasterizer.cullFaceMode || force) {
        glCullFace(states.rasterizer.cullFaceMode);
    }

    if (this->m_States.rasterizer.cullMode != states.rasterizer.cullMode || force) {
        if (states.rasterizer.cullMode != 0) {
            glEnable(GL_CULL_FACE);
            glFrontFace(states.rasterizer.cullMode);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    if (this->m_States.rasterizer.fillMode != states.rasterizer.fillMode || force) {
        glPolygonMode(GL_FRONT_AND_BACK, states.rasterizer.fillMode);
    }

    if (this->m_CurrentTargetDepth
        && (this->m_States.rasterizer.constantDepthBias != states.rasterizer.constantDepthBias
            || this->m_States.rasterizer.slopeScaledDepthBias != states.rasterizer.slopeScaledDepthBias
            || force))
    {
        if (states.rasterizer.slopeScaledDepthBias == 0.0 && states.rasterizer.constantDepthBias == 0.0) {
            glDisable(GL_POLYGON_OFFSET_FILL);
        } else {
            glEnable(GL_POLYGON_OFFSET_FILL);
        }

        float units;

        if (states.rasterizer.constantDepthBias == 0.0 ) {
            units = 0.0;
        } else {
            units =
                (states.rasterizer.constantDepthBias * 2.0)
                * (1 << this->m_CurrentTargetDepth->GetDepthBits());
        }

        glPolygonOffset(states.rasterizer.slopeScaledDepthBias * 2.0, units);
    }

    if (this->m_States.rasterizer.viewport.left != states.rasterizer.viewport.left
        || this->m_States.rasterizer.viewport.top != states.rasterizer.viewport.top
        || this->m_States.rasterizer.viewport.width != states.rasterizer.viewport.width
        || this->m_States.rasterizer.viewport.height != states.rasterizer.viewport.height
        || force)
    {
        glViewport(
            states.rasterizer.viewport.left,
            states.rasterizer.viewport.top,
            states.rasterizer.viewport.width,
            states.rasterizer.viewport.height
        );
    }

    if (this->m_States.rasterizer.zNear != states.rasterizer.zNear
        || this->m_States.rasterizer.zFar != states.rasterizer.zFar
        || force)
    {
        glDepthRange(states.rasterizer.zNear, states.rasterizer.zFar);
    }

    if (this->m_States.rasterizer.scissorEnable != states.rasterizer.scissorEnable || force) {
        if (states.rasterizer.scissorEnable) {
            glEnable(GL_SCISSOR_TEST);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }

    if (this->m_States.rasterizer.scissor.left != states.rasterizer.scissor.left
        || this->m_States.rasterizer.scissor.top != states.rasterizer.scissor.top
        || this->m_States.rasterizer.scissor.width != states.rasterizer.scissor.width
        || this->m_States.rasterizer.scissor.height != states.rasterizer.scissor.height
        || force)
    {
        glScissor(
            states.rasterizer.scissor.left,
            states.rasterizer.scissor.top,
            states.rasterizer.scissor.width,
            states.rasterizer.scissor.height
        );
    }

    int32_t maxClipPlaneIndex = GLSDLDevice::GetRendererInfo().unk36;

    if (this->m_States.rasterizer.clipPlaneMask != states.rasterizer.clipPlaneMask) {
        for (int32_t i = 0; i < maxClipPlaneIndex; ++i) {
            if (!(states.rasterizer.clipPlaneMask & this->m_States.rasterizer.clipPlaneMask & (1 << i))) {
                if ((1 << i) & states.rasterizer.clipPlaneMask) {
                    glEnable(GL_CLIP_PLANE0 + i);
                } else {
                    glDisable(GL_CLIP_PLANE0 + i);
                }
            }
        }
    }

    for (int32_t i = 0; i < maxClipPlaneIndex; ++i) {
        if (memcmp(&this->m_States.rasterizer.clipPlanes[i].plane, &states.rasterizer.clipPlanes[i].plane, sizeof(this->m_States.rasterizer.clipPlanes[i].plane))) {
            glClipPlane(GL_CLIP_PLANE0 + i, states.rasterizer.clipPlanes[i].plane);
        }
    }

    int32_t maxColorMaskIndex = GLSDLDevice::m_ExtColorMaskIndexed ? GLSDLDevice::GetRendererInfo().max_color_attachments : 1;

    for (int32_t i = 0; i < maxColorMaskIndex; ++i) {
        if (memcmp(&this->m_States.blend.colorMask[i], &states.blend.colorMask[i], sizeof(GLColor4f)) || force) {
            if (GLSDLDevice::m_ExtColorMaskIndexed) {
                glColorMaskIndexedEXT(
                    i,
                    states.blend.colorMask[i].red,
                    states.blend.colorMask[i].green,
                    states.blend.colorMask[i].blue,
                    states.blend.colorMask[i].alpha
                );
            } else {
                glColorMask(
                    states.blend.colorMask[i].red,
                    states.blend.colorMask[i].green,
                    states.blend.colorMask[i].blue,
                    states.blend.colorMask[i].alpha
                );
            }
        }
    }

    if (this->m_States.blend.alphaBlend != states.blend.alphaBlend || force) {
        if (states.blend.alphaBlend) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }
    }

    if (this->m_States.blend.srcBlendFactor != states.blend.srcBlendFactor
        || this->m_States.blend.destBlendFactor != states.blend.destBlendFactor
        || force)
    {
        glBlendFunc(states.blend.srcBlendFactor, states.blend.destBlendFactor);
    }

    if (this->m_States.blend.blendOp != states.blend.blendOp || force) {
        glBlendEquation(states.blend.blendOp);
    }

    if (memcmp(&this->m_States.blend.blendColor, &states.blend.blendColor, sizeof(GLColor4f)) || force) {
        glBlendColor(
            states.blend.blendColor.r,
            states.blend.blendColor.g,
            states.blend.blendColor.b,
            states.blend.blendColor.a
        );
    }

    if (this->m_States.fixedFunc.fogEnable != states.fixedFunc.fogEnable) {
        if (states.fixedFunc.fogEnable) {
            glEnable(GL_FOG);
        } else {
            glDisable(GL_FOG);
        }
    }

    if (memcmp(&this->m_States.fixedFunc.fogColor, &states.fixedFunc.fogColor, sizeof(GLColor4f)) || force) {
        glFogfv(GL_FOG_COLOR, (GLfloat*)&states.fixedFunc.fogColor);
    }

    if (this->m_States.fixedFunc.fogMode != states.fixedFunc.fogMode || force) {
        glFogi(GL_FOG_MODE, states.fixedFunc.fogMode);
    }

    if (this->m_States.fixedFunc.fogStart != states.fixedFunc.fogStart || force) {
        glFogf(GL_FOG_START, states.fixedFunc.fogStart);
    }

    if (this->m_States.fixedFunc.fogEnd != states.fixedFunc.fogEnd || force) {
        glFogf(GL_FOG_END, states.fixedFunc.fogEnd);
    }

    if (this->m_States.fixedFunc.fogDensity != states.fixedFunc.fogDensity || force) {
        glFogf(GL_FOG_DENSITY, states.fixedFunc.fogDensity);
    }

    if (this->m_States.fixedFunc.alphaTestEnable != states.fixedFunc.alphaTestEnable || force) {
        if (states.fixedFunc.alphaTestEnable) {
            glEnable(GL_ALPHA_TEST);
        } else {
            glDisable(GL_ALPHA_TEST);
        }
    }

    if (this->m_States.fixedFunc.alphaTestFunc != states.fixedFunc.alphaTestFunc
        || this->m_States.fixedFunc.alphaTestRef != states.fixedFunc.alphaTestRef
        || force)
    {
        glAlphaFunc(states.fixedFunc.alphaTestFunc, states.fixedFunc.alphaTestRef);
    }

    if (this->m_States.fixedFunc.transforms.modelView.isIdentity != states.fixedFunc.transforms.modelView.isIdentity
        || memcmp(this->m_States.fixedFunc.transforms.modelView.m, states.fixedFunc.transforms.modelView.m, sizeof(float) * 16)
        || force)
    {
        glMatrixMode(GL_MODELVIEW);

        if (states.fixedFunc.transforms.modelView.isIdentity) {
            glLoadIdentity();
        } else {
            glLoadMatrixf(states.fixedFunc.transforms.modelView.m);
        }

        const_cast<GLStates&>(states).fixedFunc.transforms.modelView.isDirty = false;
    }

    if (this->m_States.fixedFunc.transforms.projection.isIdentity != states.fixedFunc.transforms.projection.isIdentity
        || memcmp(this->m_States.fixedFunc.transforms.projection.m, states.fixedFunc.transforms.projection.m, sizeof(float) * 16)
        || force)
    {
        glMatrixMode(GL_PROJECTION);

        GLTransform projection = {
            true,
            {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            },
            true
        };

        if (!states.fixedFunc.transforms.projection.isIdentity) {
            projection.isIdentity = false;

            memcpy(projection.m, states.fixedFunc.transforms.projection.m, sizeof(projection.m));
            projection.isDirty = true;
        }

        if (projection.isIdentity) {
            projection.SetIdentity();
        }

        projection.a1 *= -1.0f;
        projection.b1 *= -1.0f;
        projection.c1 *= -1.0f;
        projection.d1 *= -1.0f;

        auto isIdentity = projection.a0 == 1.0f
            && projection.a1 == 0.0f
            && projection.a2 == 0.0f
            && projection.a3 == 0.0f
            && projection.b0 == 0.0f
            && projection.b1 == 1.0f
            && projection.b2 == 0.0f
            && projection.b3 == 0.0f
            && projection.c0 == 0.0f
            && projection.c1 == 0.0f
            && projection.c2 == 1.0f
            && projection.c3 == 0.0f
            && projection.d0 == 0.0f
            && projection.d1 == 0.0f
            && projection.d2 == 0.0f
            && projection.d3 == 1.0f;

        projection.isDirty = true;

        if (isIdentity) {
            glLoadIdentity();
        } else {
            glLoadMatrixf(projection.m);
        }

        projection.isDirty = false;
    }

    glMatrixMode(states.fixedFunc.transforms.matrixMode);
    glMatrixMode(GL_MODELVIEW);

    if (states.fixedFunc.transforms.view.isIdentity) {
        glLoadIdentity();
    } else {
        glLoadMatrixf(states.fixedFunc.transforms.view.m);
    }

    const_cast<GLStates&>(states).fixedFunc.transforms.view.isDirty = false;

    if (this->m_States.fixedFunc.lighting.enable != states.fixedFunc.lighting.enable || force) {
        if (states.fixedFunc.lighting.enable) {
            glEnable(GL_LIGHTING);
        } else {
            glDisable(GL_LIGHTING);
        }
    }

    for (int32_t i = 0; i < 8; ++i) {
        // TODO
        // Set up each light
    }

    if (memcmp(&this->m_States.fixedFunc.lighting.sceneAmbient, &states.fixedFunc.lighting.sceneAmbient, sizeof(GLColor4f)) || force) {
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)&states.fixedFunc.lighting.sceneAmbient);
    }

    if (this->m_States.fixedFunc.lighting.material.materialSource != states.fixedFunc.lighting.material.materialSource || force) {
        glColorMaterial(GL_FRONT_AND_BACK, states.fixedFunc.lighting.material.materialSource);

        // TODO
        // this->Sub38A20();
    }

    if (this->m_States.fixedFunc.lighting.material.colorTracking != states.fixedFunc.lighting.material.colorTracking || force) {
        if (states.fixedFunc.lighting.material.colorTracking) {
            glEnable(GL_COLOR_MATERIAL);
        } else {
            glDisable(GL_COLOR_MATERIAL);
        }

        // TODO
        // this->Sub38A20();
    }

    if (memcmp(&this->m_States.fixedFunc.lighting.material.ambient, &states.fixedFunc.lighting.material.ambient, sizeof(GLColor4f)) || force) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&states.fixedFunc.lighting.material.ambient);
    }

    if (memcmp(&this->m_States.fixedFunc.lighting.material.diffuse, &states.fixedFunc.lighting.material.diffuse, sizeof(GLColor4f)) || force) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&states.fixedFunc.lighting.material.diffuse);
    }

    if (memcmp(&this->m_States.fixedFunc.lighting.material.specular, &states.fixedFunc.lighting.material.specular, sizeof(GLColor4f)) || force) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&states.fixedFunc.lighting.material.specular);
    }

    if (memcmp(&this->m_States.fixedFunc.lighting.material.emission, &states.fixedFunc.lighting.material.emission, sizeof(GLColor4f)) || force) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat*)&states.fixedFunc.lighting.material.emission);
    }

    if (this->m_States.fixedFunc.lighting.material.shininess != states.fixedFunc.lighting.material.shininess || force) {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, states.fixedFunc.lighting.material.shininess);
    }

    for (int32_t i = 0; i < 8; ++i) {
        // TODO fixedFunc.transforms.texture[i]
        // TODO fixedFunc.texOp[i]
        // TODO fixedFunc.texGen[i]
    }

    glMatrixMode(states.fixedFunc.transforms.matrixMode);

    if (this->m_States.fixedFunc.normalizeNormal != states.fixedFunc.normalizeNormal) {
        if (states.fixedFunc.normalizeNormal) {
            glEnable(GL_NORMALIZE);
        } else {
            glDisable(GL_NORMALIZE);
        }
    }

    if (this->m_States.fixedFunc.pointSprite.enable != states.fixedFunc.pointSprite.enable || force) {
        if (states.fixedFunc.pointSprite.enable) {
            glEnable(GL_POINT_SPRITE);
            glEnable(GL_PROGRAM_POINT_SIZE_EXT);
        } else {
            glDisable(GL_POINT_SPRITE);
            glDisable(GL_PROGRAM_POINT_SIZE_EXT);
        }
    }

    if (this->m_States.fixedFunc.pointSprite.size != states.fixedFunc.pointSprite.size || force) {
        glPointSize(states.fixedFunc.pointSprite.size);
    }

    if (memcmp(&this->m_States.fixedFunc.pointSprite.attenuation, &states.fixedFunc.pointSprite.attenuation, sizeof(this->m_States.fixedFunc.pointSprite.attenuation)) || force) {
        glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION, states.fixedFunc.pointSprite.attenuation);
    }

    if (this->m_States.fixedFunc.pointSprite.min != states.fixedFunc.pointSprite.min || force ) {
        glPointParameterfARB(GL_POINT_SIZE_MIN, states.fixedFunc.pointSprite.min);
    }

    if (this->m_States.fixedFunc.pointSprite.max != states.fixedFunc.pointSprite.max || force) {
        glPointParameterfARB(GL_POINT_SIZE_MAX, states.fixedFunc.pointSprite.max);
    }

    if (this->m_States.shader.vertexShaderEnable != states.shader.vertexShaderEnable || force) {
        if (states.shader.vertexShaderEnable) {
            glEnable(GL_VERTEX_PROGRAM_ARB);
        } else {
            glDisable(GL_VERTEX_PROGRAM_ARB);
        }
    }

    glProgramEnvParameters4fvEXT(GL_VERTEX_PROGRAM_ARB, 0, 256, (GLfloat*)states.shader.vertexShaderConst);

    if (this->m_States.shader.pixelShaderEnable != states.shader.pixelShaderEnable || force) {
        if (states.shader.pixelShaderEnable) {
            glEnable(GL_FRAGMENT_PROGRAM_ARB);
        } else {
            glDisable(GL_FRAGMENT_PROGRAM_ARB);
        }
    }

    glProgramEnvParameters4fvEXT(GL_FRAGMENT_PROGRAM_ARB, 0, 64, (GLfloat*)states.shader.pixelShaderConst);

    if (memcmp(&this->m_States.clear.clearColor, &states.clear.clearColor, sizeof(GLColor4f)) || force) {
        glClearColor(
            states.clear.clearColor.r,
            states.clear.clearColor.g,
            states.clear.clearColor.b,
            states.clear.clearColor.a
        );
    }

    if (this->m_States.clear.clearDepth != states.clear.clearDepth || force) {
        glClearDepth(states.clear.clearDepth);
    }

    if (this->m_States.clear.clearStencil != states.clear.clearStencil || force) {
        glClearStencil(states.clear.clearStencil);
    }

    // Copy provided states into current states

    memcpy(&this->m_States.depth, &states.depth, sizeof(this->m_States.depth));
    memcpy(&this->m_States.stencil, &states.stencil, sizeof(this->m_States.stencil));

    if (!states.stencil.useTwoSidedStencil) {
        memcpy(&this->m_States.stencil.back, &states.stencil.front, sizeof(this->m_States.stencil.back));
    }

    if (this->m_CurrentTargetDepth) {
        memcpy(&this->m_States.rasterizer, &states.rasterizer, sizeof(this->m_States.rasterizer));
    } else {
        float v117 = this->m_States.rasterizer.constantDepthBias;
        float v118 = this->m_States.rasterizer.slopeScaledDepthBias;
        memcpy(&this->m_States.rasterizer, &states.rasterizer, sizeof(this->m_States.rasterizer));
        this->m_States.rasterizer.constantDepthBias = v117;
        this->m_States.rasterizer.slopeScaledDepthBias = v118;
    }

    memcpy(&this->m_States.blend, &states.blend, sizeof(this->m_States.blend));
    memcpy(&this->m_States.clear, &states.clear, sizeof(this->m_States.clear));

    this->m_States.fixedFunc.fogEnable = states.fixedFunc.fogEnable;
    memcpy(&this->m_States.fixedFunc.fogColor, &states.fixedFunc.fogColor, sizeof(GLColor4f));
    this->m_States.fixedFunc.fogMode = states.fixedFunc.fogMode;
    this->m_States.fixedFunc.fogStart = states.fixedFunc.fogStart;
    this->m_States.fixedFunc.fogEnd = states.fixedFunc.fogEnd;

    this->m_States.fixedFunc.alphaTestEnable = states.fixedFunc.alphaTestEnable;
    this->m_States.fixedFunc.alphaTestRef = states.fixedFunc.alphaTestRef;

    memcpy(this->m_States.fixedFunc.texOp, states.fixedFunc.texOp, sizeof(this->m_States.fixedFunc.texOp));

    this->m_States.fixedFunc.lighting.enable = states.fixedFunc.lighting.enable;
    memcpy(&this->m_States.fixedFunc.lighting.sceneAmbient, &states.fixedFunc.lighting.sceneAmbient, sizeof(GLColor4f));

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.lighting.lights[i].enable = states.fixedFunc.lighting.lights[i].enable;
        memcpy(&this->m_States.fixedFunc.lighting.lights[i].position, &states.fixedFunc.lighting.lights[i].position, sizeof(GLfloat4));
        COPY_TRANSFORM(this->m_States.fixedFunc.lighting.lights[i].view, states.fixedFunc.lighting.lights[i].view);
        this->m_States.fixedFunc.lighting.lights[i].constantAttenuation = states.fixedFunc.lighting.lights[i].constantAttenuation;
        this->m_States.fixedFunc.lighting.lights[i].linearAttenuation = states.fixedFunc.lighting.lights[i].linearAttenuation;
        this->m_States.fixedFunc.lighting.lights[i].quadraticAttenuation = states.fixedFunc.lighting.lights[i].quadraticAttenuation;
        memcpy(&this->m_States.fixedFunc.lighting.lights[i].ambient, &states.fixedFunc.lighting.lights[i].ambient, sizeof(GLColor4f));
        memcpy(&this->m_States.fixedFunc.lighting.lights[i].diffuse, &states.fixedFunc.lighting.lights[i].diffuse, sizeof(GLColor4f));
        memcpy(&this->m_States.fixedFunc.lighting.lights[i].specular, &states.fixedFunc.lighting.lights[i].specular, sizeof(GLColor4f));
    }

    memcpy(&this->m_States.fixedFunc.lighting.material, &states.fixedFunc.lighting.material, sizeof(this->m_States.fixedFunc.lighting.material));

    COPY_TRANSFORM(this->m_States.fixedFunc.transforms.modelView, states.fixedFunc.transforms.modelView);
    COPY_TRANSFORM(this->m_States.fixedFunc.transforms.world, states.fixedFunc.transforms.world);
    COPY_TRANSFORM(this->m_States.fixedFunc.transforms.view, states.fixedFunc.transforms.view);
    COPY_TRANSFORM(this->m_States.fixedFunc.transforms.projection, states.fixedFunc.transforms.projection);

    for (int32_t i = 0; i < 8; ++i) {
        COPY_TRANSFORM(this->m_States.fixedFunc.transforms.texture[i], states.fixedFunc.transforms.texture[i]);
    }

    memcpy(this->m_States.fixedFunc.texCoordIndex, states.fixedFunc.texCoordIndex, sizeof(this->m_States.fixedFunc.texCoordIndex));
    memcpy(this->m_States.fixedFunc.texGen, states.fixedFunc.texGen, sizeof(this->m_States.fixedFunc.texGen));

    memcpy(this->m_States.samplers, states.samplers, sizeof(this->m_States.samplers));
    memcpy(&this->m_States.shader, &states.shader, sizeof(this->m_States.shader));
}

void GLSDLDevice::ApplyShaderConstants() {
    GLShader* vs = this->m_VertexShader;

    if (vs) {
        if (vs->m_UsingGLSL) {
            vs->FlushUniforms(this->m_GLSLProgram);
        } else {
            auto start = this->m_DirtyVertexShaderConsts.start;
            auto end = this->m_DirtyVertexShaderConsts.end;

            if (start != end) {
                glProgramEnvParameters4fvEXT(
                    GL_VERTEX_PROGRAM_ARB,
                    start,
                    end - start,
                    reinterpret_cast<GLfloat*>(&this->m_States.shader.vertexShaderConst[start])
                );

                this->m_DirtyVertexShaderConsts.start = 0;
                this->m_DirtyVertexShaderConsts.end = 0;
            }
        }
    }

    GLShader* ps = this->m_PixelShader;

    if (ps) {
        if (ps->m_UsingGLSL) {
            ps->FlushUniforms(this->m_GLSLProgram);
        } else {
            auto start = this->m_DirtyPixelShaderConsts.start;
            auto end = this->m_DirtyPixelShaderConsts.end;

            if (start != end) {
                glProgramEnvParameters4fvEXT(
                    GL_FRAGMENT_PROGRAM_ARB,
                    start,
                    end - start,
                    reinterpret_cast<GLfloat*>(&this->m_States.shader.pixelShaderConst[start])
                );

                this->m_DirtyPixelShaderConsts.start = 0;
                this->m_DirtyPixelShaderConsts.end = 0;
            }
        }
    }
}

void GLSDLDevice::ApplyTransforms() {
    this->SetModelView(GL_MODELVIEW);

    auto& projection = this->m_States.fixedFunc.transforms.projection;
    if (projection.isDirty) {
        if (projection.isIdentity) {
            projection.SetIdentity();
        }

        projection.a1 *= -1.0f;
        projection.b1 *= -1.0f;
        projection.c1 *= -1.0f;
        projection.d1 *= -1.0f;

        projection.isIdentity = projection.a0 == 1.0f
            && projection.a1 == 0.0f
            && projection.a2 == 0.0f
            && projection.a3 == 0.0f
            && projection.b0 == 0.0f
            && projection.b1 == 1.0f
            && projection.b2 == 0.0f
            && projection.b3 == 0.0f
            && projection.c0 == 0.0f
            && projection.c1 == 0.0f
            && projection.c2 == 1.0f
            && projection.c3 == 0.0f
            && projection.d0 == 0.0f
            && projection.d1 == 0.0f
            && projection.d2 == 0.0f
            && projection.d3 == 1.0f;

        projection.isDirty = true;

        if (this->m_States.fixedFunc.transforms.matrixMode != GL_PROJECTION) {
            glMatrixMode(GL_PROJECTION);
            this->m_States.fixedFunc.transforms.matrixMode = GL_PROJECTION;
        }

        if (projection.isIdentity) {
            glLoadIdentity();
        } else {
            glLoadMatrixf(projection.m);
        }

        projection.isDirty = false;
    }

    // TODO texture transforms
}

void GLSDLDevice::BindBuffer(GLBuffer* buffer, GLEnum target) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());
    BLIZZARD_ASSERT(buffer != nullptr || target != GL_ZERO);

    GLEnum bindTarget = target == GL_ZERO ? buffer->m_Type : target;
    GLuint bindName = buffer == nullptr ? 0 : buffer->m_BufferID;

    int32_t bindIndex;

    if (bindTarget == GL_ARRAY_BUFFER) {
        bindIndex = 0;
    } else if (bindTarget == GL_ELEMENT_ARRAY_BUFFER) {
        bindIndex = 1;
    } else if (bindTarget == GL_PIXEL_PACK_BUFFER) {
        bindIndex = 2;
    } else if (bindTarget == GL_PIXEL_UNPACK_BUFFER) {
        bindIndex = 3;
    } else {
        BLIZZARD_ASSERT(false);
    }

    if (bindTarget == GL_ARRAY_BUFFER) {
        if (this->m_States.binding.vertexArrayObject && this->m_VertexArrayObject != &this->m_DefaultVertexArrayObject) {
            glBindVertexArray(0);
            this->m_States.binding.vertexArrayObject = 0;
            this->m_VertexArrayObject = &this->m_DefaultVertexArrayObject;
        }
    } else if (bindTarget == GL_PIXEL_PACK_BUFFER) {
        this->m_VertexArrayObject->m_Properties.m_PixelPackBuffer = buffer;
    } else if (bindTarget == GL_PIXEL_UNPACK_BUFFER) {
        this->m_VertexArrayObject->m_Properties.m_PixelUnpackBuffer = buffer;
    }

    if (this->m_VertexArrayObject->m_GLStates.buffers[bindIndex] != bindName) {
        glBindBuffer(bindTarget, bindName);
        this->m_VertexArrayObject->m_GLStates.buffers[bindIndex] = bindName;
    }
}

void GLSDLDevice::BindFramebuffer(GLFramebuffer* framebuffer) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());

    GLuint v3;

    if (framebuffer) {
        v3 = framebuffer->m_FramebufferID;
    } else {
        v3 = 0;
    }

    this->m_CurrentTarget = framebuffer;

    if (this->m_States.binding.framebuffer != v3) {
        glBindFramebufferEXT(GL_FRAMEBUFFER, v3);
        this->m_States.binding.framebuffer = v3;
    }
}

void GLSDLDevice::BindGLSLProgram(GLGLSLProgram* a2) {
    // TODO
}

void GLSDLDevice::BindShader(GLShader* shader) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());
    BLIZZARD_ASSERT(shader);

    if (shader->var5 == GL_FRAGMENT_PROGRAM_ARB) {
        if (this->m_States.binding.pixelProgram != shader->m_ShaderID) {
            glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, shader->m_ShaderID);
            this->m_States.binding.pixelProgram = shader->m_ShaderID;
        }
    } else if (shader->var5 == GL_VERTEX_PROGRAM_ARB) {
        if (this->m_States.binding.vertexProgram != shader->m_ShaderID) {
            glBindProgramARB(GL_VERTEX_PROGRAM_ARB, shader->m_ShaderID);
            this->m_States.binding.vertexProgram = shader->m_ShaderID;
        }
    } else {
        BLIZZARD_ASSERT(false);
    }
}

void GLSDLDevice::BindTexture(GLEnum textureType, GLTexture* texture) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());
    BLIZZARD_ASSERT(texture == nullptr || textureType == texture->m_TextureType);

    uint32_t textureID = texture ? texture->m_TextureID : 0;
    uint32_t index = GLSDLTextureTypeToIndex(textureType);

    BLIZZARD_ASSERT(this->m_States.binding.texture[index][this->m_States.binding.currentActiveTexture] != textureID);
    BLIZZARD_ASSERT(this->m_BoundTextures[index][this->m_States.binding.currentActiveTexture] != texture);

    GLTexture* boundTexture = this->m_BoundTextures[index][this->m_States.binding.currentActiveTexture];
    if (boundTexture) {
        boundTexture->Unbind(this, this->m_States.binding.currentActiveTexture);
    }

    glBindTexture(textureType, textureID);
    this->m_BoundTextures[index][this->m_States.binding.currentActiveTexture] = texture;
    this->m_States.binding.texture[index][this->m_States.binding.currentActiveTexture] = textureID;

    if (!texture || this->m_WorkerDevice || textureType == GL_TEXTURE_RECTANGLE) {
        return;
    }

    GLSDLDevice* mainDevice = GLSDLDevice::m_Devices[0];

    if (texture->var7 == mainDevice->m_TextureList.begin()) {
        texture->var7 = mainDevice->m_TextureList.insert(mainDevice->m_TextureList.begin(), texture);

        mainDevice->m_TextureTotalSize += texture->m_Size;
        texture->m_LastFrameUsed = mainDevice->m_FrameNumber;
    } else if (texture->m_LastFrameUsed == 0 || texture->m_LastFrameUsed + 60 < mainDevice->m_FrameNumber) {
        if (texture->m_LastFrameUsed == 0) {
            texture->RecreateGLTexture();
            mainDevice->m_TextureTotalSize += texture->m_Size;
        }

        // TODO
        // - oldest texture logic

        // TODO
        // - list transfer logic

        texture->m_LastFrameUsed = mainDevice->m_FrameNumber;
    }

    if (mainDevice->m_TextureTotalSize > 384 * 1024 * 1024) {
        // TODO
        // - track / clean up oldest textures
    }
}

void GLSDLDevice::BindVertexArray(GLVertexArray* a2) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());

    int32_t v4 = a2 ? a2->m_VertexArrayID : 0;

    if (this->m_States.binding.vertexArrayObject != v4) {
        glBindVertexArray(v4);
        this->m_States.binding.vertexArrayObject = v4;
        this->m_VertexArrayObject = a2 ? a2 : &this->m_DefaultVertexArrayObject;
    }
}

void GLSDLDevice::BlitFramebuffer(GLMipmap* src, const GLRect* srcRect, GLMipmap* dst, const GLRect* dstRect, GLEnum mask, GLEnum filter) {
    BLIZZARD_ASSERT(mask == GL_COLOR_BUFFER_BIT);
    BLIZZARD_ASSERT(src != nullptr);

    GLRect fullSrcRect = {
        0,
        0,
        src->GetWidth(),
        src->GetHeight()
    };

    GLRect fullDstRect = {
        0,
        0,
        dst ? dst->GetWidth() : this->m_Window->GetWidth(),
        dst ? dst->GetHeight() : this->m_Window->GetHeight()
    };

    BLIZZARD_ASSERT(filter == GL_NEAREST);
    // TODO
    // BLIZZARD_ASSERT(srcRect == nullptr || *srcRect == fullSrcRect);

    // TODO
    // - non-shader code path

    glEnable(GL_VERTEX_PROGRAM_ARB);
    glEnable(GL_FRAGMENT_PROGRAM_ARB);

    auto alphaTestEnable = this->m_States.fixedFunc.alphaTestEnable;
    auto depthTestEnable = this->m_States.depth.testEnable;
    auto alphaBlend = this->m_States.blend.alphaBlend;
    auto cullEnable = this->m_States.rasterizer.cullMode != 0;
    auto scissorEnable = this->m_States.rasterizer.scissorEnable;
    auto stencilTestEnable = this->m_States.stencil.testEnable;
    uint32_t width;
    uint32_t height;

    if (alphaTestEnable) {
        glDisable(GL_ALPHA_TEST);
    }

    if (depthTestEnable) {
        glDisable(GL_DEPTH_TEST);
    }

    if (alphaBlend) {
        glDisable(GL_BLEND);
    }

    if (cullEnable) {
        glDisable(GL_CULL_FACE);
    }

    if (scissorEnable) {
        glDisable(GL_SCISSOR_TEST);
    }

    if (stencilTestEnable) {
        glDisable(GL_STENCIL_TEST);
    }

    if (GLSDLDevice::m_ExtColorMaskIndexed) {
        glColorMaskIndexedEXT(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    } else {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    if (dst) {
        glFramebufferTexture2DEXT(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            dst->m_Target,
            dst->GetTextureID(),
            dst->m_Level
        );

        auto currentTargetDepth = this->m_CurrentTargetDepth;
        if (currentTargetDepth) {
            glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                currentTargetDepth->m_Target,
                0,
                0
            );
        }

        auto currentTargetStencil = this->m_CurrentTargetStencil;
        if (currentTargetStencil) {
            glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER,
                GL_STENCIL_ATTACHMENT,
                currentTargetStencil->m_Target,
                0,
                0
            );
        }

        dst->m_Texture->SetAddressModeS(GL_CLAMP_TO_EDGE);
        dst->m_Texture->SetAddressModeT(GL_CLAMP_TO_EDGE);
        dst->m_Texture->SetMinFilterMode(GL_LINEAR);
        dst->m_Texture->SetMagFilterMode(GL_LINEAR);

        width = dst->m_Width;
        height = dst->m_Height;
    } else {
        glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

        width = this->m_Window->GetWidth();
        height = this->m_Window->GetHeight();
    }

    if (
        this->m_States.rasterizer.viewport.left != 0
        || this->m_States.rasterizer.viewport.top != 0
        || this->m_States.rasterizer.viewport.width != width
        || this->m_States.rasterizer.viewport.height != height
    ) {
        glViewport(0, 0, width, height);
    }

    this->SetActiveTexture(0);
    src->m_Texture->Bind(nullptr, true);
    src->m_Texture->SetAddressModeS(GL_CLAMP_TO_EDGE);
    src->m_Texture->SetAddressModeT(GL_CLAMP_TO_EDGE);
    src->m_Texture->SetMinFilterMode(filter);
    src->m_Texture->SetMagFilterMode(filter);

    this->SetVertexBuffer(0, GLSDLDevice::m_BlitQuadVBO, 0, 28);

    GLVertexFormat* format = dst
        ? &GLSDLDevice::m_NormalBlitVF
        : &GLSDLDevice::m_InvertedBlitVF;

    this->SetVertexFormat(format);

    auto vertexShader = this->m_VertexShader;
    auto pixelShader = this->m_PixelShader;
    this->BindGLSLProgram(nullptr);

    this->SetShader(GLShader::eVertexShader, GLSDLDevice::m_DeviceShaders[0]);
    this->SetShader(GLShader::ePixelShader, GLSDLDevice::m_DeviceShaders[2]);

    this->m_DefaultVertexArrayObject.m_Properties.m_VertexBase = 0;
    GLVertexArray::FindVertexArray(this, this->m_DefaultVertexArrayObject);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    this->SetShader(GLShader::eVertexShader, vertexShader);
    this->SetShader(GLShader::ePixelShader, pixelShader);

    if (alphaTestEnable) {
        glEnable(GL_ALPHA_TEST);
    }

    if (depthTestEnable) {
        glEnable(GL_DEPTH_TEST);
    }

    if (alphaBlend) {
        glEnable(GL_BLEND);
    }

    if (cullEnable) {
        glEnable(GL_CULL_FACE);
    }

    if (scissorEnable) {
        glEnable(GL_SCISSOR_TEST);
    }

    if (stencilTestEnable) {
        glEnable(GL_STENCIL_TEST);
    }

    if (GLSDLDevice::m_ExtColorMaskIndexed) {
        glColorMaskIndexedEXT(
            0,
            this->m_States.blend.colorMask[0].red,
            this->m_States.blend.colorMask[0].green,
            this->m_States.blend.colorMask[0].blue,
            this->m_States.blend.colorMask[0].alpha
        );
    } else {
        glColorMask(
            this->m_States.blend.colorMask[0].red,
            this->m_States.blend.colorMask[0].green,
            this->m_States.blend.colorMask[0].blue,
            this->m_States.blend.colorMask[0].alpha
        );
    }

    if (dst) {
        auto currentTargetColor = this->m_CurrentTargetColor[0];
        glFramebufferTexture2DEXT(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            dst->m_Target,
            currentTargetColor->GetTextureID(),
            0
        );

        auto currentTargetDepth = this->m_CurrentTargetDepth;
        if (currentTargetDepth) {
            glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                currentTargetDepth->m_Target,
                currentTargetDepth->GetTextureID(),
                currentTargetDepth->m_Level
            );
        }

        auto currentTargetStencil = this->m_CurrentTargetStencil;
        if (currentTargetStencil) {
            glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER,
                GL_STENCIL_ATTACHMENT,
                currentTargetStencil->m_Target,
                currentTargetStencil->GetTextureID(),
                currentTargetStencil->m_Level
            );
        }
    }

    if (
        this->m_States.rasterizer.viewport.left != 0
        || this->m_States.rasterizer.viewport.top != 0
        || this->m_States.rasterizer.viewport.width != width
        || this->m_States.rasterizer.viewport.height != height
    ) {
        glViewport(
            this->m_States.rasterizer.viewport.left,
            this->m_States.rasterizer.viewport.top,
            this->m_States.rasterizer.viewport.width,
            this->m_States.rasterizer.viewport.height
        );
    }
}

void GLSDLDevice::CheckDepthTarget() {
    BLIZZARD_ASSERT(this->m_CurrentTargetColor[0] == nullptr || this->m_CurrentTargetColor[0]->GetTexture()->IsValid());
    BLIZZARD_ASSERT(this->m_CurrentTargetColor[1] == nullptr || this->m_CurrentTargetColor[1]->GetTexture()->IsValid());
    BLIZZARD_ASSERT(this->m_CurrentTargetColor[2] == nullptr || this->m_CurrentTargetColor[2]->GetTexture()->IsValid());
    BLIZZARD_ASSERT(this->m_CurrentTargetColor[3] == nullptr || this->m_CurrentTargetColor[3]->GetTexture()->IsValid());

    BLIZZARD_ASSERT(this->m_CurrentTargetDepth == nullptr || this->m_CurrentDepthBuffer != nullptr);

    if (!this->m_CurrentDepthBuffer) {
        return;
    }

    BLIZZARD_ASSERT(this->m_CurrentDepthBuffer->GetTexture()->IsValid());

    auto currentTargetColor = this->m_CurrentTargetColor[0];
    auto currentDepthBuffer = this->m_CurrentDepthBuffer;

    if (
        !currentTargetColor
        || (currentTargetColor->m_Width == currentDepthBuffer->m_Width && currentTargetColor->m_Height == currentDepthBuffer->m_Height)
    ) {
        if (this->m_CurrentTargetDepth != currentDepthBuffer) {
            this->Sub34BB0(GL_DEPTH_ATTACHMENT, currentDepthBuffer, 0);
            this->m_CurrentTarget->Attach(currentDepthBuffer, GL_DEPTH_ATTACHMENT, 0);
            auto currentTargetDepth = this->m_CurrentTargetDepth;
            this->m_CurrentTargetDepth = currentDepthBuffer;
            this->m_CurrentDepthBuffer = currentDepthBuffer;

            if (
                (currentTargetDepth == nullptr && currentDepthBuffer != nullptr)
                || (currentDepthBuffer != nullptr && currentTargetDepth != nullptr && currentTargetDepth->m_DepthBits != currentDepthBuffer->m_DepthBits)
            ) {
                this->SetDepthBias(this->m_ConstantDepthBias, this->m_SlopeScaledDepthBias);
            }

            if (this->m_CurrentDepthBuffer->GetFormat() == GLTF_D24S8) {
                this->Sub34BB0(GL_STENCIL_ATTACHMENT, this->m_CurrentDepthBuffer, 0);
                this->m_CurrentTarget->Attach(this->m_CurrentDepthBuffer, GL_STENCIL_ATTACHMENT, 0);
                this->m_CurrentTargetStencil = this->m_CurrentDepthBuffer;
            }
        }

        return;
    }

    auto currentTargetDepth = this->m_CurrentTargetDepth;

    if (currentTargetDepth == currentDepthBuffer) {
        this->Sub34BB0(GL_DEPTH_ATTACHMENT, nullptr, 0);
        this->m_CurrentTarget->Attach(nullptr, GL_DEPTH_ATTACHMENT, 0);
        this->m_CurrentTargetDepth = nullptr;
        this->m_CurrentDepthBuffer = currentTargetDepth;

        if (currentTargetDepth->GetFormat() == GLTF_D24S8) {
            this->Sub34BB0(GL_STENCIL_ATTACHMENT, nullptr, 0);
            this->m_CurrentTarget->Attach(nullptr, GL_STENCIL_ATTACHMENT, 0);
            this->m_CurrentTargetStencil = nullptr;
        }
    }
}

void GLSDLDevice::Clear(uint32_t clearMask, const GLColor4f& clearColor, double clearDepth, int32_t clearStencil) {
    this->CheckDepthTarget();
    this->RestoreTextures();

    auto colorMask = this->m_States.blend.colorMask[0];

    if (clearMask & 0x4000) {
        for (int32_t i = 0; i < 4; i++) {
            if (this->m_CurrentTargetColor[i] && this->m_CurrentTargetColor[i]->GetTextureID() == this->m_States.binding.texture[0][this->m_States.binding.currentActiveTexture]) {
                this->BindTexture(this->m_CurrentTargetColor[i]->m_Target, nullptr);
            }
        }

        this->SetColorWriteMask(1, 1, 1, 1, 0);
        this->SetClearColor(clearColor);
    }

    if (clearMask & 0x100) {
        if (this->m_CurrentTargetDepth) {
            if (this->m_CurrentTargetDepth->GetTextureID() == this->m_States.binding.texture[0][this->m_States.binding.currentActiveTexture]) {
                this->BindTexture(this->m_CurrentTargetDepth->m_Target, nullptr);
            }

            if (!this->m_States.depth.writeMask) {
                glDepthMask(1);
            }

            this->SetClearDepth(clearDepth);
        } else {
            clearMask &= ~0x100;
        }
    } else {
        if (this->m_CurrentTargetDepth) {
            this->m_CurrentTarget->Detach(GL_DEPTH_ATTACHMENT);
        }
    }

    if (clearMask & 0x400) {
        if (this->m_CurrentTargetStencil) {
            if (this->m_CurrentTargetStencil->GetTextureID() == this->m_States.binding.texture[0][this->m_States.binding.currentActiveTexture]) {
                this->BindTexture(this->m_CurrentTargetStencil->m_Target, nullptr);
            }

            if (this->m_States.stencil.writeMask != -1) {
                glStencilMask(-1);
            }

            this->SetClearStencil(clearStencil);
        } else {
            clearMask &= ~0x400;
        }
    } else {
        if (this->m_CurrentTargetStencil) {
            this->m_CurrentTarget->Detach(GL_STENCIL_ATTACHMENT);
        }
    }

    auto scissorEnable = this->m_States.rasterizer.scissorEnable;
    auto scissorRect = this->m_States.rasterizer.scissor;
    auto currentTargetColor = this->m_CurrentTargetColor[0];

    if (
        currentTargetColor
        && (
            this->m_States.rasterizer.viewport.left
            || this->m_States.rasterizer.viewport.top
            || this->m_States.rasterizer.viewport.width != currentTargetColor->m_Width
            || this->m_States.rasterizer.viewport.height != currentTargetColor->m_Height
        )
    ) {
        this->SetScissor(1, this->m_States.rasterizer.viewport);
        glClear(clearMask);
        this->SetScissor(scissorEnable, scissorRect);
    } else {
        glClear(clearMask);
    }

    if (clearMask & 0x4000) {
        this->SetColorWriteMask(colorMask.red, colorMask.blue, colorMask.green, colorMask.alpha, 0);
    }

    if (clearMask & 0x100) {
        if (!this->m_States.depth.writeMask) {
            glDepthMask(0);
        }
    } else {
        if (this->m_CurrentTargetDepth) {
            this->m_CurrentTarget->Attach(this->m_CurrentTargetDepth, GL_DEPTH_ATTACHMENT, 0);
        }
    }

    if (clearMask & 0x400) {
        if (this->m_States.stencil.writeMask != -1) {
            glStencilMask(this->m_States.stencil.writeMask);
        }
    } else {
        if (this->m_CurrentTargetStencil) {
            this->m_CurrentTarget->Attach(this->m_CurrentTargetStencil, GL_STENCIL_ATTACHMENT, 0);
        }
    }
}

void GLSDLDevice::CopyTex(uint32_t a2, uint32_t a3, GLMipmap* dst, const GLRect* framebufferRect) {
    BLIZZARD_ASSERT(framebufferRect->width == dst->GetWidth());
    BLIZZARD_ASSERT(framebufferRect->height == dst->GetHeight());

    dst->m_Texture->Bind(nullptr, false);

    glCopyTexSubImage2D(
        dst->m_Target,
        dst->m_Level,
        a2,
        a3,
        framebufferRect->left,
        framebufferRect->top,
        framebufferRect->width,
        framebufferRect->height
    );
}

GLBuffer* GLSDLDevice::CreateBuffer(GLEnum type, uint32_t a3, const void* a4, GLEnum usage, GLEnum format) {
    return GLBuffer::Create(type, a3, a4, usage, format);
}

GLShader* GLSDLDevice::CreateShader(GLShader::ShaderType type, const void* buf, int32_t codeLen, const char* name) {
    // TODO
    // Blizzard::Debug::Assert(this);

    return GLShader::Create(type, GLSDLDevice::m_UseHybridShader, false, "", buf, codeLen, "", name, nullptr);
}

GLTexture* GLSDLDevice::CreateTexture2D(uint32_t width, uint32_t height, uint32_t numMipMap, GLTextureFormat format, uint32_t flags) {
    return  GLTexture2D::Create(width, height, numMipMap, format, flags);
}

GLTexture* GLSDLDevice::CreateTextureCubeMap(uint32_t size, uint32_t numMipMap, GLTextureFormat format, uint32_t flags) {
    // TODO

    return nullptr;
}

void GLSDLDevice::Draw(GLEnum primitive, uint32_t a3, uint32_t a4) {
    // TODO
}

void GLSDLDevice::DrawIndexed(GLEnum primitive, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t count) {
    this->GLSDLDraw(primitive, a3, a4, a5, a6, count);
}

void GLSDLDevice::DrawRect() {
    if (!this->m_Window) {
        return;
    }

    if (!this->m_UseWindowSystemBuffer || this->m_FlippedSystemBuffer) {
        GLTexture2D* backBuffer = this->m_BackBufferColor;

        if (!backBuffer) {
            return;
        }

        GLMipmap* backBufferImage = backBuffer->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        this->BlitFramebuffer(backBufferImage, nullptr, nullptr, nullptr, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
        this->m_States.binding.framebuffer = 0;
    }

    this->m_Window->Swap();

    if (!this->m_UseWindowSystemBuffer || this->m_FlippedSystemBuffer) {
        glBindFramebufferEXT(GL_FRAMEBUFFER, this->m_CurrentTarget->m_FramebufferID);
        this->m_States.binding.framebuffer = this->m_CurrentTarget->m_FramebufferID;
    }
}

uint32_t GLSDLDevice::GetID() {
    return this->m_ID;
}

GLFramebuffer* GLSDLDevice::GetCurrentTarget() {
    return this->m_CurrentTarget;
}

GLShader* GLSDLDevice::GetShader(GLShader::ShaderType shaderType) {
    if (shaderType == GLShader::eVertexShader) {
        return this->m_VertexShader;
    } else if (shaderType == GLShader::ePixelShader) {
        return this->m_PixelShader;
    } else {
        return nullptr;
    }
}

const GLStates::VertexArrayObject& GLSDLDevice::GetVertexArrayStates() {
    return this->m_VertexArrayObject->m_GLStates;
}

void GLSDLDevice::GLSDLDraw(GLEnum mode, uint32_t start, uint32_t end, uint32_t a5,  uint32_t a6, uint32_t count) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());

    this->CheckDepthTarget();

    // if (!this->m_DrawCount) {
    //     if (this->m_Context.m_MTGLEnabled) {
    //         glFlush();
    //     }

    //     // GLFence::TestFences();
    // }

    this->m_DrawCount++;

    this->RestoreTextures();

    this->m_DefaultVertexArrayObject.m_Properties.m_VertexBase = a5;
    GLVertexArray::FindVertexArray(this, this->m_DefaultVertexArrayObject);

    auto vs = this->m_VertexShader;
    auto ps = this->m_PixelShader;

    if (ps) {
        // TODO
        // this->Sub30440();
    }

    // if (this->m_TexWorker && !this->m_TexWorker->m_UnkA1) {
    //     this->m_TexWorker->WaitOnGLObjects();
    // }

    if (vs) {
        this->SetShader(GLShader::eVertexShader, vs);
    } else {
        this->ApplyTransforms();
    }

    if (ps) {
        this->SetShader(GLShader::ePixelShader, ps);
    } else {
        this->UpdateFFPTexturing();
    }

    if (vs && vs->m_UsingGLSL && ps && ps->m_UsingGLSL) {
        this->m_GLSLProgram = GLGLSLProgram::Find(vs, ps);
    } else {
        this->BindGLSLProgram(nullptr);
    }

    this->ApplyShaderConstants();

    if (count) {
        GLBuffer* buffer = this->m_VertexArrayObject->m_Properties.m_IndexBuffer;
        GLEnum format = buffer->m_IndexFormat;

        uint32_t v18;

        if (format == GL_UNSIGNED_SHORT) {
            v18 = 1;
        } else if (format == GL_UNSIGNED_INT) {
            v18 = 2;
        } else {
            BLIZZARD_ASSERT(!"buffer uses unknown format");
        }

        void* indices = GLBuffer::m_UsingVBO
            ? reinterpret_cast<void*>(a6 << v18)
            : buffer->m_Data + (a6 << v18);

        glDrawRangeElements(mode, start, end, count, buffer->m_IndexFormat, indices);
    } else {
        glDrawArrays(mode, start, end - start);
    }
}

void GLSDLDevice::Init(GLSDLWindow* window, const char* debugName, uint32_t flags) {
    if (this->m_Init) {
        return;
    }

    this->m_BatchViewerEnabled = flags & 0x1;
    this->m_UseWindowSystemBuffer = flags & (0x2 | 0x4);
    this->m_FlippedSystemBuffer = flags & 0x4;
    this->m_ShaderCompiler = flags & 0x8;
    this->m_WorkerDevice = flags & 0x10;

    this->m_DebugName.assign(debugName, strlen(debugName));

    this->m_Window = window;

    // *** sets
    // this->m_UseWindowSystemBuffer = false;
    // ***

    GLSDLDevice::Set(this);

    this->m_Context.Create(window);
    this->m_Context.MakeCurrent(window);

    // TODO
    // if (dword_10329E4 == dword_10329E0)
    //     GLRendererInfo::InitExtensions();

    this->LoadDefaultStates();

    this->m_Init = true;

    if (this->m_ID == -1) {
        this->m_ID = GLSDLDevice::m_Devices.size();
        GLSDLDevice::m_Devices.insert(GLSDLDevice::m_Devices.end(), this);
    } else {
        GLSDLDevice::m_Devices[this->m_ID] = this;
    }

    if (!GLSDLDevice::m_StaticResourcesRefCount) {
        // GLContext::s_MainContext = this->m_Context.m_Context->context;
        GLSDLDevice::InitPools();
        GLSDLDevice::StaticInit();
    }

    ++GLSDLDevice::m_StaticResourcesRefCount;

    // if (!this->m_WorkerDevice) {
    //     GLWorker* v11 = new GLWorker(this);
    //     this->m_TexWorker = v11;
    // }

    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLFramebuffer* v12 = GLFramebuffer::Create(0);
    this->m_FBOTarget = v12;
    GLFramebuffer* currentTarget = v12;

    // TODO verify this check
    if (this->m_UseWindowSystemBuffer) {
        GLFramebuffer* v14 = GLFramebuffer::Create(1);
        this->m_SystemTarget = v14;
        currentTarget = v14;
    }

    this->m_CurrentTarget = currentTarget;

    GLuint v15 = 0;

    if (currentTarget) {
        v15 = currentTarget->m_FramebufferID;
    }

    if (this->m_States.binding.framebuffer != v15) {
        glBindFramebufferEXT(GL_FRAMEBUFFER, v15);
        this->m_States.binding.framebuffer = v15;
    }
}

void GLSDLDevice::LoadDefaultStates() {
    // Depth

    this->m_States.depth.testEnable = false;
    this->m_States.depth.compareFunc = GL_GEQUAL;
    this->m_States.depth.writeMask = true;

    // Stencil

    this->m_States.stencil.testEnable = false;
    this->m_States.stencil.ref = 0;
    this->m_States.stencil.mask = 0xFFFFFFFF;
    this->m_States.stencil.writeMask = 0xFFFFFFFF;
    this->m_States.stencil.useTwoSidedStencil = false;

    this->m_States.stencil.front.compareFunc = GL_ALWAYS;
    this->m_States.stencil.front.opFail = GL_KEEP;
    this->m_States.stencil.front.opZFail = GL_KEEP;
    this->m_States.stencil.front.opZPass = GL_KEEP;

    this->m_States.stencil.back.compareFunc = GL_ALWAYS;
    this->m_States.stencil.back.opFail = GL_KEEP;
    this->m_States.stencil.back.opZFail = GL_KEEP;
    this->m_States.stencil.back.opZPass = GL_KEEP;

    // Rasterizer

    this->m_States.rasterizer.cullMode = GL_CCW;
    this->m_States.rasterizer.cullFaceMode = GL_BACK;
    this->m_States.rasterizer.fillMode = GL_FILL;
    this->m_States.rasterizer.constantDepthBias = 0.0;
    this->m_States.rasterizer.slopeScaledDepthBias = 0.0;

    this->m_States.rasterizer.viewport = {
        0,
        0,
        this->m_Window->GetWidth(),
        this->m_Window->GetHeight()
    };

    this->m_States.rasterizer.zNear = 0.0;
    this->m_States.rasterizer.zFar = 1.0;
    this->m_States.rasterizer.scissorEnable = false;

    this->m_States.rasterizer.scissor = {
        0,
        0,
        this->m_Window->GetWidth(),
        this->m_Window->GetHeight()
    };

    this->m_States.rasterizer.clipPlaneMask = 0;
    memset(this->m_States.rasterizer.clipPlanes, 0, sizeof(this->m_States.rasterizer.clipPlanes));

    // Blend

    for (int32_t i = 0; i < 4; ++i) {
        this->m_States.blend.colorMask[i] = { true, true, true, true };
    }

    this->m_States.blend.alphaBlend = 0;
    this->m_States.blend.srcBlendFactor = 1;
    this->m_States.blend.destBlendFactor = 0;
    this->m_States.blend.blendOp = GL_FUNC_ADD;
    this->m_States.blend.blendColor = GLColor4f::ZERO;

    // FixedFunc

    this->m_States.fixedFunc.fogColor = { 0.0, 0.0, 0.0, 0.0 };
    this->m_States.fixedFunc.fogStart = 0.0;
    this->m_States.fixedFunc.alphaTestRef = 0.0;
    this->m_States.fixedFunc.fogEnable = 0;
    this->m_States.fixedFunc.fogMode = GL_LINEAR;
    this->m_States.fixedFunc.fogEnd = 1.0;
    this->m_States.fixedFunc.fogDensity = 1.0;
    this->m_States.fixedFunc.alphaTestEnable = 0;
    this->m_States.fixedFunc.alphaTestFunc = GL_ALWAYS;

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.texOp[i].texturing = 0;
        this->m_States.fixedFunc.texOp[i].constant = GLColor4f::ZERO;
        this->m_States.fixedFunc.texOp[i].colorOp = GL_MODULATE;
        this->m_States.fixedFunc.texOp[i].colorScale = 1.0;
        this->m_States.fixedFunc.texOp[i].colorArg0 = GL_TEXTURE;
        this->m_States.fixedFunc.texOp[i].colorArg1 = GL_PREVIOUS;
        this->m_States.fixedFunc.texOp[i].colorArg2 = GL_CONSTANT;
        this->m_States.fixedFunc.texOp[i].alphaOp = GL_MODULATE;
        this->m_States.fixedFunc.texOp[i].alphaScale = 1.0;
        this->m_States.fixedFunc.texOp[i].alphaArg0 = GL_TEXTURE;
        this->m_States.fixedFunc.texOp[i].alphaArg1 = GL_PREVIOUS;
        this->m_States.fixedFunc.texOp[i].alphaArg2 = GL_CONSTANT;
    }

    this->m_States.fixedFunc.lighting.enable = true;
    this->m_States.fixedFunc.lighting.sceneAmbient = { 0.0, 0.0, 0.0, 0.0 };

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.lighting.lights[i].enable = false;
        this->m_States.fixedFunc.lighting.lights[i].position = { 0.0, 0.0, 1.0, 0.0 };
        this->m_States.fixedFunc.lighting.lights[i].view.isDirty = true;
        this->m_States.fixedFunc.lighting.lights[i].view.isIdentity = true;
        this->m_States.fixedFunc.lighting.lights[i].constantAttenuation = 0.0;
        this->m_States.fixedFunc.lighting.lights[i].linearAttenuation = 0.0;
        this->m_States.fixedFunc.lighting.lights[i].quadraticAttenuation = 0.0;
        this->m_States.fixedFunc.lighting.lights[i].ambient = GLColor4f::ZERO;
        this->m_States.fixedFunc.lighting.lights[i].diffuse = GLColor4f::WHITE;
        this->m_States.fixedFunc.lighting.lights[i].specular = GLColor4f::ZERO;
    }

    this->m_States.fixedFunc.lighting.lights[0].enable = true;

    this->m_States.fixedFunc.lighting.material.materialSource = 4609;
    this->m_States.fixedFunc.lighting.material.ambient = GLColor4f::ZERO;
    this->m_States.fixedFunc.lighting.material.diffuse = GLColor4f::WHITE;
    this->m_States.fixedFunc.lighting.material.specular = GLColor4f::ZERO;
    this->m_States.fixedFunc.lighting.material.shininess = 0.0;
    this->m_States.fixedFunc.lighting.material.emission = GLColor4f::ZERO;

    this->m_States.fixedFunc.transforms.matrixMode = 5888;
    this->m_States.fixedFunc.transforms.modelviewStatus = 5888;
    this->m_States.fixedFunc.transforms.modelView.isIdentity = true;
    this->m_States.fixedFunc.transforms.modelView.isDirty = true;
    this->m_States.fixedFunc.transforms.world.isIdentity = true;
    this->m_States.fixedFunc.transforms.world.isDirty = true;
    this->m_States.fixedFunc.transforms.view.isIdentity = true;
    this->m_States.fixedFunc.transforms.view.isDirty = true;
    this->m_States.fixedFunc.transforms.projection.isIdentity = true;
    this->m_States.fixedFunc.transforms.projection.isDirty = true;

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.transforms.texture[i].isIdentity = true;
        this->m_States.fixedFunc.transforms.texture[i].isDirty = true;
    }

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.texCoordIndex[i] = i;
    }

    for (int32_t i = 0; i < 8; ++i) {
        this->m_States.fixedFunc.texGen[i].S = 0;
        this->m_States.fixedFunc.texGen[i].T = 0;
        this->m_States.fixedFunc.texGen[i].R = 0;
        this->m_States.fixedFunc.texGen[i].Q = 0;
    }

    this->m_States.fixedFunc.pointSprite.enable = false;
    this->m_States.fixedFunc.pointSprite.size = 1.0;
    this->m_States.fixedFunc.pointSprite.attenuation[0] = 1.0;
    this->m_States.fixedFunc.pointSprite.attenuation[1] = 0.0;
    this->m_States.fixedFunc.pointSprite.attenuation[2] = 0.0;
    this->m_States.fixedFunc.pointSprite.min = 0.0;
    this->m_States.fixedFunc.pointSprite.max = 1.0;

    this->m_States.fixedFunc.normalizeNormal = false;

    // Samplers

    for (int32_t i = 0; i < 16; ++i) {
        this->m_States.samplers[i].mipmapBias = 0.0;
        this->m_States.samplers[i].addressModeS = 10497;
        this->m_States.samplers[i].addressModeT = 10497;
        this->m_States.samplers[i].addressModeR = 10497;
        this->m_States.samplers[i].magFilterMode = 9729;
        this->m_States.samplers[i].minFilterMode = 9729;
        this->m_States.samplers[i].maxAnisotropy = 1.0;
        this->m_States.samplers[i].borderColor = { 0.0, 0.0, 0.0, 0.0 };
    }

    // Shader

    this->m_States.shader.vertexShaderEnable = false;

    for (int32_t i = 0; i < 256; ++i) {
        this->m_States.shader.vertexShaderConst[i] = { 0.0, 0.0, 0.0, 1.0 };
    }

    this->m_States.shader.pixelShaderEnable = false;

    for (int32_t i = 0; i < 64; ++i) {
        this->m_States.shader.pixelShaderConst[i] = { 0.0, 0.0, 0.0, 1.0 };
    }

    // Binding

    memset(&this->m_States.binding, 0, sizeof(this->m_States.binding));

    // Clear

    this->m_States.clear.clearColor = GLColor4f::ZERO;
    this->m_States.clear.clearDepth = 1.0;
    this->m_States.clear.clearStencil = 0;

    // Misc

    this->m_States.misc.drawBuffers[0] = 0;
    this->m_States.misc.drawBuffers[1] = 0;
    this->m_States.misc.drawBuffers[2] = 0;
    this->m_States.misc.drawBuffers[3] = 0;
    this->m_States.misc.readBuffer = 0;

    // Assign default states
    memcpy(&this->m_DefaultStates.depth, &this->m_States.depth, sizeof(this->m_DefaultStates.depth));
    memcpy(&this->m_DefaultStates.stencil, &this->m_States.stencil, sizeof(this->m_DefaultStates.stencil));
    memcpy(&this->m_DefaultStates.rasterizer, &this->m_States.rasterizer, sizeof(this->m_DefaultStates.rasterizer));
    memcpy(&this->m_DefaultStates.blend, &this->m_States.blend, sizeof(this->m_DefaultStates.blend));
    memcpy(&this->m_DefaultStates.clear, &this->m_States.clear, sizeof(this->m_DefaultStates.clear));
    memcpy(&this->m_DefaultStates.fixedFunc, &this->m_States.fixedFunc, sizeof(this->m_DefaultStates.fixedFunc));
    memcpy(this->m_DefaultStates.samplers, this->m_States.samplers, sizeof(this->m_DefaultStates.samplers));
    memcpy(&this->m_DefaultStates.shader, &this->m_States.shader, sizeof(this->m_DefaultStates.shader));
    memcpy(&this->m_DefaultStates.binding, &this->m_States.binding, sizeof(this->m_DefaultStates.binding));
    memcpy(&this->m_DefaultStates.misc, &this->m_States.misc, sizeof(this->m_DefaultStates.misc));

    // TODO
    // if (v54 != this->m_States.misc.unpackClientStorage) {
    //     glPixelStorei(34226, v54);
    //     this->m_States.misc.unpackClientStorage = v54;
    // }

    this->ApplyGLBindings(this->m_States, 1);
    this->ApplyGLStates(this->m_States, 1);
}

void GLSDLDevice::ResetBackbuffer(uint32_t width, uint32_t height, GLTextureFormat colorFormat, GLTextureFormat depthFormat, uint32_t sampleCount) {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());

    if (
        this->m_BackBufferColor
        && this->m_BackBufferColor->m_Width == width
        && this->m_BackBufferColor->m_Height == height
        && this->m_BackBufferColor->m_Format == colorFormat
        && this->m_BackBufferDepth
        && this->m_BackBufferDepth->m_Format == depthFormat
    ) {
        if (this->m_UseWindowSystemBuffer && this->m_CurrentTarget->GetSampleCount() != sampleCount) {
            // this->m_Context.SetContextFormat(depthFormat, sampleCount);
        }

        return;
    }

    auto v24 = this->m_BackBufferColor != 0;

    if (this->m_BackBufferColor) {
        this->m_BackBufferColor->Release();
        this->m_BackBufferColor = nullptr;
    }

    if (this->m_BackBufferStencil) {
        this->m_BackBufferStencil->Release();
        this->m_BackBufferStencil = nullptr;
    }

    if (this->m_BackBufferDepth) {
        this->m_BackBufferDepth->Release();
        this->m_BackBufferDepth = nullptr;
    }

    GLTextureFormat v10 = this->m_UseWindowSystemBuffer ? depthFormat : GLTF_INVALID;
    // this->m_Context.SetContextFormat(v10, sampleCount);

    this->BindFramebuffer(
        this->m_UseWindowSystemBuffer
            ? this->m_SystemTarget
            : this->m_FBOTarget
    );

    if (this->m_UseWindowSystemBuffer && !v24) {
        this->Clear(0x4500, GLColor4f::BLACK, 1.0, 0);
        this->m_Window->Swap();
    }

    uint32_t v13 = GLTFLAG_SYSTEM_BUFFER;
    if (!this->m_UseWindowSystemBuffer) {
        v13 = 0;
    }

    uint32_t v14 = GLTFLAG_RENDERTARGET;
    if (!this->m_FlippedSystemBuffer) {
        v14 = v13 | GLTFLAG_RENDERTARGET;
    }

    auto v15 = GLTexture2D::Create(width, height, 1, colorFormat, v14);
    this->m_BackBufferColor = v15;
    auto v16 = v15->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    this->m_CurrentTargetColor[0] = v16;
    this->m_CurrentTarget->Attach(v16, GL_COLOR_ATTACHMENT0, 0);

    GLRect v29 = { 0, 0, static_cast<int32_t>(width), static_cast<int32_t>(height) };
    this->SetViewport(v29, 0.0, 1.0);

    if (this->m_FlippedSystemBuffer) {
        auto v19 = this->m_BackBufferColor->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        GLRect v25 = { 0, 0, v19->m_Width, v19->m_Height };
        this->CopyTex(0, 0, v19, &v25);
    }

    if (depthFormat) {
        if (depthFormat == GLTF_D24S8) {
            auto v20 = GLTexture2D::Create(width, height, 1, GLTF_D24S8, v13 | GLTFLAG_RENDERTARGET | GLTFLAG_DEPTH | GLTFLAG_STENCIL);
            this->m_BackBufferDepth = v20;
            this->m_BackBufferStencil = v20;

            // TODO how to properly increment GLObject ref counts?
            v20->AddRefTwin();
            v20->m_RefCount++;

            auto v22 = this->m_BackBufferStencil->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
            this->m_CurrentTargetStencil = v22;
            this->m_CurrentTarget->Attach(v22, GL_STENCIL_ATTACHMENT, 0);
        } else {
            this->m_BackBufferDepth = GLTexture2D::Create(width, height, 1, depthFormat, v13 | GLTFLAG_RENDERTARGET | GLTFLAG_DEPTH);
        }

        auto v17 = this->m_BackBufferDepth->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        this->m_CurrentTargetDepth = v17;
        this->m_CurrentTarget->Attach(v17, GL_DEPTH_ATTACHMENT, 0);
        this->m_CurrentDepthBuffer = this->m_CurrentTargetDepth;
        this->SetDepthTestEnable(1);
    }

    BLIZZARD_ASSERT(this->m_CurrentTarget->IsValid());

    this->Clear(0x4500, GLColor4f::BLACK, 1.0, 0);
}

void GLSDLDevice::Resize(uint32_t width, uint32_t height) {
    auto colorFormat = this->m_BackBufferColor ? this->m_BackBufferColor->m_Format : GLTF_INVALID;
    auto depthFormat = this->m_BackBufferDepth ? this->m_BackBufferDepth->m_Format : GLTF_INVALID;

    this->SetDisplay(
        width,
        height,
        colorFormat,
        depthFormat,
        0,
        false,
        false,
        this->m_Context.GetSampleCount()
    );
}

void GLSDLDevice::RestoreTextures() {
    BLIZZARD_ASSERT(this->m_Context.IsCurrentContext());

    for (int32_t i = 0; i < 16; i++) {
        GLTexture* texture = this->m_Textures[i];

        if (texture && !texture->IsValid()) {
            this->m_Textures[i] = nullptr;
        }

        this->SetTexture(i, this->m_Textures[i]);
    }
}

void GLSDLDevice::SetActiveTexture(uint32_t a2) {
    if (this->m_States.binding.currentActiveTexture != a2) {
        glActiveTexture(GL_TEXTURE0 + a2);
        this->m_States.binding.currentActiveTexture = a2;
    }
}

void GLSDLDevice::SetAlphaBlend(GLEnum srcBlend, GLEnum dstBlend, GLEnum blendOp) {
    if (this->m_States.blend.srcBlendFactor != srcBlend || this->m_States.blend.destBlendFactor != dstBlend) {
        glBlendFunc(srcBlend, dstBlend);
        this->m_States.blend.srcBlendFactor = srcBlend;
        this->m_States.blend.destBlendFactor = dstBlend;
    }

    if (this->m_States.blend.blendOp != blendOp) {
        glBlendEquation(blendOp);
        this->m_States.blend.blendOp = blendOp;
    }
}

void GLSDLDevice::SetAlphaBlendEnable(bool enable) {
    if (this->m_States.blend.alphaBlend != enable) {
        if (enable) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }

        this->m_States.blend.alphaBlend = enable;
    }
}

void GLSDLDevice::SetAlphaTest(GLEnum func, float ref) {
    if (this->m_States.fixedFunc.alphaTestFunc != func || this->m_States.fixedFunc.alphaTestRef != ref) {
        BLIZZARD_ASSERT(ref <= 1.0f);

        glAlphaFunc(func, ref);
        this->m_States.fixedFunc.alphaTestFunc = func;
        this->m_States.fixedFunc.alphaTestRef = ref;
    }
}

void GLSDLDevice::SetAlphaTestEnable(bool enable) {
    if (this->m_States.fixedFunc.alphaTestEnable != enable) {
        if (enable) {
            glEnable(GL_ALPHA_TEST);
        } else {
            glDisable(GL_ALPHA_TEST);
        }
    }
}

void GLSDLDevice::SetClearColor(const GLColor4f& clearColor) {
    if (
        this->m_States.clear.clearColor.r != clearColor.r
        || this->m_States.clear.clearColor.g != clearColor.g
        || this->m_States.clear.clearColor.b != clearColor.b
        || this->m_States.clear.clearColor.a != clearColor.a
    ) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        this->m_States.clear.clearColor = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
    }
}

void GLSDLDevice::SetClearDepth(double clearDepth) {
    if (this->m_States.clear.clearDepth != clearDepth) {
        glClearDepth(clearDepth);
        this->m_States.clear.clearDepth = clearDepth;
    }
}

void GLSDLDevice::SetClearStencil(int32_t clearStencil) {
    if (this->m_States.clear.clearStencil != clearStencil) {
        glClearStencil(clearStencil);
        this->m_States.clear.clearStencil = clearStencil;
    }
}

void GLSDLDevice::SetColorWriteMask(bool red, bool green, bool blue, bool alpha, uint32_t index) {
    if (
        this->m_States.blend.colorMask[index].red != red
        || this->m_States.blend.colorMask[index].green != green
        || this->m_States.blend.colorMask[index].blue != blue
        || this->m_States.blend.colorMask[index].alpha != alpha
    ) {
        if (GLSDLDevice::m_ExtColorMaskIndexed) {
            glColorMaskIndexedEXT(index, red, green, blue, alpha);
        } else if (index == 0) {
            glColorMask(red, green, blue, alpha);
        }

        this->m_States.blend.colorMask[index].red = red;
        this->m_States.blend.colorMask[index].green = green;
        this->m_States.blend.colorMask[index].blue = blue;
        this->m_States.blend.colorMask[index].alpha = alpha;
    }
}

void GLSDLDevice::SetCullMode(GLEnum cullMode) {
    if (this->m_States.rasterizer.cullMode != cullMode) {
        if (cullMode) {
            if (cullMode - GL_CW <= 1) {
                glEnable(GL_CULL_FACE);
                glFrontFace(cullMode);
            }

            this->m_States.rasterizer.cullMode = cullMode;
        } else {
            glDisable(GL_CULL_FACE);
            this->m_States.rasterizer.cullMode = 0;
        }
    }
}

void GLSDLDevice::SetDepthBias(float constantBias, float slopeScaledBias) {
    // TODO
}

void GLSDLDevice::SetDepthTestEnable(bool enable) {
    if (this->m_States.depth.testEnable != enable) {
        if (enable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        this->m_States.depth.testEnable = enable;
    }
}

void GLSDLDevice::SetDepthTestFunc(GLEnum func) {
    if (this->m_States.depth.compareFunc != func) {
        glDepthFunc(func);
        this->m_States.depth.compareFunc = func;
    }
}

void GLSDLDevice::SetDepthWriteMask(bool enable) {
    if (this->m_States.depth.writeMask != enable) {
        glDepthMask(enable);
        this->m_States.depth.writeMask = enable;
    }
}

void GLSDLDevice::SetDisplay(uint32_t width, uint32_t height, GLTextureFormat colorFormat, GLTextureFormat depthFormat, uint32_t refreshRate, bool windowed, bool captureDisplay, uint32_t sampleCount) {
    // uint32_t v9 = a9;

    // if (this->m_PBOPool) {
    //     // TODO
    //     // this->m_PBOPool->Flush(1);
    // }

    // if (a9 > 7) {
    //     uint32_t v10;

    //     if (a7) {
    //         // CGDirectDisplayID mainDisplay = CGMainDisplayID();
    //         // CGRect bounds = CGDisplayBounds(mainDisplay);
    //         // CGFloat boundsWidth = CGRectGetWidth(bounds);
    //         // CGFloat boundsHeight = CGRectGetHeight(bounds);

    //         // v10 = std::floor(boundsWidth) * std::floor(boundsHeight);
    //     } else {
    //         v10 = width * height;
    //     }

    //     if (v10 >= 2304001) {
    //         v9 = 6;
    //     }
    // }

    // if (this->m_Context.m_Window) {
    //     // TODO
    //     // - callback related... set to default callbacks?
    //     // (this->m_Context.m_Window + 84)();
    // }

    // if (a7) {
    //     if (this->m_Context.m_Window) {
    //         this->m_Context.m_Window->Resize(width, height);
    //     }

    //     this->m_Context.SetWindow(this->m_Context.m_Window, 0);
    // } else {
    //     this->m_Context.SetFullscreenMode(width, height, a6, a8);
    // }

    GLSDLWindowRect newRect;
    newRect.size.width = width;
    newRect.size.height = height;

    this->m_Window->Resize(newRect);

    this->ResetBackbuffer(width, height, colorFormat, depthFormat, sampleCount);

    // if (this->m_Context.m_Window) {
    //     // TODO
    //     // - set active callbacks to callbacks
    //     // (this->m_Context.m_Window + 88)();
    // }
}

void GLSDLDevice::SetFogColor(float r, float g, float b, float a) {
    if (
        this->m_States.fixedFunc.fogColor.r != r
        || this->m_States.fixedFunc.fogColor.g != g
        || this->m_States.fixedFunc.fogColor.b != b
        || this->m_States.fixedFunc.fogColor.a != a
    ) {
        this->m_States.fixedFunc.fogColor.r = r;
        this->m_States.fixedFunc.fogColor.g = g;
        this->m_States.fixedFunc.fogColor.b = b;
        this->m_States.fixedFunc.fogColor.a = a;

        glFogfv(GL_FOG_COLOR, reinterpret_cast<GLfloat*>(&this->m_States.fixedFunc.fogColor));

        // TODO logic related to a renderer info value
    }
}

void GLSDLDevice::SetFogEnable(bool enable) {
    if (this->m_States.fixedFunc.fogEnable != enable) {
        if (enable) {
            glEnable(GL_FOG);
        } else {
            glDisable(GL_FOG);
        }

        this->m_States.fixedFunc.fogEnable = enable;
    }
}

void GLSDLDevice::SetFogParam(GLEnum param, float value) {
    if (param == GL_FOG_START) {
        if (this->m_States.fixedFunc.fogStart != value)  {
            glFogf(GL_FOG_START, value);
            this->m_States.fixedFunc.fogStart = value;
        }
    } else if (param == GL_FOG_END) {
        if (this->m_States.fixedFunc.fogEnd != value)  {
            glFogf(GL_FOG_END, value);
            this->m_States.fixedFunc.fogEnd = value;
        }
    } else if (param == GL_FOG_DENSITY) {
        if (this->m_States.fixedFunc.fogDensity != value)  {
            glFogf(GL_FOG_DENSITY, value);
            this->m_States.fixedFunc.fogDensity = value;
        }
    } else {
        BLIZZARD_ASSERT(false);
    }
}

void GLSDLDevice::SetIndexBuffer(GLBuffer* buffer) {
    BLIZZARD_ASSERT(buffer == nullptr || buffer->m_IndexFormat != GL_ZERO);
    this->m_DefaultVertexArrayObject.m_Properties.m_IndexBuffer = buffer;
}

void GLSDLDevice::SetLightingEnable(bool enable) {
    if (this->m_States.fixedFunc.lighting.enable != enable) {
        if (enable) {
            glEnable(GL_LIGHTING);
        } else {
            glDisable(GL_LIGHTING);
        }

        this->m_States.fixedFunc.lighting.enable = enable;
    }
}

void GLSDLDevice::SetModelView(GLEnum transform) {
    if (transform == 'VIEW') {
        // TODO
        return;
    }

    if (transform == 'WRLD') {
        // TODO
        return;
    }

    if (transform != GL_MODELVIEW) {
        BLIZZARD_ASSERT(false);
    }

    auto& world = this->m_States.fixedFunc.transforms.world;
    auto& view = this->m_States.fixedFunc.transforms.view;
    auto& modelView = this->m_States.fixedFunc.transforms.modelView;

    if (this->m_States.fixedFunc.transforms.modelviewStatus != transform || modelView.isDirty) {
        if (world.isIdentity && view.isIdentity) {
            modelView.isIdentity = true;
            modelView.isDirty = true;
        } else if (world.isIdentity) {
            modelView = view;
            modelView.isIdentity = false;
            modelView.isDirty = true;
        } else if (view.isIdentity) {
            modelView = world;
            modelView.isIdentity = false;
            modelView.isDirty = true;
        } else {
            // TODO assign model * view to modelView
            BLIZZARD_ASSERT(!"Unimplemented");
        }

        if (this->m_States.fixedFunc.transforms.matrixMode != GL_MODELVIEW) {
            glMatrixMode(GL_MODELVIEW);
            this->m_States.fixedFunc.transforms.matrixMode = GL_MODELVIEW;
        }

        if (modelView.isIdentity) {
            glLoadIdentity();
        } else {
            glLoadMatrixf(modelView.m);
        }

        this->m_States.fixedFunc.transforms.modelviewStatus = transform;
    }
}

void GLSDLDevice::SetScissor(bool a2, const GLRect& a3) {
    // TODO
}

void GLSDLDevice::SetShader(GLShader::ShaderType shaderType, GLShader* shader) {
    if (shader) {
        if (shader->var18) {
            // TODO
        }

        BLIZZARD_ASSERT(shader->IsEnabled());
        BLIZZARD_ASSERT(shader->GetShaderType() == shaderType);

        this->BindShader(shader);
    }

    int32_t enable = shader != nullptr;

    if (shaderType == GLShader::eVertexShader) {
        if (this->m_States.shader.vertexShaderEnable != enable) {
            if (enable) {
                glEnable(GL_VERTEX_PROGRAM_ARB);
            } else {
                glDisable(GL_VERTEX_PROGRAM_ARB);
            }

            this->m_States.shader.vertexShaderEnable = enable;
        }

        this->m_VertexShader = shader;
    } else if (shaderType == GLShader::ePixelShader) {
        if (this->m_States.shader.pixelShaderEnable != enable) {
            if (enable) {
                glEnable(GL_FRAGMENT_PROGRAM_ARB);
            } else {
                glDisable(GL_FRAGMENT_PROGRAM_ARB);
            }

            this->m_States.shader.pixelShaderEnable = enable;
        }

        this->m_PixelShader = shader;
    } else {
        BLIZZARD_ASSERT(!"Unknown shader type!");
    }
}

void GLSDLDevice::SetShaderConstants(GLShader::ShaderType shaderType, uint32_t index, const float* constants, uint32_t count) {
    BLIZZARD_ASSERT(count != 0);

    GLShader* shader = nullptr;

    if (shaderType == GLShader::eVertexShader) {
        shader = this->m_VertexShader;
    } else if (shaderType == GLShader::ePixelShader) {
        shader = this->m_PixelShader;
    }

    if (GLSDLDevice::m_ShaderConstantBindings) {
        if (!shader) {
            return;
        }

        if (shader && shader->var10) {
            shader->SetShaderConstants(shaderType, index, constants, count);
            return;
        }
    }

    this->SetShaderConstantsInternal(shaderType, index, constants, count);
}

void GLSDLDevice::SetShaderConstantsInternal(GLShader::ShaderType shaderType, uint32_t index, const float* constants, uint32_t count) {
    if (shaderType == GLShader::eVertexShader) {
        BLIZZARD_ASSERT((index + count) <= std::extent<decltype(this->m_States.shader.vertexShaderConst)>::value);

        memcpy(&this->m_States.shader.vertexShaderConst[index], constants, (sizeof(float) * 4) * count);

        BLIZZARD_ASSERT(index <= 0xFFFF);
        BLIZZARD_ASSERT(count <= 0xFFFF);

        uint16_t start = std::min(static_cast<uint16_t>(index), this->m_DirtyVertexShaderConsts.start);
        uint16_t end = std::max(static_cast<uint16_t>(index + count), this->m_DirtyVertexShaderConsts.end);

        this->m_DirtyVertexShaderConsts.start = start;
        this->m_DirtyVertexShaderConsts.end = end;
    } else if (shaderType == GLShader::ePixelShader) {
        BLIZZARD_ASSERT((index + count) <= std::extent<decltype(this->m_States.shader.pixelShaderConst)>::value);

        memcpy(&this->m_States.shader.pixelShaderConst[index], constants, (sizeof(float) * 4) * count);

        BLIZZARD_ASSERT(index <= 0xFFFF);
        BLIZZARD_ASSERT(count <= 0xFFFF);

        uint16_t start = std::min(static_cast<uint16_t>(index), this->m_DirtyPixelShaderConsts.start);
        uint16_t end = std::max(static_cast<uint16_t>(index + count), this->m_DirtyPixelShaderConsts.end);

        this->m_DirtyPixelShaderConsts.start = start;
        this->m_DirtyPixelShaderConsts.end = end;
    } else {
        // TODO
        // BLIZZARD_ASSERT(false, "Unknown shader type %d!", shaderType);
    }
}

void GLSDLDevice::SetTexture(uint32_t stage, GLTexture* texture) {
    if (stage > 15) {
        BLIZZARD_ASSERT(!"setting an unsupported texture stage to a non-NULL texture");
    }

    BLIZZARD_ASSERT(texture == nullptr || texture->IsValid());

    uint32_t textureID = 0;
    GLEnum textureType = GL_TEXTURE_2D;

    if (this->m_Textures[stage]) {
        textureType = this->m_Textures[stage]->m_TextureType;
    }

    if (texture) {
        textureID = texture->m_TextureID;
        textureType = texture->m_TextureType;
    }

    uint32_t index = GLSDLTextureTypeToIndex(textureType);

    if (this->m_States.binding.texture[index][stage] != textureID) {
        this->SetActiveTexture(stage);

        if (texture) {
            texture->Bind(this, 1);
        } else {
            this->BindTexture(textureType, nullptr);
        }
    }

    this->m_Textures[stage] = texture;
}

void GLSDLDevice::SetTransform(GLEnum transform, const float* a3) {
    GLTransform* t;

    if (transform == 'VIEW') {
        t = &this->m_States.fixedFunc.transforms.view;
    } else if (transform == 'WRLD') {
        t = &this->m_States.fixedFunc.transforms.world;
    } else if (transform == GL_PROJECTION) {
        t = &this->m_States.fixedFunc.transforms.projection;
    } else if (transform >= GL_TEXTURE0 && transform <= GL_TEXTURE7) {
        t = &this->m_States.fixedFunc.transforms.texture[transform - GL_TEXTURE0];
    } else {
        BLIZZARD_ASSERT(false);
    }

    if (*t != a3) {
        t->Set(a3);
    }

    if (t->isDirty) {
        if (transform == 'VIEW' || transform == 'WRLD') {
            this->m_States.fixedFunc.transforms.modelView.isDirty = true;
        }
    }
}

void GLSDLDevice::SetUnpackClientStorage(bool enable) {
    // TODO
    // Blizzard::Debug::Assert(!this->IsUsingPBO() || enable == GL_FALSE);

    if (this->m_States.misc.unpackClientStorage != enable) {
        // glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, enable);
        this->m_States.misc.unpackClientStorage = enable;
    }
}

void GLSDLDevice::SetVertexBuffer(uint32_t index, GLBuffer* buffer, uint32_t offset, uint32_t stride) {
    BLIZZARD_ASSERT(index < GL_MAX_STREAM);
    BLIZZARD_ASSERT(buffer == nullptr || buffer->m_IndexFormat == GL_ZERO);

    auto properties = &this->m_DefaultVertexArrayObject.m_Properties;
    properties->m_VertexBuffer[index] = buffer;
    properties->m_VertexBufferOffset[index] = offset;
    properties->m_VertexBufferStride[index] = stride;
}

void GLSDLDevice::SetVertexFormat(GLVertexFormat* format) {
    BLIZZARD_ASSERT(format->m_Size <= kMAX_VERTEX_ATTRIBS);
    this->m_DefaultVertexArrayObject.m_Properties.m_VertexBufferFormat = format;
}

void GLSDLDevice::SetViewport(const GLRect& viewport, double zNear, double zFar) {
    if (
        this->m_States.rasterizer.viewport.left != viewport.left
        || this->m_States.rasterizer.viewport.top != viewport.top
        || this->m_States.rasterizer.viewport.width != viewport.width
        || this->m_States.rasterizer.viewport.height != viewport.height
    ) {
        glViewport(viewport.left, viewport.top, viewport.width, viewport.height);
        this->m_States.rasterizer.viewport = viewport;
    }

    if (
        this->m_States.rasterizer.zNear != zNear
        || this->m_States.rasterizer.zFar != zFar
    ) {
        glDepthRange(zNear, zFar);
        this->m_States.rasterizer.zNear = zNear;
        this->m_States.rasterizer.zFar = zFar;
    }
}

void GLSDLDevice::Sub34BB0(GLEnum a2, GLMipmap* a3, uint32_t index) {
    if (!a3 || !this->m_UseWindowSystemBuffer) {
        return;
    }

    GLFramebuffer* target;

    if (a3->GetTexture() == this->m_BackBufferColor || a3->GetTexture() == this->m_BackBufferDepth || a3->GetTexture() == this->m_BackBufferStencil) {
        if (this->m_CurrentTarget == this->m_SystemTarget) {
            return;
        }

        switch (a2) {
            case GL_DEPTH_ATTACHMENT:
                this->Sub34BB0(GL_DEPTH_ATTACHMENT, nullptr, 0);
                this->m_CurrentTarget->Attach(nullptr, GL_DEPTH_ATTACHMENT, 0);
                this->m_CurrentTargetDepth = nullptr;
                this->m_CurrentDepthBuffer = nullptr;
                break;

            case GL_STENCIL_ATTACHMENT:
                this->Sub34BB0(GL_STENCIL_ATTACHMENT, nullptr, 0);
                this->m_CurrentTarget->Attach(nullptr, GL_STENCIL_ATTACHMENT, 0);
                this->m_CurrentTargetStencil = nullptr;
                break;

            case GL_COLOR_ATTACHMENT0:
                // TODO this->SetColorTarget(0, index);
                break;
        }

        target = this->m_SystemTarget;
    } else {
        target = this->m_FBOTarget;
    }

    this->BindFramebuffer(target);
}

void GLSDLDevice::Sub38460(bool a2) {
    if (this->m_States.binding.framebuffer == 0) {
        glDrawBuffer(GL_BACK);
        this->m_States.misc.drawBuffers[0] = GL_BACK;

        glReadBuffer(GL_BACK);
        this->m_States.misc.readBuffer = GL_BACK;

        return;
    }

    if (a2) {
        // TODO

        return;
    }

    // TODO
}

void GLSDLDevice::Swap() {
    // if (this->m_Context.m_Window) {
    //     if (this->m_FlippedSystemBuffer) {
    //         GLRect rect = {
    //             0,
    //             0,
    //             static_cast<int32_t>(this->m_BackBufferColor->m_Width),
    //             static_cast<int32_t>(this->m_BackBufferColor->m_Height)
    //         };

    //         GLMipmap* image = this->m_BackBufferColor->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    //         this->CopyTex(0, 0, image, &rect);
    //     }

    //     this->DrawRect();

    //     this->m_FrameNumber++;
    //     this->m_DrawCount = 0;
    // } else {
    //     // glFlushRender();
    //     this->m_DrawCount = 0;
    // }

    this->DrawRect();
    this->m_FrameNumber++;
    this->m_DrawCount = 0;
}

void GLSDLDevice::UpdateFFPTexturing() {
    // TODO
}
