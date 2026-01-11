#ifndef OBJECT_CLIENT_CG_PLAYER_HPP
#define OBJECT_CLIENT_CG_PLAYER_HPP

#include <cstdint>

struct CGPlayerData {
    // TODO
};

class CGPlayer {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t GetRemoteDataSize();
        static uint32_t GetRemoteDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalRemoteFields();
        static uint32_t TotalFieldsSaved();
        static uint32_t TotalRemoteFieldsSaved();

        // Public member variables
        CGPlayerData* m_player;
        uint32_t* m_playerSaved;
};

#endif
