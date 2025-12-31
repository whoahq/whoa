#ifndef GLUE_C_CHARACTER_CREATION_HPP
#define GLUE_C_CHARACTER_CREATION_HPP

#include <storm/Array.hpp>
#include <cstdint>

class ChrClassesRec;
class CCharacterComponent;
class CSimpleModelFFX;

struct CharacterPreferences;
struct ComponentData;

class CCharacterCreation {
    public:
        // Static variables
        static CCharacterComponent* s_character;
        static CSimpleModelFFX* s_charCustomizeFrame;
        static float s_charFacing;
        static CharacterPreferences* s_charPreferences[22][2];
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
        static bool IsRaceClassValid(int32_t raceID, int32_t classID);
        static void ResetCharCustomizeInfo();
        static void SavePreferences();
        static void SetCharCustomizeModel(const char* filename);
        static void SetFacing(float orientation);
        static void SetSelectedClass(int32_t classID);
        static void SetSelectedRace(int32_t raceIndex);
        static void SetSelectedSex(int32_t sexID);
};

#endif
