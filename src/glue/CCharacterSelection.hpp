#ifndef GLUE_C_CHARACTER_SELECTION_HPP
#define GLUE_C_CHARACTER_SELECTION_HPP

#include "net/Types.hpp"
#include <storm/Array.hpp>

class CSimpleModelFFX;

struct CharacterSelectionDisplay {
    CHARACTER_INFO info;
    // TODO
};

class CCharacterSelection {
    public:
        // Static variables
        static TSGrowableArray<CharacterSelectionDisplay> s_characterList;
        static float s_charFacing;
        static int32_t s_enterWorldIndex;
        static CSimpleModelFFX* s_modelFrame;
        static uint32_t s_restrictHuman;
        static uint32_t s_restrictDwarf;
        static uint32_t s_restrictGnome;
        static uint32_t s_restrictNightElf;
        static uint32_t s_restrictDraenei;
        static uint32_t s_restrictOrc;
        static uint32_t s_restrictTroll;
        static uint32_t s_restrictTauren;
        static uint32_t s_restrictUndead;
        static uint32_t s_restrictBloodElf;
        static int32_t s_selectionIndex;

        // Static functions
        static void ClearCharacterList();
        static void ClearCharacterModel();
        static void EnumerateCharactersCallback(const CHARACTER_INFO& info, void* param);
        static const CharacterSelectionDisplay* GetCharacterDisplay(int32_t index);
        static const CharacterSelectionDisplay* GetSelectedCharacter();
        static void OnGetCharacterList();
        static void RenderPrep();
        static void SetBackgroundModel(const char* modelPath);
        static void ShowCharacter();
        static void UpdateCharacterList();
};

#endif
