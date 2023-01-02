#ifndef MODEL_M2_ANIMATE_HPP
#define MODEL_M2_ANIMATE_HPP

#include "model/CM2Model.hpp"
#include "model/M2Data.hpp"
#include "model/M2Model.hpp"

struct M2SequenceFallback {
    uint16_t uint0;
    uint16_t uint2;
};

template<class T1, class T2>
void M2SetValue(const T1& sourceValue, T2& destValue) {
    destValue = sourceValue;
}

template<>
void M2SetValue(const M2CompQuat& sourceValue, C4Quaternion& destValue) {
    destValue.x = (sourceValue.auCompQ[0] & 0xFFFF) * 0.000030518044f - 1.0f;
    destValue.y = (sourceValue.auCompQ[0] >> 16)    * 0.000030518044f - 1.0f;
    destValue.z = (sourceValue.auCompQ[1] & 0xFFFF) * 0.000030518044f - 1.0f;
    destValue.w = (sourceValue.auCompQ[1] >> 16)    * 0.000030518044f - 1.0f;
}

template<>
void M2SetValue(const fixed16& sourceValue, float& destValue) {
    destValue = static_cast<float>(sourceValue);
}

void M2InterpolateLinear(const C3Vector& startValue, const C3Vector& endValue, float ratio, C3Vector& value) {
    value.x = startValue.x + (ratio * (endValue.x - startValue.x));
    value.y = startValue.y + (ratio * (endValue.y - startValue.y));
    value.z = startValue.z + (ratio * (endValue.z - startValue.z));
}

void M2InterpolateLinear(float startValue, float endValue, float ratio, float& value) {
    value = startValue + (ratio * (endValue - startValue));
}

void M2InterpolateLinear(fixed16 startValue, fixed16 endValue, float ratio, float& value) {
    value = static_cast<float>(startValue) + (ratio * (static_cast<float>(endValue) - static_cast<float>(startValue)));
}

void M2InterpolateLinear(uint8_t startValue, uint8_t endValue, float ratio, uint8_t& value) {
    value = startValue + (ratio * (endValue - startValue));
}

void M2InterpolateLinear(const M2CompQuat& startValue, const M2CompQuat& endValue, float ratio, C4Quaternion& value) {
    C4Quaternion quat1;
    C4Quaternion quat2;
    M2SetValue(startValue, quat1);
    M2SetValue(endValue, quat2);

    value = C4Quaternion::Nlerp(ratio, quat1, quat2);
}

void M2InterpolateCubicBezier(const M2SplineKey<C3Vector>& startKey, const M2SplineKey<C3Vector>& endKey, float ratio, C3Vector& value) {
    // TODO
}

void M2InterpolateCubicBezier(const M2SplineKey<float>& startKey, const M2SplineKey<float>& endKey, float ratio, float& value) {
    // TODO
}

void M2InterpolateCubicHermite(const M2SplineKey<C3Vector>& startKey, const M2SplineKey<C3Vector>& endKey, float ratio, C3Vector& value) {
    // TODO
}

void M2InterpolateCubicHermite(const M2SplineKey<float>& startKey, const M2SplineKey<float>& endKey, float ratio, float& value) {
    // TODO
}

template<class T1, class T2>
void M2AnimateSplineTrack(CM2Model* model, M2ModelBone* modelBone, const M2Track<T1>& track, M2ModelTrack<T2>& modelTrack, const T2& defaultValue) {
    auto seqIndex = modelBone->sequence.uint4 < track.sequenceKeys.Count() ? modelBone->sequence.uint4 : 0;
    auto& seqKeys = track.sequenceKeys[seqIndex];

    if (seqKeys.keys.Count()) {
        uint32_t nextKey;
        float ratio;

        model->FindKey(&modelBone->sequence, track, modelTrack.currentKey, nextKey, ratio);

        if (track.trackType == 0) {
            modelTrack.currentValue = seqKeys.keys[modelTrack.currentKey].value;
            return;
        }

        auto& startKey = seqKeys.keys[modelTrack.currentKey];
        auto& endKey = seqKeys.keys[nextKey];

        switch (track.trackType) {
            case 1:
                M2InterpolateLinear(startKey.value, endKey.value, ratio, modelTrack.currentValue);
                break;

            case 2:
                M2InterpolateCubicBezier(startKey, endKey, ratio, modelTrack.currentValue);
                break;

            case 3:
                M2InterpolateCubicHermite(startKey, endKey, ratio, modelTrack.currentValue);
                break;
        }
    } else {
        modelTrack.currentValue = defaultValue;

        if (track.trackType == 0) {
            return;
        }
    }

    // TODO
    // - blend with secondary active sequence
}

template<class T1, class T2>
void M2AnimateTrack(CM2Model* model, M2ModelBone* modelBone, const M2Track<T1>& track, M2ModelTrack<T2>& modelTrack, const T2& defaultValue) {
    auto seqIndex = modelBone->sequence.uint4 < track.sequenceKeys.Count() ? modelBone->sequence.uint4 : 0;
    auto& seqKeys = track.sequenceKeys[seqIndex];

    if (seqKeys.keys.Count()) {
        uint32_t nextKey;
        float ratio;

        model->FindKey(&modelBone->sequence, track, modelTrack.currentKey, nextKey, ratio);

        if (track.trackType == 0) {
            M2SetValue<T1, T2>(seqKeys.keys[modelTrack.currentKey], modelTrack.currentValue);
            return;
        }

        auto& startValue = seqKeys.keys[modelTrack.currentKey];
        auto& endValue = seqKeys.keys[nextKey];

        M2InterpolateLinear(startValue, endValue, ratio, modelTrack.currentValue);
    } else {
        modelTrack.currentValue = defaultValue;

        if (track.trackType == 0) {
            return;
        }
    }

    // TODO
    // - blend with secondary active sequence
}

#endif
