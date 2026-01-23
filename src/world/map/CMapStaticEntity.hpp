#ifndef WORLD_MAP_C_MAP_STATIC_ENTITY_HPP
#define WORLD_MAP_C_MAP_STATIC_ENTITY_HPP

#include "world/map/CMapBaseObj.hpp"
#include <tempest/Vector.hpp>

class CM2Model;

class CMapStaticEntity : public CMapBaseObj {
    public:
        // Member variables
        // TODO
        CM2Model* m_model = nullptr;
        // TODO
        CImVector m_ambient = {};
        CImVector m_interiorDirColor = {};
        float m_dirLightScale = 0.0f;
};

#endif
