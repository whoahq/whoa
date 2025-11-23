#include "sound/SoundKitProperties.hpp"
#include "util/SFile.hpp"

void SoundKitProperties::ResetToDefaults() {
    // TODO

    this->int20 = -1;

    // TODO

    this->m_type = 0;

    // TODO

    this->uint24 = 0;

    // TODO

    this->m_streaming = SFile::IsStreamingMode() != 0;

    // TODO
}
