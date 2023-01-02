#include "gx/Coordinate.hpp"
#include <cmath>

namespace Coordinate {
    float s_aspect = 1.333333f;
    float s_aspectCompensation = 1.0f;
    float s_y = 0.6f;
    float s_x = 0.8f;
}

float CoordinateGetAspectCompensation() {
    return Coordinate::s_aspectCompensation;
}

void CoordinateSetAspectRatio(float aspect) {
    Coordinate::s_aspect = aspect;
    Coordinate::s_aspectCompensation = aspect * 0.75;

    float v1 = 1.0 / sqrt(aspect * aspect + 1.0);
    Coordinate::s_y = v1;
    Coordinate::s_x = aspect * v1;
}

void DDCToNDC(float ddcx, float ddcy, float* ndcx, float* ndcy) {
    if (ndcx) {
        *ndcx = ddcx / Coordinate::s_x;
    }

    if (ndcy) {
        *ndcy = ddcy / Coordinate::s_y;
    }
}

float DDCToNDCHeight(float ddcy) {
    return ddcy / Coordinate::s_y;
}

float DDCToNDCWidth(float ddcx) {
    return ddcx / Coordinate::s_x;
}

void NDCToDDC(float ndcx, float ndcy, float* ddcx, float* ddcy) {
    if (ddcx) {
        *ddcx = Coordinate::s_x * ndcx;
    }

    if (ddcy) {
        *ddcy = Coordinate::s_y * ndcy;
    }
}

float NDCToDDCHeight(float ndcy) {
    return Coordinate::s_y * ndcy;
}

float NDCToDDCWidth(float ndcx) {
    return Coordinate::s_x * ndcx;
}
