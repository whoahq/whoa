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
