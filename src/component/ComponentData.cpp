#include "component/ComponentData.hpp"
#include "component/Types.hpp"

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
