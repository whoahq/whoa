#ifndef OBJECT_CLIENT_CG_UNIT_C_HPP
#define OBJECT_CLIENT_CG_UNIT_C_HPP

#include "object/client/CGObject_C.hpp"
#include "object/client/CGUnit.hpp"
#include "object/Types.hpp"

class ChrClassesRec;
class ChrRacesRec;

class CGUnit_C : public CGObject_C, public CGUnit {
    public:
        // Public static functions
        static const char* GetDisplayClassNameFromRecord(const ChrClassesRec* classRec, UNIT_SEX sex, UNIT_SEX* displaySex);
        static const char* GetDisplayRaceNameFromRecord(const ChrRacesRec* raceRec, UNIT_SEX sex, UNIT_SEX* displaySex);
};

#endif
