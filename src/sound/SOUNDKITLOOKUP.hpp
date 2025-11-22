#ifndef SOUND_SOUND_KIT_LOOKUP_HPP
#define SOUND_SOUND_KIT_LOOKUP_HPP

#include <storm/Hash.hpp>

struct SOUNDKITLOOKUP : public TSHashObject<SOUNDKITLOOKUP, HASHKEY_CONSTSTRI> {
    public:
        // Member variables
        uint32_t ID;
};


#endif
