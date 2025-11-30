#include "ffx/EffectDeath.hpp"
#include "console/CVar.hpp"

bool FFXDeathCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

EffectDeath::EffectDeath() {
    // TODO

    CVar::Register(
        "ffxDeath",
        "full screen death effect",
        0x1,
        "1",
        &FFXDeathCallback,
        GRAPHICS
    );
}
