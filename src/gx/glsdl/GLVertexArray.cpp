#include "gx/glsdl/GLVertexArray.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include <bc/Debug.hpp>

bool GLVertexArray::s_VertexArrayEnable = false;

GLVertexArray::GLVertexArray(bool a2) {
    // TODO
}

void GLVertexArray::FindVertexArray(GLSDLDevice* a1, GLVertexArray& a2) {
    if (GLVertexArray::s_VertexArrayEnable) {
        // TODO
    }

    a2.ApplyVertexFormat(a1);
}

void GLVertexArray::ApplyGLStates(GLStates::VertexArrayObject& vao) {
    GLSDLDevice* device = GLSDLDevice::Get();

    device->BindVertexArray(this);

    for (int32_t i = 0; i < kMAX_VERTEX_ATTRIBS; i++) {
        auto& attrib = vao.vertexAttribs[i];

        if (attrib.enable) {
            glBindBuffer(attrib.buffer->m_Type, attrib.buffer->m_BufferID);

            glVertexAttribPointerARB(
                i,
                attrib.size,
                attrib.type,
                attrib.normalized,
                attrib.stride,
                reinterpret_cast<void*>(attrib.offset)
            );

            glEnableVertexAttribArrayARB(i);
        } else {
            glDisableVertexAttribArrayARB(i);
        }
    }

    if (vao.position.enable) {
        glBindBuffer(vao.position.buffer->m_Type, vao.position.buffer->m_BufferID);
        glVertexPointer(vao.position.size, vao.position.type, vao.position.stride, vao.position.offset);
        glEnableClientState(GL_VERTEX_ARRAY);
    } else {
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    if (vao.normal.enable) {
        glBindBuffer(vao.normal.buffer->m_Type, vao.normal.buffer->m_BufferID);
        glNormalPointer(vao.normal.type, vao.normal.stride, vao.normal.offset);
        glEnableClientState(GL_NORMAL_ARRAY);
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if (vao.color0.enable) {
        glBindBuffer(vao.color0.buffer->m_Type, vao.color0.buffer->m_BufferID);
        glColorPointer(vao.color0.size, vao.color0.type, vao.color0.stride, vao.color0.offset);
        glEnableClientState(GL_COLOR_ARRAY);
    } else {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    if (vao.color1.enable) {
        glBindBuffer(vao.color1.buffer->m_Type, vao.color1.buffer->m_BufferID);
        glColorPointer(vao.color1.size, vao.color1.type, vao.color1.stride, vao.color1.offset);
        glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
    } else {
        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    }

    for (int32_t i = 0; i < 8; i++) {
        glClientActiveTextureARB(GL_TEXTURE0 + i);

        if (vao.texCoord[i].enable) {
            glBindBuffer(vao.texCoord[i].buffer->m_Type, vao.texCoord[0].buffer->m_BufferID);
            glTexCoordPointer(vao.texCoord[i].size, vao.texCoord[i].type, vao.texCoord[i].stride, vao.texCoord[i].offset);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        } else {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vao.buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.buffers[1]);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, vao.buffers[2]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, vao.buffers[3]);

    this->m_GLStates = vao;
}

void GLVertexArray::ApplyVertexFormat(GLSDLDevice* device) {
    if (GLVertexArray::s_VertexArrayEnable) {
        device->BindVertexArray(this);
    }

    auto indexBuffer = this->m_Properties.m_IndexBuffer;
    uint32_t indexBufferID = indexBuffer ? indexBuffer->m_BufferID : 0;

    if (this->m_GLStates.buffers[1] != indexBufferID) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        this->m_GLStates.buffers[1] = indexBufferID;
    }

    BLIZZARD_ASSERT(this->GetProperties().m_VertexBufferFormat != nullptr);

    auto& properties = this->GetProperties();
    bool attribEnable[16] = {};

    bool useVertexShader = device->GetShader(GLShader::eVertexShader) != nullptr;

    for (int32_t index = 0; index < this->GetProperties().m_VertexBufferFormat->m_Size; index++) {
        BLIZZARD_ASSERT(index < kMAX_VERTEX_ATTRIBS);

        auto& attrib = this->GetProperties().m_VertexBufferFormat->m_Attribs[index];

        BLIZZARD_ASSERT(attrib.type != GLVT_INVALID);
        BLIZZARD_ASSERT(attrib.type < GLVT_NUM_VERTEX_TYPES);

        auto vertexBuffer = this->GetProperties().m_VertexBuffer[attrib.stream];

        if (useVertexShader || static_cast<uint32_t>(attrib.slot - 1) > 1) {
            if (this->m_GLStates.buffers[0] != vertexBuffer->m_BufferID) {
                glBindBuffer(vertexBuffer->m_Type, vertexBuffer->m_BufferID);
                this->m_GLStates.buffers[0] = vertexBuffer->m_BufferID;
            }

            attribEnable[attrib.slot] = 1;

            int32_t stride = properties.m_VertexBufferStride[attrib.stream];
            int32_t offset = attrib.offset
                + properties.m_VertexBufferOffset[attrib.stream]
                + properties.m_VertexBase * stride;

            if (useVertexShader) {
                glVertexAttribPointerARB(
                    attrib.slot,
                    k_VertexTypeInfo[attrib.type].m_Size,
                    k_VertexTypeInfo[attrib.type].m_Type,
                    k_VertexTypeInfo[attrib.type].m_Normalized,
                    stride,
                    reinterpret_cast<void*>(offset)
                );
            } else {
                switch (attrib.slot) {
                case 0: {
                    glVertexPointer(
                        k_VertexTypeInfo[attrib.type].m_Size,
                        k_VertexTypeInfo[attrib.type].m_Type,
                        stride,
                        reinterpret_cast<void*>(offset)
                    );

                    break;
                }

                case 3: {
                    glNormalPointer(
                        k_VertexTypeInfo[attrib.type].m_Type,
                        stride,
                        reinterpret_cast<void*>(offset)
                    );

                    break;
                }

                case 4: {
                    glColorPointer(
                        k_VertexTypeInfo[attrib.type].m_Size,
                        k_VertexTypeInfo[attrib.type].m_Type,
                        stride,
                        reinterpret_cast<void*>(offset)
                    );

                    break;
                }

                case 5: {
                    glSecondaryColorPointer(
                        k_VertexTypeInfo[attrib.type].m_Size,
                        k_VertexTypeInfo[attrib.type].m_Type,
                        stride,
                        reinterpret_cast<void*>(offset)
                    );

                    break;
                }

                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13: {
                    auto tmu = attrib.slot - 6;
                    auto texCoordIndex = device->m_States.fixedFunc.texCoordIndex[tmu];
                    glClientActiveTextureARB(GL_TEXTURE0 + texCoordIndex);
                    glTexCoordPointer(
                        k_VertexTypeInfo[attrib.type].m_Size,
                        k_VertexTypeInfo[attrib.type].m_Type,
                        stride,
                        reinterpret_cast<void*>(offset)
                    );

                    break;
                }
                }
            }
        }
    }

    for (int32_t s = 0; s < 16; s++) {
        // Shader
        if (useVertexShader) {
            auto prevAttribEnable = &this->m_GLStates.vertexAttribs[s].enable;

            if (*prevAttribEnable != attribEnable[s]) {
                if (attribEnable[s]) {
                    glEnableVertexAttribArrayARB(s);
                } else {
                    glDisableVertexAttribArrayARB(s);
                }
            }

            *prevAttribEnable = attribEnable[s];

        // FFP
        } else {
            bool* prevAttribEnable = nullptr;
            GLenum glArray = GL_NONE;

            switch (s) {
            case 0: {
                prevAttribEnable = &this->m_GLStates.position.enable;
                glArray = GL_VERTEX_ARRAY;

                break;
            }

            case 3: {
                prevAttribEnable = &this->m_GLStates.normal.enable;
                glArray = GL_NORMAL_ARRAY;

                break;
            }

            case 4: {
                prevAttribEnable = &this->m_GLStates.color0.enable;
                glArray = GL_COLOR_ARRAY;

                break;
            }

            case 5: {
                prevAttribEnable = &this->m_GLStates.color1.enable;
                glArray = GL_SECONDARY_COLOR_ARRAY;

                break;
            }

            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13: {
                auto tmu = s - 6;
                auto texCoordIndex = device->m_States.fixedFunc.texCoordIndex[tmu];

                prevAttribEnable = &this->m_GLStates.texCoord[texCoordIndex].enable;
                glArray = GL_TEXTURE_COORD_ARRAY;

                glClientActiveTextureARB(GL_TEXTURE0 + texCoordIndex);

                break;
            }

            default:
                break;
            }

            if (prevAttribEnable) {
                if (*prevAttribEnable != attribEnable[s]) {
                    if (attribEnable[s]) {
                        glEnableClientState(glArray);
                    } else {
                        glDisableClientState(glArray);
                    }
                }

                *prevAttribEnable = attribEnable[s];
            }
        }
    }

    if (!useVertexShader) {
        // TODO device->SetColorMaterial(this->m_GLStates.color0.enable);
    }
}

GLVertexArray::Properties& GLVertexArray::GetProperties() {
    return this->m_Properties;
}

void GLVertexArray::ReleaseObject() {
    // TODO
}
