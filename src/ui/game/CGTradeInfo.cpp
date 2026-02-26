#include "ui/game/CGTradeInfo.hpp"

uint32_t CGTradeInfo::s_playerMoney;
WOWGUID CGTradeInfo::s_tradingPlayer;

uint32_t CGTradeInfo::GetPlayerTradeMoney() {
    return CGTradeInfo::s_playerMoney;
}

WOWGUID CGTradeInfo::GetTradePartner() {
    return CGTradeInfo::s_tradingPlayer;
}
