#ifndef GX_COORDINATE_HPP
#define GX_COORDINATE_HPP

float CoordinateGetAspectCompensation(void);

void CoordinateSetAspectRatio(float);

void DDCToNDC(float, float, float*, float*);

float DDCToNDCHeight(float);

float DDCToNDCWidth(float);

void NDCToDDC(float, float, float*, float*);

float NDCToDDCHeight(float);

float NDCToDDCWidth(float);

#endif
