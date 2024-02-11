#ifndef GX_GL_SDL_GL_TYPES_HPP
#define GX_GL_SDL_GL_TYPES_HPP

#include <cstdint>

#include <GL/glew.h>

class GLBuffer;
class GLFramebuffer;

enum GLTextureFormat {
    GLTF_INVALID = 0,
    GLTF_ARGB8888 = 1,
    GLTF_XRGB8888 = 2,
    GLTF_RGBA8888 = 3,
    GLTF_ABGR8888 = 4,
    GLTF_ARGB0888 = 5,
    GLTF_RGB888 = 6,
    GLTF_BGR888 = 7,
    GLTF_RGBA32F = 8,
    GLTF_RGBA16F = 9,
    GLTF_RG16F = 10,
    GLTF_D32 = 11,
    GLTF_D24 = 12,
    GLTF_D16 = 13,
    GLTF_DF = 14,
    GLTF_D24S8 = 15,
    GLTF_S8 = 16,
    GLTF_ARGB4444 = 17,
    GLTF_ARGB1555 = 18,
    GLTF_ARGB0555 = 19,
    GLTF_RGB565 = 20,
    GLTF_A2RGB10 = 21,
    GLTF_RGB16 = 22,
    GLTF_L8 = 23,
    GLTF_A8 = 24,
    GLTF_A8L8 = 25,
    GLTF_DXT1 = 26,
    GLTF_DXT3 = 27,
    GLTF_DXT5 = 28,
    GLTF_NUM_TEXTURE_FORMATS = 29
};

enum GLVertexType {
    GLVT_INVALID = 0,
    GLVT_FLOAT1 = 1,
    GLVT_FLOAT2 = 2,
    GLVT_FLOAT3 = 3,
    GLVT_FLOAT4 = 4,
    GLVT_UBYTE4 = 5,
    GLVT_UBYTE4N = 6,
    GLVT_SHORT = 7,
    GLVT_SHORT2 = 8,
    GLVT_SHORT4 = 9,
    GLVT_SHORT2N = 10,
    GLVT_SHORT4N = 11,
    GLVT_USHORT2N = 12,
    GLVT_USHORT4N = 13,
    GLVT_NUM_VERTEX_TYPES = 14
};

enum GLBufferType {
    eGLBT_PIXEL_UNPACK = 3,
};

struct GLAttachPoint {
    GLFramebuffer* framebuffer;
    int32_t point; // TODO GLenum?
    int32_t zOffset; // TODO check type
};

struct GLBox {
    int32_t left;
    int32_t top;
    int32_t front;
    int32_t width;
    int32_t height;
    int32_t depth;
};

struct GLColor4f {
    float r;
    float g;
    float b;
    float a;

    static GLColor4f ZERO;
    static GLColor4f WHITE;
    static GLColor4f BLACK;
};

struct GLDirtyRange {
    uint16_t start;
    uint16_t end;
};

struct GLfloat4 {
    float x;
    float y;
    float z;
    float w;
};

struct GLRect {
    int32_t left;
    int32_t top;
    int32_t width;
    int32_t height;
};

struct GLTransform {
    bool isDirty;

    union {
        struct {
            float a0;
            float a1;
            float a2;
            float a3;
            float b0;
            float b1;
            float b2;
            float b3;
            float c0;
            float c1;
            float c2;
            float c3;
            float d0;
            float d1;
            float d2;
            float d3;
        };

        struct {
            float rows[4][4];
        };

        float m[16];
    };

    bool isIdentity;

    bool operator==(const float m[16]) const;
    bool operator!=(const float m[16]) const;
    void Set(const float m[16]);
    void SetIdentity();
};

struct GLStates {
    struct Depth {
        bool testEnable;
        int32_t compareFunc;
        bool writeMask;
    };

    struct Stencil {
        struct StencilFace {
            int32_t compareFunc;
            int32_t opFail;
            int32_t opZFail;
            int32_t opZPass;
        };

        bool testEnable;
        int32_t ref;
        uint32_t mask;
        uint32_t writeMask;
        bool useTwoSidedStencil;
        StencilFace front;
        StencilFace back;
    };

    struct Rasterizer {
        struct ClipPlane {
            double plane[4];
        };

        int32_t cullMode;
        int32_t cullFaceMode;
        int32_t fillMode;
        float constantDepthBias;
        float slopeScaledDepthBias;
        GLRect viewport;
        double zNear;
        double zFar;
        bool scissorEnable;
        GLRect scissor;
        uint32_t clipPlaneMask;
        ClipPlane clipPlanes[6];
    };

    struct Blend {
        struct ColorMask {
            bool red;
            bool green;
            bool blue;
            bool alpha;
        };

        ColorMask colorMask[4];
        bool alphaBlend;
        int32_t srcBlendFactor;
        int32_t destBlendFactor;
        int32_t blendOp;
        GLColor4f blendColor;
    };

    struct Clear {
        GLColor4f clearColor;
        double clearDepth;
        int32_t clearStencil;
    };

    struct FixedFunc {
        struct TexOp {
            int32_t texturing;
            GLColor4f constant;
            int32_t colorOp;
            float colorScale;
            int32_t colorArg0;
            int32_t colorArg1;
            int32_t colorArg2;
            int32_t alphaOp;
            float alphaScale;
            int32_t alphaArg0;
            int32_t alphaArg1;
            int32_t alphaArg2;
        };

        struct Light {
            bool enable;
            GLfloat4 position;
            GLTransform view;
            float constantAttenuation;
            float linearAttenuation;
            float quadraticAttenuation;
            GLColor4f ambient;
            GLColor4f diffuse;
            GLColor4f specular;
        };

        struct Material {
            bool colorTracking;
            int32_t materialSource;
            GLColor4f ambient;
            GLColor4f diffuse;
            GLColor4f specular;
            float shininess;
            GLColor4f emission;
        };

        struct Lighting {
            bool enable;
            GLColor4f sceneAmbient;
            Light lights[8];
            Material material;
        };

        struct Transforms {
            int32_t matrixMode;
            int32_t modelviewStatus;
            GLTransform modelView;
            GLTransform world;
            GLTransform view;
            GLTransform projection;
            GLTransform texture[8];
        };

        struct TexGen {
            int32_t S;
            int32_t T;
            int32_t R;
            int32_t Q;
        };

        struct PointSprite {
            bool enable;
            float size;
            float attenuation[3];
            float min;
            float max;
        };

        bool fogEnable;
        GLColor4f fogColor;
        int32_t fogMode;
        float fogStart;
        float fogEnd;
        float fogDensity;
        bool alphaTestEnable;
        int32_t alphaTestFunc;
        float alphaTestRef;
        TexOp texOp[8];
        Lighting lighting;
        Transforms transforms;
        int32_t texCoordIndex[8];
        TexGen texGen[8];
        PointSprite pointSprite;
        bool normalizeNormal;
    };

    struct Sampler {
        float mipmapBias;
        int32_t addressModeS;
        int32_t addressModeT;
        int32_t addressModeR;
        int32_t magFilterMode;
        int32_t minFilterMode;
        float maxAnisotropy;
        GLColor4f borderColor;
    };

    struct Shader {
        bool vertexShaderEnable;
        GLfloat4 vertexShaderConst[256];
        bool pixelShaderEnable;
        GLfloat4 pixelShaderConst[64];
    };

    struct Binding {
        uint32_t currentActiveTexture;
        uint32_t texture[4][16];
        uint32_t framebuffer;
        uint32_t renderbuffer;
        uint32_t vertexProgram;
        uint32_t pixelProgram;
        uint32_t glslProgram;
        uint32_t vertexArrayObject;
    };

    struct Misc {
        bool unpackClientStorage;
        int32_t drawBuffers[4];
        int32_t readBuffer;
    };

    struct VertexArrayObject {
        struct VertexAttrib {
            bool enable = 0;
            uint32_t size = 4;
            int32_t type = GL_FLOAT;
            bool normalized = 0;
            uint32_t stride = 0;
            void* offset = nullptr;
            GLBuffer* buffer = nullptr;
        };

        uint32_t buffers[4] = {};
        VertexAttrib vertexAttribs[16];
        VertexAttrib position;
        VertexAttrib normal;
        VertexAttrib color0;
        VertexAttrib color1;
        VertexAttrib texCoord[8];
    };

    Depth depth;
    Stencil stencil;
    Rasterizer rasterizer;
    Blend blend;
    Clear clear;
    FixedFunc fixedFunc;
    Sampler samplers[16];
    Shader shader;
    Binding binding;
    Misc misc;
};

struct GLVertexAttrib {
    uint32_t stream;
    int32_t slot;
    int32_t type;
    uint32_t offset;
};

#endif
