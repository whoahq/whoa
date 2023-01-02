#include "model/M2Init.hpp"

int32_t M2Init(uint8_t* base, uint32_t size, M2Data& data) {
    if (!M2Init<char>(base, size, data, data.name)) {
        return 0;
    }

    if (!M2Init<M2Loop>(base, size, data, data.loops)) {
        return 0;
    }

    if (!M2Init<M2Sequence>(base, size, data, data.sequences)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.sequenceIdxHashById)) {
        return 0;
    }

    if (!M2Init<M2CompBone>(base, size, data, data.bones)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.boneIndicesById)) {
        return 0;
    }

    if (!M2Init<M2Vertex>(base, size, data, data.vertices)) {
        return 0;
    }

    if (!M2Init<M2Color>(base, size, data, data.colors)) {
        return 0;
    }

    if (!M2Init<M2Texture>(base, size, data, data.textures)) {
        return 0;
    }

    if (!M2Init<M2TextureWeight>(base, size, data, data.textureWeights)) {
        return 0;
    }

    if (!M2Init<M2TextureTransform>(base, size, data, data.textureTransforms)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.textureIndicesById)) {
        return 0;
    }

    if (!M2Init<M2Material>(base, size, data, data.materials)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.boneCombos)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.textureCombos)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.textureCoordCombos)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.textureWeightCombos)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.textureTransformCombos)) {
        return 0;
    }

    if (!M2Init(base, size, data, data.bounds)) {
        return 0;
    }

    if (!M2Init(base, size, data, data.collisionBounds)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.collisionIndices)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, data.collisionPositions)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, data.collisionFaceNormals)) {
        return 0;
    }

    if (!M2Init<M2Attachment>(base, size, data, data.attachments)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.attachmentIndicesById)) {
        return 0;
    }

    if (!M2Init<M2Event>(base, size, data, data.events)) {
        return 0;
    }

    if (!M2Init<M2Light>(base, size, data, data.lights)) {
        return 0;
    }

    if (!M2Init<M2Camera>(base, size, data, data.cameras)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, data.cameraIndicesById)) {
        return 0;
    }

    if (!M2Init<M2Ribbon>(base, size, data, data.ribbons)) {
        return 0;
    }

    if (!M2Init<M2Particle>(base, size, data, data.particles)) {
        return 0;
    }

    if (data.flags & 0x8) {
        if (!M2Init<uint16_t>(base, size, data, data.textureCombinerCombos)) {
            return 0;
        }
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, char& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, fixed16& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, float& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint8_t value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, ubyte4 value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint16_t value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, uint32_t value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, C2Vector& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, C3Vector& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Attachment& attachment) {
    if (!M2Init<uint8_t>(base, size, data, attachment.visibilityTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Batch& batch) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Bounds& bounds) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Camera& camera) {
    if (!M2Init<M2SplineKey<C3Vector>>(base, size, data, camera.positionTrack)) {
        return 0;
    }

    if (!M2Init<M2SplineKey<C3Vector>>(base, size, data, camera.targetTrack)) {
        return 0;
    }

    if (!M2Init<M2SplineKey<float>>(base, size, data, camera.rollTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Color& color) {
    if (!M2Init<C3Vector>(base, size, data, color.colorTrack)) {
        return 0;
    }

    if (!M2Init<fixed16>(base, size, data, color.alphaTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2CompBone& bone) {
    if (!M2Init<C3Vector>(base, size, data, bone.translationTrack)) {
        return 0;
    }

    if (!M2Init<M2CompQuat>(base, size, data, bone.rotationTrack)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, bone.scaleTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2CompQuat& value) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Event& event) {
    if (!M2Init(base, size, data, event.eventTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Light& light) {
    if (!M2Init<C3Vector>(base, size, data, light.ambientColorTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, light.ambientIntensityTrack)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, light.diffuseColorTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, light.diffuseIntensityTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, light.attenuationStartTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, light.attenuationEndTrack)) {
        return 0;
    }

    if (!M2Init<uint8_t>(base, size, data, light.visibilityTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Loop& loop) {
    if (!M2Init(base, size, data, loop.length)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Material& material) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Particle& particle) {
    if (!M2Init<char>(base, size, data, particle.geometryMdl)) {
        return 0;
    }

    if (!M2Init<char>(base, size, data, particle.recursionMdl)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.speedTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.variationTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.latitudeTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.longitudeTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.gravityTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.lifeTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.emissionRateTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.widthTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.lengthTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, particle.zsourceTrack)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, particle.colorTrack)) {
        return 0;
    }

    if (!M2Init<fixed16>(base, size, data, particle.alphaTrack)) {
        return 0;
    }

    if (!M2Init<C2Vector>(base, size, data, particle.scaleTrack)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, particle.headCellTrack)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, particle.tailCellTrack)) {
        return 0;
    }

    if (!M2Init<uint8_t>(base, size, data, particle.visibilityTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Ribbon& ribbon) {
    if (!M2Init<uint16_t>(base, size, data, ribbon.textureIndices)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, ribbon.materialIndices)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, ribbon.colorTrack)) {
        return 0;
    }

    if (!M2Init<fixed16>(base, size, data, ribbon.alphaTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, ribbon.heightAboveTrack)) {
        return 0;
    }

    if (!M2Init<float>(base, size, data, ribbon.heightBelowTrack)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, ribbon.textureSlotTrack)) {
        return 0;
    }

    if (!M2Init<uint8_t>(base, size, data, ribbon.visibilityTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Sequence& sequence) {
    if (sequence.flags & 0x1) {
        sequence.flags |= 0x80;
    }

    switch (sequence.id) {
        case 0:
        case 4:
        case 5:
        case 13:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 69:
        case 119:
        case 120:
        case 143:
        case 223:
            sequence.flags &= ~0x1;
            break;

        default:
            break;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SequenceTimes& track) {
    if (!M2Init<uint32_t>(base, size, data, track.times)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SkinProfile& skinProfile) {
    if (!M2Init<uint16_t>(base, size, data, skinProfile.vertices)) {
        return 0;
    }

    if (!M2Init<uint16_t>(base, size, data, skinProfile.indices)) {
        return 0;
    }

    if (!M2Init<ubyte4>(base, size, data, skinProfile.bones)) {
        return 0;
    }

    if (!M2Init<M2SkinSection>(base, size, data, skinProfile.skinSections)) {
        return 0;
    }

    if (!M2Init<M2Batch>(base, size, data, skinProfile.batches)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2SkinSection& skinSection) {
    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Texture& texture) {
    if (!M2Init<char>(base, size, data, texture.filename)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TextureTransform& transform) {
    if (!M2Init<C3Vector>(base, size, data, transform.translationTrack)) {
        return 0;
    }

    if (!M2Init<M2CompQuat>(base, size, data, transform.rotationTrack)) {
        return 0;
    }

    if (!M2Init<C3Vector>(base, size, data, transform.scaleTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TextureWeight& weight) {
    if (!M2Init<fixed16>(base, size, data, weight.weightTrack)) {
        return 0;
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2TrackBase& track) {
    if (CM2Model::s_loadingSequence == 0xFFFFFFFF) {
        if (track.loopIndex == 0xFFFF) {
            if (track.sequenceTimes.count) {
                uintptr_t absoluteOffset = reinterpret_cast<uintptr_t>(base) + track.sequenceTimes.offset;
                uintptr_t relativeOffset = absoluteOffset - reinterpret_cast<uintptr_t>(&track.sequenceTimes);

                track.sequenceTimes.offset = relativeOffset;
            } else {
                track.sequenceTimes.offset = 0;
            }

            for (int32_t i = 0; i < track.sequenceTimes.count; i++) {
                auto& keys = track.sequenceTimes[i];

                if (!M2InitSequenceKeyFrames<uint32_t>(base, size, data, i, keys.times)) {
                    return 0;
                }
            }
        } else {
            if (!M2Init<M2SequenceTimes>(base, size, data, track.sequenceTimes)) {
                return 0;
            }
        }
    } else {
        if (CM2Model::s_loadingSequence < track.sequenceTimes.count) {
            auto& keys = track.sequenceTimes[CM2Model::s_loadingSequence];

            if (!M2InitKeyFrameData<uint32_t>(CM2Model::s_sequenceBase, CM2Model::s_sequenceBaseSize, data, CM2Model::s_loadingSequence, keys.times)) {
                return 0;
            }
        }
    }

    return 1;
}

int32_t M2Init(uint8_t* base, uint32_t size, const M2Data& data, M2Vertex& vertex) {
    return 1;
}
