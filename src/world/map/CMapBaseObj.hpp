#ifndef WORLD_MAP_C_MAP_BASE_OBJ_HPP
#define WORLD_MAP_C_MAP_BASE_OBJ_HPP

#include <storm/List.hpp>
#include <cstdint>

class CM2Lighting;
class CMapBaseObj;

class CMapBaseObjLink {
    public:
        // Member variables
        uint32_t memHandle;
        CMapBaseObj* owner;
        CMapBaseObj* ref;
        TSLink<CMapBaseObjLink> refLink;
        TSLink<CMapBaseObjLink> ownerLink;
};

class CMapBaseObj {
    friend class CMap;
    friend class CWorld;

    public:
        // Enums
        enum {
            Type_BaseObj        = 0x1,
            Type_Area           = 0x2,
            Type_Chunk          = 0x4,
            Type_MapObjDef      = 0x8,
            Type_MapObjDefGroup = 0x10,
            Type_Entity         = 0x20,
            Type_DoodadDef      = 0x40,
            Type_Light          = 0x80,
            Type_100            = 0x100,
            Type_200            = 0x200,
        };

        // Public member variables
        uint32_t m_memHandle;
        uint32_t m_flags = 0x0;
        TSLink<CMapBaseObj> m_lameAssLink;
        STORM_EXPLICIT_LIST(CMapBaseObjLink, ownerLink) m_parentLinkList;

        // TODO

        // Public virtual member functions
        virtual ~CMapBaseObj() = default;
        virtual void SelectLights(CM2Lighting* lighting) {};
        virtual void SelectUnderwater(CM2Lighting* lighting) {};

        // Public member functions
        uint32_t GetType();

    protected:
        // Protected member variables
        uint16_t m_type = Type_BaseObj;
};

#endif
