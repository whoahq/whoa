#include "console/Client.hpp"
#include "CVar.hpp"

static const char* s_filename;

void ConsoleInitializeClientCommand() {
    // TODO
}

void ConsoleInitializeClientCVar(const char* filename) {
    s_filename = filename;

    CVar::Initialize();

    // TODO
    // CVar::Load(s_filename);
}
