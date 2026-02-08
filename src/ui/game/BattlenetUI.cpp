#include "ui/game/BattlenetUI.hpp"
#include "client/ClientServices.hpp"
#include "net/Login.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace BattlenetUI {

int32_t Script_BNGetInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumFriends(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetFriendInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetFriendInfoByID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumFriendToons(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetFriendToonInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetToonInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNRemoveFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetFriendNote(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetSelectedFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetSelectedFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumFriendInvites(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetFriendInviteInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSendFriendInvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSendFriendInviteByID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNAcceptFriendInvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNDeclineFriendInvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNReportFriendInvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetAFK(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetDND(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetCustomMessage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetCustomMessageTable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSendWhisper(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNCreateConversation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNInviteToConversation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNLeaveConversation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSendConversationMessage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumConversationMembers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetConversationMemberInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetConversationInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNListConversation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumBlocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetBlockedInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNIsBlocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetBlocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetSelectedBlock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetSelectedBlock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumBlockedToons(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetBlockedToonInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNIsToonBlocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetToonBlocked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetSelectedToonBlock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetSelectedToonBlock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNReportPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNConnected(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNFeaturesEnabledAndConnected(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsBNLogin(lua_State* L) {
    if (ClientServices::LoginConnection() && ClientServices::LoginConnection()->GetLoginServerType() == 1) {
        lua_pushboolean(L, true);
    } else {
        lua_pushboolean(L, false);
    }

    return 1;
}

int32_t Script_BNFeaturesEnabled(lua_State* L) {
    // TODO real implementation
    lua_pushboolean(L, false);

    return 1;
}

int32_t Script_BNRequestFOFInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetNumFOF(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetFOFInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNSetMatureLanguageFilter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetMatureLanguageFilter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNIsSelf(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNIsFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BNGetMaxPlayersInConversation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

static FrameScript_Method s_ScriptFunctions[] = {
    { "BNGetInfo",                      &Script_BNGetInfo },
    { "BNGetNumFriends",                &Script_BNGetNumFriends },
    { "BNGetFriendInfo",                &Script_BNGetFriendInfo },
    { "BNGetFriendInfoByID",            &Script_BNGetFriendInfoByID },
    { "BNGetNumFriendToons",            &Script_BNGetNumFriendToons },
    { "BNGetFriendToonInfo",            &Script_BNGetFriendToonInfo },
    { "BNGetToonInfo",                  &Script_BNGetToonInfo },
    { "BNRemoveFriend",                 &Script_BNRemoveFriend },
    { "BNSetFriendNote",                &Script_BNSetFriendNote },
    { "BNSetSelectedFriend",            &Script_BNSetSelectedFriend },
    { "BNGetSelectedFriend",            &Script_BNGetSelectedFriend },
    { "BNGetNumFriendInvites",          &Script_BNGetNumFriendInvites },
    { "BNGetFriendInviteInfo",          &Script_BNGetFriendInviteInfo },
    { "BNSendFriendInvite",             &Script_BNSendFriendInvite },
    { "BNSendFriendInviteByID",         &Script_BNSendFriendInviteByID },
    { "BNAcceptFriendInvite",           &Script_BNAcceptFriendInvite },
    { "BNDeclineFriendInvite",          &Script_BNDeclineFriendInvite },
    { "BNReportFriendInvite",           &Script_BNReportFriendInvite },
    { "BNSetAFK",                       &Script_BNSetAFK },
    { "BNSetDND",                       &Script_BNSetDND },
    { "BNSetCustomMessage",             &Script_BNSetCustomMessage },
    { "BNGetCustomMessageTable",        &Script_BNGetCustomMessageTable },
    { "BNSetFocus",                     &Script_BNSetFocus },
    { "BNSendWhisper",                  &Script_BNSendWhisper },
    { "BNCreateConversation",           &Script_BNCreateConversation },
    { "BNInviteToConversation",         &Script_BNInviteToConversation },
    { "BNLeaveConversation",            &Script_BNLeaveConversation },
    { "BNSendConversationMessage",      &Script_BNSendConversationMessage },
    { "BNGetNumConversationMembers",    &Script_BNGetNumConversationMembers },
    { "BNGetConversationMemberInfo",    &Script_BNGetConversationMemberInfo },
    { "BNGetConversationInfo",          &Script_BNGetConversationInfo },
    { "BNListConversation",             &Script_BNListConversation },
    { "BNGetNumBlocked",                &Script_BNGetNumBlocked },
    { "BNGetBlockedInfo",               &Script_BNGetBlockedInfo },
    { "BNIsBlocked",                    &Script_BNIsBlocked },
    { "BNSetBlocked",                   &Script_BNSetBlocked },
    { "BNSetSelectedBlock",             &Script_BNSetSelectedBlock },
    { "BNGetSelectedBlock",             &Script_BNGetSelectedBlock },
    { "BNGetNumBlockedToons",           &Script_BNGetNumBlockedToons },
    { "BNGetBlockedToonInfo",           &Script_BNGetBlockedToonInfo },
    { "BNIsToonBlocked",                &Script_BNIsToonBlocked },
    { "BNSetToonBlocked",               &Script_BNSetToonBlocked },
    { "BNSetSelectedToonBlock",         &Script_BNSetSelectedToonBlock },
    { "BNGetSelectedToonBlock",         &Script_BNGetSelectedToonBlock },
    { "BNReportPlayer",                 &Script_BNReportPlayer },
    { "BNConnected",                    &Script_BNConnected },
    { "BNFeaturesEnabledAndConnected",  &Script_BNFeaturesEnabledAndConnected },
    { "IsBNLogin",                      &Script_IsBNLogin },
    { "BNFeaturesEnabled",              &Script_BNFeaturesEnabled },
    { "BNRequestFOFInfo",               &Script_BNRequestFOFInfo },
    { "BNGetNumFOF",                    &Script_BNGetNumFOF },
    { "BNGetFOFInfo",                   &Script_BNGetFOFInfo },
    { "BNSetMatureLanguageFilter",      &Script_BNSetMatureLanguageFilter },
    { "BNGetMatureLanguageFilter",      &Script_BNGetMatureLanguageFilter },
    { "BNIsSelf",                       &Script_BNIsSelf },
    { "BNIsFriend",                     &Script_BNIsFriend },
    { "BNGetMaxPlayersInConversation",  &Script_BNGetMaxPlayersInConversation },
};

}

void BattlenetUI_RegisterScriptFunctions() {
    for (auto& func : BattlenetUI::s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
