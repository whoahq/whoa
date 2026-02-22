#ifndef UI_GAME_C_G_CAMERA_HPP
#define UI_GAME_C_G_CAMERA_HPP

#include "ui/simple/CSimpleCamera.hpp"
#include "util/GUID.hpp"

class CGCamera : public CSimpleCamera {
    public:
        // Public structs
        struct CameraViewData {
            const char* m_distance;
            const char* m_pitch;
            const char* m_yaw;
        };

        // Public static variables
        static CameraViewData s_cameraViewDataDefault[];

        // Virtual public member functions
        virtual ~CGCamera() = default;
        virtual float FOV() const;
        virtual C3Vector Forward() const;
        virtual C3Vector Right() const;
        virtual C3Vector Up() const;

        // Public member functions
        CGCamera();
        const WOWGUID& GetTarget() const;
        C33Matrix ParentToWorld() const;
        void SetupWorldProjection(const CRect& projRect);
        C3Vector Target() const;

    private:
        // Private member variables
        // TODO
        WOWGUID m_target;
        // TODO
        WOWGUID m_relativeTo;
        // TODO
        int32_t m_view;
        // TODO
        float m_distance;
        float m_yaw;
        float m_pitch;
        float m_roll;
        // TODO
        float m_fovOffset;
};

void CameraRegisterCVars();

#endif
