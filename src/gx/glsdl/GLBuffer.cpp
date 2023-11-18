#include "gx/glsdl/GLBuffer.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLPool.hpp"
#include "gx/glsdl/GL.hpp"
#include <bc/Debug.hpp>
#include <bc/Memory.hpp>

bool GLBuffer::m_UsingVBO = 1;

GLEnum GLBuffer::s_FlagToAccess[] = {
    GL_READ_WRITE,  // GLMap_None
    GL_WRITE_ONLY,  // GLMap_Unk1
    GL_WRITE_ONLY,  // GLMap_Unk2
    GL_READ_ONLY    // GLMap_Unk3
};

GLBuffer* GLBuffer::Create(GLEnum type, uint32_t size, const void* a3, GLEnum usage, GLEnum format) {
    GLBuffer* buffer = GLPool<GLBuffer>::Get()->GetNextObject();

    buffer->m_Type = type;
    buffer->m_Size = size;
    buffer->m_Usage = usage;
    buffer->m_IndexFormat = format;

    GLSDLDevice* device = GLSDLDevice::Get();
    device->BindBuffer(buffer, GL_ZERO);

    if (GLBuffer::m_UsingVBO) {
        glBufferData(buffer->m_Type, buffer->m_Size, a3, buffer->m_Usage);
        // glBufferParameteriAPPLE(buffer->m_Type, GL_BUFFER_SERIALIZED_MODIFY_APPLE, buffer->m_Usage - GL_DYNAMIC_DRAW > 1);
        // glBufferParameteriAPPLE(buffer->m_Type, GL_BUFFER_FLUSHING_UNMAP_APPLE, 0);
    } else {
        Blizzard::Memory::Free(buffer->m_Data);

        void* data = Blizzard::Memory::Allocate(size);
        if (a3) {
            memcpy(data, a3, size);
        }

        buffer->m_Data = reinterpret_cast<char*>(data);
    }

    // TODO
    // buffer->m_TimeStamp = Blizzard::Time::GetTimestamp();

    return buffer;
}

GLBuffer::GLBuffer() : GLObject() {
    if (GLBuffer::m_UsingVBO) {
        this->m_BufferID = GLPool<GLBuffer>::Get()->GetNextName();
    }
}

char* GLBuffer::Map(uint32_t offset, uint32_t size, eMapFlag flag) {
    BLIZZARD_ASSERT((offset + size) <= this->m_Size);
    BLIZZARD_ASSERT(this->m_Usage == GL_STATIC_DRAW || flag != GLMap_None);
    BLIZZARD_ASSERT(this->m_MapFlag == GLMap_NotMapped);
    BLIZZARD_ASSERT(flag >= GLMap_None && flag < GLMap_Count);

    this->m_MapOffset = offset;
    this->m_MapSize = offset + size == 0 ? this->m_Size : size;
    this->m_MapFlag = flag;

    if (GLBuffer::m_UsingVBO) {
        GLSDLDevice* device = GLSDLDevice::Get();
        device->BindBuffer(this, GL_ZERO);

        if (flag == GLMap_Unk2) {
            if (this->m_Usage - GL_DYNAMIC_DRAW <= 1) {
                BLIZZARD_ASSERT(offset == 0);
            }

            glBufferData(this->m_Type, this->m_Size, nullptr, this->m_Usage);
        }

        void* data = glMapBuffer(this->m_Type, GLBuffer::s_FlagToAccess[flag]);
        this->m_Data = reinterpret_cast<char*>(data);

        BLIZZARD_ASSERT(this->m_Data != nullptr);
    }

    return this->m_Data + offset;
}

void GLBuffer::ReleaseObject() {
    if (GLBuffer::m_UsingVBO) {
        if (this->m_Type) {
            GLSDLDevice* device = GLSDLDevice::Get();
            device->BindBuffer(this, GL_ZERO);

            glBufferData(this->m_Type, 1, nullptr, this->m_Usage);

            // TODO GLPool<GLBuffer>::GLObjectPool::Push((GLPool<GLBuffer>::m_pool + 32776), this);
        } else {
            // TODO GLPool<GLBuffer>::GLObjectPool::Push((GLPool<GLBuffer>::m_pool + 32776), this);
        }
    } else {
        Blizzard::Memory::Free(this->m_Data);
        this->m_Data = nullptr;

        // TODO GLPool<GLBuffer>::GLObjectPool::Push((GLPool<GLBuffer>::m_pool + 32776), this);
    }
}

void GLBuffer::Unmap(uint32_t size) {
    BLIZZARD_ASSERT((this->m_MapOffset + size) <= m_Size);

    GLSDLDevice* device = GLSDLDevice::Get();
    device->BindBuffer(this, GL_ZERO);

    if (this->m_MapFlag != 3) {
        if (GLBuffer::m_UsingVBO) {
            glFlushMappedBufferRange(this->m_Type, this->m_MapOffset, size ? size : this->m_MapSize);
        }

        // TODO
        // this->m_TimeStamp = Blizzard::Time::GetTimestamp();
    }

    if (!GLBuffer::m_UsingVBO) {
        this->m_MapFlag = GLMap_NotMapped;
        return;
    }

    GLboolean result = glUnmapBuffer(this->m_Type);
    BLIZZARD_ASSERT(result);

    this->m_MapFlag = GLMap_NotMapped;
}
