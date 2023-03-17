#include "model/CM2Cache.hpp"
#include "gx/Gx.hpp"
#include "model/CM2Shared.hpp"
#include "model/Model2.hpp"
#include "util/Filesystem.hpp"
#include "util/SFile.hpp"
#include <cstring>
#include <new>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <tempest/Box.hpp>

CM2Cache CM2Cache::s_cache;

void CM2Cache::BeginThread(void (*callback)(void*), void* arg) {
    // TODO
}

CM2Shared* CM2Cache::CreateShared(const char* path, uint32_t flags) {
    char convertedPath[STORM_MAX_PATH];
    if (!M2ConvertModelFileName(path, convertedPath, STORM_MAX_PATH, flags)) {
        return nullptr;
    }

    char* ext = OsPathFindExtensionWithDot(convertedPath);

    CAaBox v28;
    ModelBlobQuery(convertedPath, v28.b, v28.t);

    if (ext) {
        *ext = '.';
    }

    // TODO

    SFile* fileptr;

    if (SFile::OpenEx(nullptr, convertedPath, (flags >> 2) & 1, &fileptr)) {
        auto m = SMemAlloc(sizeof(CM2Shared), __FILE__, __LINE__, 0x0);
        auto shared = new (m) CM2Shared(this);

        if (shared->Load(fileptr, flags & 0x4, &v28)) {
            strcpy(shared->m_filePath, convertedPath);
            shared->ext = strrchr(shared->m_filePath, '.');;

            if (shared->ext > shared->m_filePath) {
                // TODO
            }

            // TODO

            return shared;
        }

        SFile::Close(fileptr);
        delete shared;
    }

    return nullptr;
}

void CM2Cache::GarbageCollect(int32_t a2) {
    // TODO
}

int32_t CM2Cache::Initialize(uint32_t flags) {
    if (this->m_initialized) {
        // TODO

        return 1;
    }

    // TODO

    if (flags & 0x8) {
        if (GxCaps().m_shaderTargets[GxSh_Vertex] > GxShVS_none && GxCaps().m_shaderTargets[GxSh_Pixel] > GxShPS_none) {
            this->m_flags |= 0x8;
        }
    }

    // TODO

    this->m_initialized = 1;

    return 1;
}

void CM2Cache::UpdateShared() {
    // TODO
}

void CM2Cache::WaitThread() {
    // TODO
}
