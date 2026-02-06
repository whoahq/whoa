#include "ui/game/CharacterInfoScript.hpp"
#include "db/Db.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetInventorySlotInfo(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Invalid inventory slot in GetInventorySlotInfo");
        return 0;
    }

    auto slotName = lua_tostring(L, 1);

    PaperDollItemFrameRec* slotRec = nullptr;
    for (int32_t i = 0; i < g_paperDollItemFrameDB.GetNumRecords(); i++) {
        auto paperDollItemFrameRec = g_paperDollItemFrameDB.GetRecordByIndex(i);

        if (paperDollItemFrameRec && !SStrCmpI(slotName, paperDollItemFrameRec->m_itemButtonName)) {
            slotRec = paperDollItemFrameRec;
            break;
        }
    }

    if (!slotRec) {
        luaL_error(L, "Invalid inventory slot in GetInventorySlotInfo");
        return 0;
    }

    // id
    lua_pushnumber(L, slotRec->m_slotNumber);

    // textureName
    lua_pushstring(L, slotRec->m_slotIcon);

    // checkRelic
    if (slotRec->m_slotNumber == EQUIPPED_LAST) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 3;
}

int32_t Script_GetInventoryItemsForSlot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemBroken(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemCount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemQuality(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemDurability(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemLink(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryItemGems(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_KeyRingButtonIDToInvSlotID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupInventoryItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UseInventoryItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SocketInventoryItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInventoryItemLocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PutItemInBag(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PutItemInBackpack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupBagFromSlot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CursorCanGoInSlot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowInventorySellCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetInventoryPortraitTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGuildInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInventoryAlertStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateInventoryAlertStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_OffhandHasWeapon(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasInspectHonorData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestInspectHonorData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInspectHonorData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInspectArenaTeamData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearInspectPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetWeaponEnchantInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasWandEquipped(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetInventorySlotInfo",           &Script_GetInventorySlotInfo },
    { "GetInventoryItemsForSlot",       &Script_GetInventoryItemsForSlot },
    { "GetInventoryItemTexture",        &Script_GetInventoryItemTexture },
    { "GetInventoryItemBroken",         &Script_GetInventoryItemBroken },
    { "GetInventoryItemCount",          &Script_GetInventoryItemCount },
    { "GetInventoryItemQuality",        &Script_GetInventoryItemQuality },
    { "GetInventoryItemCooldown",       &Script_GetInventoryItemCooldown },
    { "GetInventoryItemDurability",     &Script_GetInventoryItemDurability },
    { "GetInventoryItemLink",           &Script_GetInventoryItemLink },
    { "GetInventoryItemID",             &Script_GetInventoryItemID },
    { "GetInventoryItemGems",           &Script_GetInventoryItemGems },
    { "KeyRingButtonIDToInvSlotID",     &Script_KeyRingButtonIDToInvSlotID },
    { "PickupInventoryItem",            &Script_PickupInventoryItem },
    { "UseInventoryItem",               &Script_UseInventoryItem },
    { "SocketInventoryItem",            &Script_SocketInventoryItem },
    { "IsInventoryItemLocked",          &Script_IsInventoryItemLocked },
    { "PutItemInBag",                   &Script_PutItemInBag },
    { "PutItemInBackpack",              &Script_PutItemInBackpack },
    { "PickupBagFromSlot",              &Script_PickupBagFromSlot },
    { "CursorCanGoInSlot",              &Script_CursorCanGoInSlot },
    { "ShowInventorySellCursor",        &Script_ShowInventorySellCursor },
    { "SetInventoryPortraitTexture",    &Script_SetInventoryPortraitTexture },
    { "GetGuildInfo",                   &Script_GetGuildInfo },
    { "GetInventoryAlertStatus",        &Script_GetInventoryAlertStatus },
    { "UpdateInventoryAlertStatus",     &Script_UpdateInventoryAlertStatus },
    { "OffhandHasWeapon",               &Script_OffhandHasWeapon },
    { "HasInspectHonorData",            &Script_HasInspectHonorData },
    { "RequestInspectHonorData",        &Script_RequestInspectHonorData },
    { "GetInspectHonorData",            &Script_GetInspectHonorData },
    { "GetInspectArenaTeamData",        &Script_GetInspectArenaTeamData },
    { "ClearInspectPlayer",             &Script_ClearInspectPlayer },
    { "GetWeaponEnchantInfo",           &Script_GetWeaponEnchantInfo },
    { "HasWandEquipped",                &Script_HasWandEquipped },
};

void CharacterInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
