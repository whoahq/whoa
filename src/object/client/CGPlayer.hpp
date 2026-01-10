#ifndef OBJECT_CLIENT_CG_PLAYER_HPP
#define OBJECT_CLIENT_CG_PLAYER_HPP

#include <cstdint>

struct CGPlayerData {
    // TODO
};

class CGPlayer {
    public:
        // Public static functions
        static uint32_t TotalFields();
        static uint32_t TotalRemoteFields();
        static uint32_t TotalFieldsSaved();
        static uint32_t TotalRemoteFieldsSaved();
};

#endif
