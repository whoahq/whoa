#ifndef GLUE_C_CHARACTER_CREATION_HPP
#define GLUE_C_CHARACTER_CREATION_HPP

#include <storm/Array.hpp>
#include <cstdint>

class ChrClassesRec;
class CCharacterComponent;
class CSimpleModelFFX;
struct ComponentData;

class CCharacterCreation {
    public:
        // Static variables
        static CCharacterComponent* s_character;
        static CSimpleModelFFX* s_charCustomizeFrame;
        static float s_charFacing;
        static TSFixedArray<const ChrClassesRec*> s_classes;
        static int32_t s_existingCharacterIndex;
        static int32_t s_raceIndex;
        static TSGrowableArray<int32_t> s_races;
        static int32_t s_selectedClassID;

        // Static functions
        static void CalcClasses(int32_t raceID);
        static void CreateComponent(ComponentData* data, bool randomize);
        static void Dress();
        static int32_t GetRandomClassID();
        static void GetRandomRaceAndSex(ComponentData* data);
        static void Initialize();
        static bool IsClassValid(int32_t classID);
        static void ResetCharCustomizeInfo();
        static void SetSelectedClass(int32_t classID);
};

#endif
