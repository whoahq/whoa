#include "component/ComponentData.hpp"
#include "component/Types.hpp"

void ComponentData::GetPreferences(CharacterPreferences* preferences) {
    if (!preferences) {
        return;
    }

    preferences->raceID = this->raceID;
    preferences->sexID = this->sexID;
    preferences->classID = this->classID;
    preferences->hairColorID = this->hairColorID;
    preferences->skinColorID = this->skinColorID;
    preferences->faceID = this->faceID;
    preferences->facialHairStyleID = this->facialHairStyleID;
    preferences->hairStyleID = this->hairStyleID;
}

void ComponentData::SetPreferences(CharacterPreferences* preferences) {
    if (!preferences) {
        return;
    }

    this->raceID = preferences->raceID;
    this->sexID = preferences->sexID;
    this->classID = preferences->classID;
    this->hairColorID = preferences->hairColorID;
    this->skinColorID = preferences->skinColorID;
    this->faceID = preferences->faceID;
    this->facialHairStyleID = preferences->facialHairStyleID;
    this->hairStyleID = preferences->hairStyleID;
}
