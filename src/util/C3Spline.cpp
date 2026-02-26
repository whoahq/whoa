#include "util/C3Spline.hpp"
#include <tempest/vector/C3Vector.hpp>

CDataStore& operator>>(CDataStore& msg, C3Spline_CatmullRom& spline) {
    uint32_t pointCount = 0;
    msg.Get(pointCount);

    void* points;
    msg.GetDataInSitu(points, sizeof(C3Vector) * pointCount);

    uint8_t splineMode;
    msg.Get(splineMode);
    // TODO spline.splineMode = splineMode;

    if (pointCount && msg.IsValid()) {
        // TODO spline.SetPoints()
    }

    return msg;
}
