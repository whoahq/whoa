#ifndef GX_GLL_GL_COMMAND_H
#define GX_GLL_GL_COMMAND_H

#include "gx/gll/GLMipmap.h"

class GLDevice;

class GLCommand {
    public:
        // Virtual member functions
        virtual void Execute(GLDevice*) = 0;
};

class GLFlush : public GLCommand {
    public:
        // Virtual member functions
        virtual void Execute(GLDevice* device);
};

class GLTexUnmap : public GLCommand {
    public:
        // Member variables
        GLTexture* m_Texture;
        GLMipmap* m_Mipmap;
        GLMipmap::MapParams* m_MapParams;

        // Virtual member functions
        virtual void Execute(GLDevice*);

        // Member functions
        GLTexUnmap(GLTexture*, GLMipmap*, GLMipmap::MapParams*);
};

class GLTexDestroy : public GLCommand {
    public:
        // Member variables
        GLTexture* m_Texture;

        // Virtual member functions
        virtual void Execute(GLDevice* device);

        // Member functions
        GLTexDestroy(GLTexture* texture);
};

#endif
