#ifndef MODEL_M2_DATA_HPP
#define MODEL_M2_DATA_HPP

#include "gx/Buffer.hpp"
#include "math/Types.hpp"
#include <cstdint>
#include <tempest/Box.hpp>
#include <tempest/Vector.hpp>

/*
    M2Array has been modified from the implementation present in 12340. The
    implementation present in 12340 looks like this:

    template<class T>
    struct M2Array {
        uint32_t count;
        union {
            T* data;
            uint32_t offset;
        }
    };

    On a 32-bit system, sizeof(M2Array) == 8 bytes in memory: 4 bytes for the
    count, and 4 bytes for the union. This lines up with M2Array in the .m2
    files: each M2Array is 8 bytes.

    In 12340 (and until 64-bit support was introduced), the M2Init functions
    simply adjust the M2Array when loading:

    m2Data->someM2Array.offset =
        (uint32_t)m2Data + m2Data->someM2Array.offset;

    This ensures T* data points to the appropriate (absolute) location in
    memory.

    Unfortunately, this approach fails on 64-bit systems. On a 64-bit system,
    M2Array would occupy 12 bytes in memory: 4 bytes for the count, and 8 bytes
    for the union. This would make the approach outlined above fail.

    As a result, on 64-bit systems, a different approach is used: M2Arrays are
    assumed (reasonably so) to only exist within the same structure that their
    on-disk offsets reference. Thus, M2Init adjusts the M2Array when loading:

    uintptr_t absoluteOffset =
        (uintptr_t)m2Data + m2Data->someM2Array.offset;
    uintptr_t relativeOffset =
        absoluteOffset - (uintptr_t)&m2Data->someM2Array;
    m2Data->someM2Array.offset =
        (uint32_t)relativeOffset;

    By storing the relative offset, access to the data is possible by adding
    the relative offset to the address of the M2Array:

    uintptr_t absoluteOffset =
        (uintptr_t)m2Data->someM2Array + m2Data->someM2Array.offset;
    T* data = (T*)absoluteOffset;
*/

template<class T>
struct M2Array {
    uint32_t count;
    uint32_t offset;

    T& operator[](uint32_t i);
    T& operator[](uint32_t i) const;
    uint32_t Count(void);
    uint32_t Count(void) const;
    T* Data(void);
};

template<class T>
T& M2Array<T>::operator[](uint32_t i) {
    T* data = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + this->offset);
    return data[i];
}

template<class T>
T& M2Array<T>::operator[](uint32_t i) const {
    T* data = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + this->offset);
    return data[i];
}

template<class T>
uint32_t M2Array<T>::Count() {
    return this->count;
}

template<class T>
uint32_t M2Array<T>::Count() const {
    return this->count;
}

template<class T>
T* M2Array<T>::Data() {
    T* data = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + this->offset);
    return data;
}

template<class T>
struct M2SequenceKeys {
    M2Array<T> keys;
};

struct M2SequenceTimes {
    M2Array<uint32_t> times;
};

struct M2TrackBase {
    uint16_t trackType;
    uint16_t loopIndex;
    M2Array<M2SequenceTimes> sequenceTimes;
};

template<class T>
class M2Track : public M2TrackBase {
    public:
        M2Array<M2SequenceKeys<T>> sequenceKeys;
};

struct M2Attachment {
    uint32_t attachmentId;
    uint16_t boneIndex;
    C3Vector position;
    M2Track<uint8_t> visibilityTrack;
};

struct M2Batch {
    uint8_t flags;
    int8_t priorityPlane;
    uint16_t shader;
    uint16_t skinSectionIndex;
    uint16_t geosetIndex;
    uint16_t colorIndex;
    uint16_t materialIndex;
    uint16_t materialLayer;
    uint16_t textureCount;
    uint16_t textureComboIndex;
    uint16_t textureCoordComboIndex;
    uint16_t textureWeightComboIndex;
    uint16_t textureTransformComboIndex;
};

struct M2Bounds {
    CAaBox extent;
    float radius;
};

template<class T>
struct M2SplineKey {
    T value;
    T inTan;
    T outTan;
};

struct M2Camera {
    uint32_t cameraId;
    float fieldOfView;
    float farClip;
    float nearClip;
    M2Track<M2SplineKey<C3Vector>> positionTrack;
    C3Vector positionPivot;
    M2Track<M2SplineKey<C3Vector>> targetTrack;
    C3Vector targetPivot;
    M2Track<M2SplineKey<float>> rollTrack;
};

struct M2Color {
    M2Track<C3Vector> colorTrack;
    M2Track<fixed16> alphaTrack;
};

struct M2CompQuat {
    uint32_t auCompQ[2];
};

struct M2CompBone {
    uint32_t boneId;
    uint32_t flags;
    uint16_t parentIndex;
    uint16_t uDistToParent;
    union {
        struct {
            uint16_t uDistToFurthDesc;
            uint16_t uZRatioOfChain;
        } CompressData;
        uint32_t boneNameCRC;
    };
    M2Track<C3Vector> translationTrack;
    M2Track<M2CompQuat> rotationTrack;
    M2Track<C3Vector> scaleTrack;
    C3Vector pivot;
};

struct M2Event {
    uint32_t eventId;
    uint32_t data;
    uint16_t boneIndex;
    C3Vector position;
    M2TrackBase eventTrack;
};

struct M2Light {
    uint16_t lightType;
    uint16_t boneIndex;
    C3Vector position;
    M2Track<C3Vector> ambientColorTrack;
    M2Track<float> ambientIntensityTrack;
    M2Track<C3Vector> diffuseColorTrack;
    M2Track<float> diffuseIntensityTrack;
    M2Track<float> attenuationStartTrack;
    M2Track<float> attenuationEndTrack;
    M2Track<uint8_t> visibilityTrack;
};

struct M2Loop {
    uint32_t length;
};

struct M2Material {
    uint16_t flags;
    uint16_t blendMode;
};

template<class T>
struct M2PartTrack {
    M2Array<fixed16> times;
    M2Array<T> values;
};

struct M2Particle {
    uint32_t particleId;
    uint32_t flags;
    C3Vector position;
    uint16_t boneIndex;
    uint16_t textureIndex;
    M2Array<char> geometryMdl;
    M2Array<char> recursionMdl;
    uint8_t blendMode;
    uint8_t emitterType;
    uint16_t colorIndex;
    uint16_t pad;
    int16_t priorityPlane;
    uint16_t rows;
    uint16_t cols;
    M2Track<float> speedTrack;
    M2Track<float> variationTrack;
    M2Track<float> latitudeTrack;
    M2Track<float> longitudeTrack;
    M2Track<float> gravityTrack;
    M2Track<float> lifeTrack;
    float lifeVariation;
    M2Track<float> emissionRateTrack;
    float emissionRateVariation;
    M2Track<float> widthTrack;
    M2Track<float> lengthTrack;
    M2Track<float> zsourceTrack;
    M2PartTrack<C3Vector> colorTrack;
    M2PartTrack<fixed16> alphaTrack;
    M2PartTrack<C2Vector> scaleTrack;
    C2Vector scaleVariation;
    M2PartTrack<uint16_t> headCellTrack;
    M2PartTrack<uint16_t> tailCellTrack;
    float tailLength;
    float twinkleFPS;
    float twinkleOnOff;
    CRange twinkleScale;
    float ivelScale;
    float drag;
    float initialSpin;
    float initialSpinVariation;
    float spin;
    float spinVariation;
    CAaBox tumble;
    C3Vector windVector;
    float windTime;
    float followSpeed1;
    float followScale1;
    float followSpeed2;
    float followScale2;
    M2Array<C3Vector> spline;
    M2Track<uint8_t> visibilityTrack;
};

struct M2Ribbon {
    uint32_t ribbonId;
    uint16_t boneIndex;
    C3Vector position;
    M2Array<uint16_t> textureIndices;
    M2Array<uint16_t> materialIndices;
    M2Track<C3Vector> colorTrack;
    M2Track<fixed16> alphaTrack;
    M2Track<float> heightAboveTrack;
    M2Track<float> heightBelowTrack;
    float edgesPerSecond;
    float edgeLifetime;
    float gravity;
    uint16_t textureRows;
    uint16_t textureCols;
    M2Track<uint16_t> textureSlotTrack;
    M2Track<uint8_t> visibilityTrack;
    int16_t priorityPlane;
    uint16_t pad;
};

struct M2Sequence {
    uint16_t id;
    uint16_t variationIndex;
    uint32_t duration;
    float movespeed;
    uint32_t flags;
    uint32_t frequency;
    CiRange replay;
    uint32_t blendtime;
    M2Bounds bounds;
    uint16_t variationNext;
    uint16_t aliasNext;
};

struct M2SkinSection {
    uint32_t skinSectionId;
    uint16_t vertexStart;
    uint16_t vertexCount;
    uint16_t indexStart;
    uint16_t indexCount;
    uint16_t boneCount;
    uint16_t boneComboIndex;
    uint16_t boneInfluences;
    uint16_t centerBoneIndex;
    C3Vector centerPosition;
    C3Vector sortCenterPosition;
    float sortRadius;
};

struct M2Texture {
    uint32_t textureId;
    uint16_t flags;
    M2Array<char> filename;
};

struct M2TextureTransform {
    M2Track<C3Vector> translationTrack;
    M2Track<M2CompQuat> rotationTrack;
    M2Track<C3Vector> scaleTrack;
};

struct M2TextureWeight {
    M2Track<fixed16> weightTrack;
};

struct M2Vertex {
    C3Vector position;
    ubyte4 weights;
    ubyte4 indices;
    C3Vector normal;
    C2Vector texcoord[2];
};

// .m2 files
struct M2Data {
    uint32_t MD20;
    uint32_t version;
    M2Array<char> name;
    uint32_t flags;
    M2Array<M2Loop> loops;
    M2Array<M2Sequence> sequences;
    M2Array<uint16_t> sequenceIdxHashById;
    M2Array<M2CompBone> bones;
    M2Array<uint16_t> boneIndicesById;
    M2Array<M2Vertex> vertices;
    uint32_t numSkinProfiles;
    M2Array<M2Color> colors;
    M2Array<M2Texture> textures;
    M2Array<M2TextureWeight> textureWeights;
    M2Array<M2TextureTransform> textureTransforms;
    M2Array<uint16_t> textureIndicesById;
    M2Array<M2Material> materials;
    M2Array<uint16_t> boneCombos;
    M2Array<uint16_t> textureCombos;
    M2Array<uint16_t> textureCoordCombos;
    M2Array<uint16_t> textureWeightCombos;
    M2Array<uint16_t> textureTransformCombos;
    M2Bounds bounds;
    M2Bounds collisionBounds;
    M2Array<uint16_t> collisionIndices;
    M2Array<C3Vector> collisionPositions;
    M2Array<C3Vector> collisionFaceNormals;
    M2Array<M2Attachment> attachments;
    M2Array<uint16_t> attachmentIndicesById;
    M2Array<M2Event> events;
    M2Array<M2Light> lights;
    M2Array<M2Camera> cameras;
    M2Array<uint16_t> cameraIndicesById;
    M2Array<M2Ribbon> ribbons;
    M2Array<M2Particle> particles;
    M2Array<uint16_t> textureCombinerCombos;
};

// .skin files
struct M2SkinProfile {
    uint32_t magic;
    M2Array<uint16_t> vertices;
    M2Array<uint16_t> indices;
    M2Array<ubyte4> bones;
    M2Array<M2SkinSection> skinSections;
    M2Array<M2Batch> batches;
    uint32_t boneCountMax;
};

#endif
