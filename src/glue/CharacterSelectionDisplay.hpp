#ifndef GLUE_CHARACTER_SELECTION_DISPLAY_HPP
#define GLUE_CHARACTER_SELECTION_DISPLAY_HPP

#include "net/Types.hpp"

class CCharacterComponent;
class CM2Model;

class CharacterSelectionDisplay {
    public:
        // Member variables
        CHARACTER_INFO m_info;
        CCharacterComponent* m_component = nullptr;
        CM2Model* m_petModel = nullptr;
        // TODO

        // Member functions
        void CreateModelData();
};

#endif
