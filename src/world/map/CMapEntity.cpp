#include "world/map/CMapEntity.hpp"

CMapEntity::CMapEntity() {
    this->m_type |= CMapBaseObj::Type_Entity;
}
