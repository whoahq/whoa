#include "gx/gll/GLCommand.h"
#include "gx/gll/GLTexture.h"

void GLFlush::Execute(GLDevice* device) {
    glFlush();
}

GLTexUnmap::GLTexUnmap(GLTexture* texture, GLMipmap* mipmap, GLMipmap::MapParams* mapParams) {
    this->m_Texture = texture;
    this->m_Mipmap = mipmap;
    this->m_MapParams = mapParams;
}

void GLTexUnmap::Execute(GLDevice* device) {
    this->m_Mipmap->Unmap(this->m_MapParams);
}

GLTexDestroy::GLTexDestroy(GLTexture* texture) {
    this->m_Texture = texture;
}

void GLTexDestroy::Execute(GLDevice* device) {
    this->m_Texture->FreeTexture();
}
