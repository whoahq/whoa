#ifndef GLUE_C_CHARACTER_SELECTION_HPP
#define GLUE_C_CHARACTER_SELECTION_HPP

#include <storm/Array.hpp>

class CSimpleModelFFX;

struct CharacterSelectionDisplay {
    // TODO
};

class CCharacterSelection {
    public:
        // Static variables
        static TSGrowableArray<CharacterSelectionDisplay> s_characterList;
        static CSimpleModelFFX* s_modelFrame;

        // Static functions
        static void RenderPrep();
        static void SetBackgroundModel(const char* modelPath);
};

#endif
