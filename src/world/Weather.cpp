#include "world/Weather.hpp"
#include "console/CVar.hpp"

static CVar* s_useShadersCvar;

bool WeatherDensityCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

Weather::Weather() {
    // TODO

    CVar::Register(
        "weatherDensity",
        nullptr,
        0x0,
        "2",
        &WeatherDensityCallback,
        DEFAULT
    );

    s_useShadersCvar = CVar::Register(
        "useWeatherShaders",
        nullptr,
        0x0,
        "1",
        nullptr,
        DEFAULT
    );
}
