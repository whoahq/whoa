#ifndef OBJECT_CLIENT_UNIT_C_HPP
#define OBJECT_CLIENT_UNIT_C_HPP

#include "object/Types.hpp"

class ChrClassesRec;
class ChrRacesRec;

class CGUnit_C {
    public:
        static const char* GetDisplayClassNameFromRecord(const ChrClassesRec* classRec, UNIT_SEX sex, UNIT_SEX* displaySex);
        static const char* GetDisplayRaceNameFromRecord(const ChrRacesRec* raceRec, UNIT_SEX sex, UNIT_SEX* displaySex);
};

#endif
