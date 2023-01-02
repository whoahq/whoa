#include "gx/shader/CGxShader.hpp"
#include "gx/Device.hpp"
#include "util/SFile.hpp"

void CGxShader::Load(SFile* file) {
    SFile::Read(file, &this->int3C, 4, nullptr, nullptr, nullptr);
    SFile::Read(file, &this->int40, 4, nullptr, nullptr, nullptr);
    SFile::Read(file, &this->int44, 2, nullptr, nullptr, nullptr);
    SFile::Read(file, &this->int46, 2, nullptr, nullptr, nullptr);

    int32_t codeLen;
    SFile::Read(file, &codeLen, 4, nullptr, nullptr, nullptr);
    this->code.SetCount(codeLen);
    SFile::Read(file, this->code.m_data, codeLen, 0, 0, 0);

    int32_t pad;
    SFile::Read(file, &pad, ((codeLen + 3) & 0xFFFFFFFC) - codeLen, 0, 0, 0);
}

int32_t CGxShader::Valid() {
    if (!this->loaded) {
        g_theGxDevicePtr->IShaderCreate(this);
    }

    return this->valid;
}
