#ifndef GX_C_CAMERA_HPP
#define GX_C_CAMERA_HPP

#include <common/DataMgr.hpp>

class CRect;

class CAngle : public TManaged<float> {
    public:
        // Static functions
        static float ClampTo2Pi(float angle);

        // Member variables
        float m_cos;
        float m_sin;

        // Virtual member functions
        virtual void Set(const float& angle);

        // Member functions
        CAngle(float angle) {
            this->Set(angle);
        };
};

class CCamera : public CDataMgr {
    public:
        static const C3Vector DEFAULT_POSITION;
        static const float DEFAULT_DIST;
        static const float DEFAULT_FARZ;
        static const float DEFAULT_NEARZ;

        // Member variables
        TManaged<C3Vector> m_position;
        TManaged<C3Vector> m_target;
        TManaged<float> m_distance;
        TManaged<float> m_zFar;
        TManaged<float> m_zNear;
        CAngle m_aoa;
        CAngle m_fov;
        CAngle m_roll;
        CAngle m_rotation;

        // Member functions
        CCamera()
            : m_position(DEFAULT_POSITION)
            , m_distance(DEFAULT_DIST)
            , m_zFar(DEFAULT_FARZ)
            , m_zNear(DEFAULT_NEARZ)
            , m_aoa(0.0f)
            , m_fov(0.0f)
            , m_roll(0.0f)
            , m_rotation(0.0f)
        {
            this->m_managedArray.SetCount(9);

            this->AddManaged(&this->m_position, 7, 0x0);
            this->AddManaged(&this->m_target, 8, 0x0);
            this->AddManaged(&this->m_distance, 1, 0x0);
            this->AddManaged(&this->m_zFar, 2, 0x0);
            this->AddManaged(&this->m_zNear, 3, 0x0);
            this->AddManaged(&this->m_aoa, 0, 0x0);
            this->AddManaged(&this->m_fov, 4, 0x0);
            this->AddManaged(&this->m_roll, 5, 0x0);
            this->AddManaged(&this->m_rotation, 6, 0x0);
        }
        void SetupWorldProjection(const CRect& projectionRect, uint32_t flags);
};

#endif
