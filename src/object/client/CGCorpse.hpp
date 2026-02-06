#ifndef OBJECT_CLIENT_CG_CORPSE_HPP
#define OBJECT_CLIENT_CG_CORPSE_HPP

#include <cstdint>

struct CGCorpseData {
    // TODO
};

class CGCorpse {
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
        CGCorpseData* m_corpse;
        uint32_t* m_corpseSaved;

        // Protected member functions
        CGCorpseData* Corpse() const;
};

#endif
