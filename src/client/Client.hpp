#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include "event/Event.hpp"
#include <cstdint>

class CVar;

namespace Client {
    extern CVar* g_accountListVar;
    extern HEVENTCONTEXT g_clientEventContext;
}

void ClientPostClose(int32_t a1);

void CommonMain();

void StormInitialize();

void WowClientInit();

#endif
