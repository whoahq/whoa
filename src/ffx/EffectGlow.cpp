#include "ffx/EffectGlow.hpp"
#include "console/CVar.hpp"

bool FFXGlowCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

EffectGlow::EffectGlow() {
    // TODO

    CVar::Register(
        "ffxGlow",
        "full screen glow effect",
        0x1,
        "1",
        &FFXGlowCallback,
        GRAPHICS
    );

    // TODO
}
