#ifndef OBJECT_CLIENT_CG_GAME_OBJECT_HPP
#define OBJECT_CLIENT_CG_GAME_OBJECT_HPP

#include <cstdint>

struct CGGameObjectData {
    // TODO
};

class CGGameObject {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

    protected:
        // Protected member variables
        CGGameObjectData* m_gameObj;
        uint32_t* m_gameObjSaved;

        // Protected member functions
        CGGameObjectData* GameObject() const;
};

#endif
