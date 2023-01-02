#include "gx/font/FaceData.hpp"
#include "util/SFile.hpp"

FACEDATA::~FACEDATA() {
    if (this->face) {
        FT_Done_Face(this->face);
    }

    if (this->data) {
        SFile::Unload(this->data);
    }
}
