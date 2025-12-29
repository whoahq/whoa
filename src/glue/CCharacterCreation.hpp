#ifndef GLUE_C_CHARACTER_CREATION_HPP
#define GLUE_C_CHARACTER_CREATION_HPP

#include <cstdint>

class CCharacterComponent;
class CSimpleModelFFX;
struct ComponentData;

class CCharacterCreation {
    public:
        // Static variables
        static CCharacterComponent* s_character;
        static CSimpleModelFFX* s_charCustomizeFrame;
        static int32_t s_existingCharacterIndex;
        static int32_t s_raceIndex;
        static int32_t s_selectedClassID;

        // Static functions
        static void CalcClasses(int32_t raceID);
        static void CreateComponent(ComponentData* data, bool randomize);
        static void Dress();
        static int32_t GetRandomClassID();
        static void GetRandomRaceAndSex(ComponentData* data);
        static void ResetCharCustomizeInfo();
        static void SetSelectedClass(int32_t classID);
};

#endif
