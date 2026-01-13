#include "object/movement/CMoveSpline.hpp"
#include "util/DataStore.hpp"
#include <common/Time.hpp>

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
