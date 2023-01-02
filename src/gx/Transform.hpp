#ifndef GX_TRANSFORM_HPP
#define GX_TRANSFORM_HPP

#include "gx/Types.hpp"

class C3Vector;
class C44Matrix;

void GxXformPop(EGxXform xf);

void GxXformProjection(C44Matrix&);

void GxXformProjNative(C44Matrix&);

void GxXformProjNativeTranspose(C44Matrix&);

void GxXformPush(EGxXform xf);

void GxXformSet(EGxXform xf, const C44Matrix& matrix);

void GxXformSetProjection(const C44Matrix&);

void GxXformSetView(const C44Matrix&);

void GxXformSetViewport(float, float, float, float, float, float);

void GxXformView(C44Matrix&);

void GxXformViewport(float&, float&, float&, float&, float&, float&);

void GxXformViewProjNativeTranspose(C44Matrix&);

void GxuXformCreateLookAtSgCompat(const C3Vector& eye, const C3Vector& center, const C3Vector& up, C44Matrix& dst);

void GxuXformCreateOrtho(float, float, float, float, float, float, C44Matrix&);

void GxuXformCreateOrthoDepth(float, float, float, float, float, float, C44Matrix&);

void GxuXformCreateProjection_Exact(float fovyInRadians, float aspect, float minZ, float maxZ, C44Matrix& dst);

void GxuXformCreateProjection_SG(float fov, float aspect, float minZ, float maxZ, C44Matrix& dst);

#endif
