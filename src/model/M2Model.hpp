#ifndef MODEL_M2_MODEL_HPP
#define MODEL_M2_MODEL_HPP

#include "gx/Camera.hpp"
#include "model/CM2Light.hpp"
#include <cstdint>
#include <tempest/Quaternion.hpp>
#include <tempest/Vector.hpp>

template<class T>
class M2Track;

template<class T>
struct M2ModelTrack {
    uint32_t currentKey = 0;
    M2Track<T>* sourceTrack = nullptr;
    T currentValue;
};

struct M2ModelAttachment {
};

struct M2ModelBoneSeq {
    uint32_t uint0 = 0;
    uint16_t uint4 = -1;
    uint16_t uint6 = -1;
    uint16_t uint8 = -1;
    uint8_t uintA = 1;
    uint8_t uintB = 1;
    uint32_t uintC = 0;
    uint32_t uint10 = 0;
    float float14 = 0.0;
    float float18 = 0.0;
    uint32_t uint1C = 0;
    uint32_t uint20 = 0;
};

struct M2ModelBone {
    M2ModelTrack<C3Vector> translationTrack;
    M2ModelTrack<C4Quaternion> rotationTrack;
    M2ModelTrack<C3Vector> scaleTrack;
    M2ModelBoneSeq sequence;
    M2ModelBoneSeq secondarySequence;
    uint32_t flags = 0;
    uint32_t uint90 = -1;
    uint16_t uint94 = 0;
    uint32_t uint9C = 0;
    float floatA0 = 0.0f;
    float floatA4 = 1.0f;
    float floatA8 = 0.0f;
};

struct M2ModelCamera {
    M2ModelTrack<C3Vector> positionTrack;
    M2ModelTrack<C3Vector> targetTrack;
    M2ModelTrack<float> rollTrack;
    HCAMERA m_camera = nullptr;
};

struct M2ModelColor {
    M2ModelTrack<C3Vector> colorTrack;
    M2ModelTrack<float> alphaTrack;
};

struct M2ModelLight {
    M2ModelTrack<C3Vector> ambientColorTrack;
    M2ModelTrack<float> ambientIntensityTrack;
    M2ModelTrack<C3Vector> diffuseColorTrack;
    M2ModelTrack<float> diffuseIntensityTrack;
    M2ModelTrack<uint8_t> visibilityTrack;
    uint32_t uint64 = 1;
    CM2Light light;
};

struct M2ModelTextureTransform {
};

struct M2ModelTextureWeight {
    M2ModelTrack<float> weightTrack;
};

#endif
