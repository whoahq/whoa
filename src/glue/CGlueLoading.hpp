#ifndef GLUE_C_GLUE_LOADING_HPP
#define GLUE_C_GLUE_LOADING_HPP

#include <cstdint>

class CCharacterComponent;

class CGlueLoading {
    public:
        // Static variables
        static int32_t s_loadState;

        // Static functions
        static void StartLoad(CCharacterComponent* component, int32_t a2);
};

#endif
