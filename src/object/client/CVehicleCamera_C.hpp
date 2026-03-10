#ifndef OBJECT_CLIENT_C_VEHICLE_CAMERA_C_HPP
#define OBJECT_CLIENT_C_VEHICLE_CAMERA_C_HPP

#include <cstdint>
class CGObject_C;

class CVehicleCamera_C {
    public:
        // Public static functions
        static int32_t ConvertSmoothFacingFromRawToWorld(float& smoothFacing, CGObject_C* relativeTo);
};

#endif
