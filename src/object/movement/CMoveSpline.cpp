#include "object/movement/CMoveSpline.hpp"
#include "util/DataStore.hpp"
#include <common/Time.hpp>

void CMoveSpline::Skip(CDataStore* msg) {
    uint32_t flags;
    msg->Get(flags);

    uint32_t faceBytes = 0;

    if (flags & 0x20000) {
        faceBytes = 4;
    } else if (flags & 0x10000) {
        faceBytes = 8;
    } else if (flags & 0x8000) {
        faceBytes = 12;
    }

    void* data;
    msg->GetDataInSitu(data, faceBytes + 28);

    uint32_t splinePoints = 0;
    msg->Get(splinePoints);

    msg->GetDataInSitu(data, (splinePoints * sizeof(C3Vector)) + 13);
}

CDataStore& operator>>(CDataStore& msg, CMoveSpline& spline) {
    msg.Get(spline.flags);

    if (spline.flags & 0x20000) {
        msg.Get(spline.face.facing);
    } else if (spline.flags & 0x10000) {
        msg.Get(spline.face.guid);
    } else if (spline.flags & 0x8000) {
        msg >> spline.face.spot;
    }

    uint32_t val;
    msg.Get(val);
    spline.start = OsGetAsyncTimeMsPrecise() - val;

    msg.Get(spline.uint2C);
    msg.Get(spline.uint30);
    msg.Get(spline.float204);
    msg.Get(spline.float208);
    msg.Get(spline.float20C);
    msg.Get(spline.uint210);

    msg >> spline.spline;

    msg >> spline.vector1F8;

    return msg;
}
