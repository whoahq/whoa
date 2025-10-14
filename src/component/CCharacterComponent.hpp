#ifndef COMPONENT_C_CHARACTER_COMPONENT_HPP
#define COMPONENT_C_CHARACTER_COMPONENT_HPP

#include <cstdint>

class CM2Model;

struct ComponentData {
    int32_t raceID = 0;
    int32_t sexID = 0;
    int32_t classID = 0;
    int32_t hairColorID = 0;
    int32_t skinID = 0;
    int32_t faceID = 0;
    int32_t facialHairStyleID = 0;
    int32_t hairStyleID = 0;
    CM2Model* model = nullptr;
    uint32_t flags = 0x0;
    uint8_t byte28 = 0;
};

class CCharacterComponent {
    public:
        // Static functions
        static CCharacterComponent* AllocComponent();

        // Member variables
        uint32_t m_flags = 0x1 | 0x2 | 0x4;
        ComponentData m_data;

        // Member functions
        void GeosRenderPrep();
        void Init(ComponentData* data, const char* a3);
        void RenderPrep(int32_t a2);
};

#endif
