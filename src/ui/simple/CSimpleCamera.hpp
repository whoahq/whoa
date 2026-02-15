#ifndef UI_SIMPLE_C_SIMPLE_CAMERA_HPP
#define UI_SIMPLE_C_SIMPLE_CAMERA_HPP

#include <tempest/Matrix.hpp>
#include <tempest/Vector.hpp>

class CSimpleCamera {
    public:
        // Virtual public member functions
        virtual float FOV();
        virtual C3Vector Forward() const;
        virtual C3Vector Right() const;
        virtual C3Vector Up() const;

        // Public member functions
        CSimpleCamera(float nearZ, float farZ, float fov);
        void SetFacing(const C3Vector& forward);
        void SetFacing(const C3Vector& forward, const C3Vector& up);
        void SetFacing(float yaw, float pitch, float roll);
        void SetFarZ(float farZ);
        void SetFieldOfView(float fov);
        void SetNearZ(float nearZ);

    protected:
        // Protected member variables
        C3Vector m_position;
        float float10;
        C33Matrix m_facing;
        float m_nearZ;
        float m_farZ;
        float m_fov;
        float m_aspect;
};

#endif
