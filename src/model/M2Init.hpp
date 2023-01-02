#ifndef MODEL_M2_INIT_HPP
#define MODEL_M2_INIT_HPP

#include "model/M2Data.hpp"
#include "model/CM2Model.hpp"

int32_t M2Init(uint8_t* base, uint32_t size, M2Data& data);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, char& value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, fixed16& value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, float& value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint8_t value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, ubyte4 value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint16_t value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint32_t value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, C2Vector& value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, C3Vector& value);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Attachment& attachment);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Batch& batch);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Bounds& bounds);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Camera& camera);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Color& color);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2CompBone& bone);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2CompQuat& quat);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Event& event);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Light& light);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Loop& loop);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Material& material);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Particle& particle);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Ribbon& ribbon);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Sequence& sequence);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SequenceTimes& track);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SkinProfile& skinProfile);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SkinSection& skinSection);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Texture& texture);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TextureTransform& transform);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TextureWeight& weight);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TrackBase& track);

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Vertex& vertex);

template<class T>
int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Array<T>& array) {
    if (CM2Model::s_loadingSequence == 0xFFFFFFFF) {
        if (array.offset > size) {
            return 0;
        }

        if (array.offset + (array.count * sizeof(T)) > size) {
            return 0;
        }

        if (array.count) {
            uintptr_t absoluteOffset = reinterpret_cast<uintptr_t>(base) + array.offset;
            uintptr_t relativeOffset = absoluteOffset - reinterpret_cast<uintptr_t>(&array);

            array.offset = relativeOffset;
        } else {
            array.offset = 0;
        }
    }

    for (int32_t i = 0; i < array.count; i++) {
        auto& element = array[i];

        if (!M2Init(base, size, data, element)) {
            return 0;
        }
    }

    return 1;
}

template<class T>
int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2PartTrack<T>& track) {
    if (!M2Init<fixed16>(base, size, data, track.times)) {
        return 0;
    }

    if (!M2Init<T>(base, size, data, track.values)) {
        return 0;
    }

    return 1;
}

template<class T>
int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SequenceKeys<T>& track) {
    if (!M2Init<T>(base, size, data, track.keys)) {
        return 0;
    }

    return 1;
}

template<class T>
int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SplineKey<T>& key) {
    return 1;
}

template<class T>
int32_t M2InitKeyFrameData(uint8_t* base, uint32_t size, const M2Data& data, uint32_t sequence, M2Array<T>& keys) {
    if (keys.offset > size) {
        return 0;
    }

    if (keys.offset + (keys.count * sizeof(T)) > size) {
        return 0;
    }

    if (keys.count) {
        uintptr_t absoluteOffset = reinterpret_cast<uintptr_t>(base) + keys.offset;
        uintptr_t relativeOffset = absoluteOffset - reinterpret_cast<uintptr_t>(&keys);

        keys.offset = relativeOffset;
    } else {
        keys.offset = 0;
    }

    uint32_t sequenceFlags = data.sequences[sequence].flags;

    if (sequenceFlags & 0x40) {
        return 1;
    }

    for (int32_t i = 0; i < keys.count; i++) {
        auto& element = keys[i];

        if (!M2Init(base, size, data, element)) {
            return 0;
        }
    }

    return 1;
}

template<class T>
int32_t M2InitSequenceKeyFrames(uint8_t* base, uint32_t size, const M2Data& data, uint32_t sequence, M2Array<T>& keys) {
    uint32_t sequenceFlags = data.sequences[sequence].flags;

    if (sequenceFlags & 0x20) {
        if (!M2InitKeyFrameData<T>(base, size, data, sequence, keys)) {
            return 0;
        }
    }

    return 1;
}

template<class T>
int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Track<T>& track) {
    if (!M2Init(base, size, data, static_cast<M2TrackBase&>(track))) {
        return 0;
    }

    if (CM2Model::s_loadingSequence == 0xFFFFFFFF) {
        if (track.loopIndex == 0xFFFF) {
            if (track.sequenceKeys.count) {
                uintptr_t absoluteOffset = reinterpret_cast<uintptr_t>(base) + track.sequenceKeys.offset;
                uintptr_t relativeOffset = absoluteOffset - reinterpret_cast<uintptr_t>(&track.sequenceKeys);

                track.sequenceKeys.offset = relativeOffset;
            } else {
                track.sequenceKeys.offset = 0;
            }

            for (int32_t i = 0; i < track.sequenceTimes.count; i++) {
                auto& keys = track.sequenceKeys[i];

                if (!M2InitSequenceKeyFrames<T>(base, size, data, i, keys.keys)) {
                    return 0;
                }
            }
        } else {
            if (!M2Init<M2SequenceKeys<T>>(base, size, data, track.sequenceKeys)) {
                return 0;
            }
        }
    } else if (CM2Model::s_loadingSequence < track.sequenceKeys.count) {
        auto& keys = track.sequenceKeys[CM2Model::s_loadingSequence];

        if (!M2InitKeyFrameData<T>(CM2Model::s_sequenceBase, CM2Model::s_sequenceBaseSize, data, CM2Model::s_loadingSequence, keys.keys)) {
            return 0;
        }
    }

    return 1;
}

#endif
