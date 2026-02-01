#include "ui/game/ScriptUtil.hpp"
#include "object/Client.hpp"
#include "ui/game/CGGameUI.hpp"
#include <storm/String.hpp>

bool Script_GetGUIDFromToken(const char* token, WOWGUID& guid, bool defaultToTarget) {
    auto activePlayer = static_cast<CGPlayer_C*>(ClntObjMgrObjectPtr(ClntObjMgrGetActivePlayer(), TYPE_PLAYER, __FILE__, __LINE__));

    // Null or empty token
    if (token == nullptr || *token == '\0') {
        if (defaultToTarget) {
            guid = CGGameUI::GetLockedTarget();

            return true;
        }

        return false;
    }

    guid = 0;
    auto parseToken = token;

    // player - active player
    if (!SStrCmpI(parseToken, "player", 6)) {
        parseToken += 6;

        if (activePlayer) {
            guid = activePlayer->GetGUID();
        }
    }

    // vehicle - active player's vehicle
    else if (!SStrCmpI(parseToken, "vehicle", 7)) {
        parseToken += 7;

        // TODO
    }

    // pet - active player's pet
    else if (!SStrCmpI(parseToken, "pet", 3)) {
        parseToken += 3;

        // TODO
    }

    // target - current locked target
    else if (!SStrCmpI(parseToken, "target", 6)) {
        parseToken += 6;

        guid = CGGameUI::GetLockedTarget();
    }

    // partypet1-4 - party member's pet
    else if (!SStrCmpI(parseToken, "partypet", 8)) {
        parseToken += 8;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // party1-4 - party member
    else if (!SStrCmpI(parseToken, "party", 5)) {
        parseToken += 5;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // raidpet1-40 - raid member's pet
    else if (!SStrCmpI(parseToken, "raidpet", 7)) {
        parseToken += 7;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // raid1-40 - raid member
    else if (!SStrCmpI(parseToken, "raid", 4)) {
        parseToken += 4;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // boss1-5 - boss unit
    else if (!SStrCmpI(parseToken, "boss", 4)) {
        parseToken += 4;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // arenapet1-5 - arena opponent's pet
    else if (!SStrCmpI(parseToken, "arenapet", 8)) {
        parseToken += 8;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // arena1-5 - arena opponent
    else if (!SStrCmpI(parseToken, "arena", 5)) {
        parseToken += 5;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // commentator1-N - commentator arena member
    else if (!SStrCmpI(parseToken, "commentator", 11)) {
        parseToken += 11;

        auto index = SStrToUnsigned(parseToken);
        // TODO
    }

    // mouseover - object under cursor
    else if (!SStrCmpI(parseToken, "mouseover", 9)) {
        parseToken += 9;

        // TODO
    }

    // focus - focus target
    else if (!SStrCmpI(parseToken, "focus", 5)) {
        parseToken += 5;

        // TODO
    }

    // npc - NPC interaction target
    else if (!SStrCmpI(parseToken, "npc")) {
        parseToken += 3;

        // TODO
    }

    // questnpc - quest giver NPC
    else if (!SStrCmpI(parseToken, "questnpc")) {
        parseToken += 8;

        // TODO
    }

    // none
    else if (!SStrCmpI(parseToken, "none")) {
        parseToken += 4;

        guid = -1;
    }

    // TODO ParseTrailingTokens
    // TODO Script_GetGUIDFromString
    // TODO guid -2

    return false;
}
