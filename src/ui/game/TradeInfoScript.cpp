#include "ui/game/TradeInfoScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_CloseTrade(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClickTradeButton(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClickTargetTradeButton(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTradeTargetItemInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTradeTargetItemLink(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTradePlayerItemInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTradePlayerItemLink(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptTrade(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelTradeAccept(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPlayerTradeMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTargetTradeMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupTradeMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AddTradeMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetTradeMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "CloseTrade",             &Script_CloseTrade },
    { "ClickTradeButton",       &Script_ClickTradeButton },
    { "ClickTargetTradeButton", &Script_ClickTargetTradeButton },
    { "GetTradeTargetItemInfo", &Script_GetTradeTargetItemInfo },
    { "GetTradeTargetItemLink", &Script_GetTradeTargetItemLink },
    { "GetTradePlayerItemInfo", &Script_GetTradePlayerItemInfo },
    { "GetTradePlayerItemLink", &Script_GetTradePlayerItemLink },
    { "AcceptTrade",            &Script_AcceptTrade },
    { "CancelTradeAccept",      &Script_CancelTradeAccept },
    { "GetPlayerTradeMoney",    &Script_GetPlayerTradeMoney },
    { "GetTargetTradeMoney",    &Script_GetTargetTradeMoney },
    { "PickupTradeMoney",       &Script_PickupTradeMoney },
    { "AddTradeMoney",          &Script_AddTradeMoney },
    { "SetTradeMoney",          &Script_SetTradeMoney },
};

void TradeInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
