#include "ui/game/CGGameUI.hpp"
#include "client/Client.hpp"
#include "console/CVar.hpp"
#include "object/Client.hpp"
#include "ui/CScriptObject.hpp"
#include "ui/FrameXML.hpp"
#include "ui/Key.hpp"
#include "ui/game/ActionBarScript.hpp"
#include "ui/game/BattlefieldInfoScript.hpp"
#include "ui/game/BattlenetUI.hpp"
#include "ui/game/CGCharacterModelBase.hpp"
#include "ui/game/CGCooldown.hpp"
#include "ui/game/CGDressUpModelFrame.hpp"
#include "ui/game/CGMinimapFrame.hpp"
#include "ui/game/CGQuestPOIFrame.hpp"
#include "ui/game/CGTabardModelFrame.hpp"
#include "ui/game/CGTooltip.hpp"
#include "ui/game/CGWorldFrame.hpp"
#include "ui/game/CharacterInfoScript.hpp"
#include "ui/game/GMTicketInfoScript.hpp"
#include "ui/game/GameScript.hpp"
#include "ui/game/PartyInfoScript.hpp"
#include "ui/game/RaidInfoScript.hpp"
#include "ui/game/ScriptEvents.hpp"
#include "ui/game/TradeInfoScript.hpp"
#include "ui/game/Types.hpp"
#include "ui/game/UIBindingsScript.hpp"
#include "ui/simple/CSimpleTop.hpp"
#include "util/CStatus.hpp"
#include <common/MD5.hpp>

WOWGUID CGGameUI::s_currentObjectTrack;
uint32_t CGGameUI::s_cursorMoney;
CScriptObject* CGGameUI::s_gameTooltip;
bool CGGameUI::s_inWorld;
WOWGUID CGGameUI::s_lockedTarget;
bool CGGameUI::s_loggingIn;
CSimpleTop* CGGameUI::s_simpleTop;

void LoadScriptFunctions() {
    // TODO

    CGTooltip::CreateScriptMetaTable();
    CGCooldown::CreateScriptMetaTable();
    CGMinimapFrame::CreateScriptMetaTable();
    CGCharacterModelBase::CreateScriptMetaTable();
    CGDressUpModelFrame::CreateScriptMetaTable();
    CGTabardModelFrame::CreateScriptMetaTable();
    CGQuestPOIFrame::CreateScriptMetaTable();

    GameScriptRegisterFunctions();
    UIBindingsRegisterScriptFunctions();

    // TODO

    ScriptEventsRegisterFunctions();

    // TODO

    ActionBarRegisterScriptFunctions();
    PartyInfoRegisterScriptFunctions();

    // TODO

    CharacterInfoRegisterScriptFunctions();

    // TODO

    TradeInfoRegisterScriptFunctions();

    // TODO

    BattlefieldInfoRegisterScriptFunctions();

    // TODO

    RaidInfoRegisterScriptFunctions();

    // TODO

    GMTicketInfoRegisterScriptFunctions();
    BattlenetUI_RegisterScriptFunctions();
}

void CGGameUI::EnterWorld() {
    if (CGGameUI::s_inWorld) {
        return;
    }

    CGGameUI::s_inWorld = true;

    // TODO

    if (CGGameUI::s_loggingIn) {
        CGGameUI::s_loggingIn = false;

        FrameScript_SignalEvent(SCRIPT_PLAYER_LOGIN, nullptr);

        // TODO CGLCD::Login();
    }

    FrameScript_SignalEvent(SCRIPT_PLAYER_ENTERING_WORLD, nullptr);

    // TODO
}

WOWGUID& CGGameUI::GetCurrentObjectTrack() {
    return CGGameUI::s_currentObjectTrack;
}

uint32_t CGGameUI::GetCursorMoney() {
    return CGGameUI::s_cursorMoney;
}

WOWGUID& CGGameUI::GetLockedTarget() {
    return CGGameUI::s_lockedTarget;
}

void CGGameUI::Initialize() {
    // TODO

    CGGameUI::s_loggingIn = true;

    // TODO

    CGGameUI::s_simpleTop = STORM_NEW(CSimpleTop);

    // TODO

    LoadScriptFunctions();
    ScriptEventsRegisterEvents();
    CGGameUI::RegisterGameCVars();

    // TODO

    CGGameUI::RegisterFrameFactories();

    // TODO

    CStatus status;

    // TODO

    uint8_t digest1[16];

    switch (FrameXML_CheckSignature("Interface\\FrameXML\\FrameXML.toc", "Interface\\FrameXML\\Bindings.xml", InterfaceKey, digest1)) {
        case 0: {
            status.Add(STATUS_WARNING, "FrameXML missing signature");
            ClientPostClose(10);

            break;
        }

        case 1: {
            status.Add(STATUS_WARNING, "FrameXML has corrupt signature");
            ClientPostClose(10);

            break;
        }

        case 2: {
            status.Add(STATUS_WARNING, "FrameXML is modified or corrupt");
            ClientPostClose(10);

            break;
        }

        case 3: {
            // Success
            break;
        }

        default: {
            ClientPostClose(10);

            break;
        }
    }

    MD5_CTX md5;
    MD5Init(&md5);

    // TODO file count and progress bar logic

    FrameXML_FreeHashNodes();

    FrameXML_CreateFrames("Interface\\FrameXML\\FrameXML.toc", nullptr, &md5, &status);

    // TODO CGUIBindings::s_bindings->Load("Interface\\FrameXML\\Bindings.xml", &md5, &status);

    uint8_t digest2[16];
    MD5Final(digest2, &md5);

    // TODO digest validation

    // TODO

    CGGameUI::s_gameTooltip = CScriptObject::GetScriptObjectByName("GameTooltip", CGTooltip::GetObjectType());
    STORM_ASSERT(CGGameUI::s_gameTooltip);

    // TODO

    if (ClntObjMgrGetActivePlayer()) {
        CGGameUI::EnterWorld();
    }

    // TODO
}

void CGGameUI::InitializeGame() {
    // TODO

    CGGameUI::Initialize();

    // TODO
}

bool CGGameUI::IsLoggingIn() {
    return CGGameUI::s_loggingIn;
}

int32_t CGGameUI::IsRaidMember(const WOWGUID& guid) {
    // TODO

    return false;
}

int32_t CGGameUI::IsRaidMemberOrPet(const WOWGUID& guid) {
    // TODO

    return false;
}

void CGGameUI::RegisterFrameFactories() {
    FrameXML_RegisterFactory("WorldFrame", &CGWorldFrame::Create, true);
    FrameXML_RegisterFactory("GameTooltip", &CGTooltip::Create, false);
    FrameXML_RegisterFactory("Cooldown", &CGCooldown::Create, false);
    FrameXML_RegisterFactory("Minimap", &CGMinimapFrame::Create, false);
    FrameXML_RegisterFactory("PlayerModel", &CGCharacterModelBase::Create, false);
    FrameXML_RegisterFactory("DressUpModel", &CGDressUpModelFrame::Create, false);
    FrameXML_RegisterFactory("TabardModel", &CGTabardModelFrame::Create, false);
    FrameXML_RegisterFactory("QuestPOIFrame", &CGQuestPOIFrame::Create, false);
}

void CGGameUI::RegisterGameCVars() {
    // TODO

    CVar::Register("enableCombatText", "Whether to show floating combat text", 0x10, "1", nullptr, GAME);
    CVar::Register("combatTextFloatMode", "The combat text float mode", 0x10, "1", nullptr, GAME);
    CVar::Register("fctCombatState", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctDodgeParryMiss", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctDamageReduction", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctRepChanges", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctReactives", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctFriendlyHealers", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctComboPoints", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctLowManaHealth", nullptr, 0x10, "1", nullptr, GAME);
    CVar::Register("fctEnergyGains", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctPeriodicEnergyGains", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctHonorGains", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctAuras", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctAllSpellMechanics", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctSpellMechanics", nullptr, 0x10, "0", nullptr, GAME);
    CVar::Register("fctSpellMechanicsOther", nullptr, 0x10, "0", nullptr, GAME);

    CVar::Register("xpBarText", "Whether the XP bar shows the numeric experience value", 0x10, "0", nullptr, GAME);

    CVar::Register("playerStatusText", "Whether the player portrait shows numeric health/mana values", 0x10, "0", nullptr, GAME);
    CVar::Register("petStatusText", "Whether the pet portrait shows numeric health/mana values", 0x10, "0", nullptr, GAME);
    CVar::Register("partyStatusText", "Whether the party portraits shows numeric health/mana values", 0x10, "0", nullptr, GAME);
    CVar::Register("targetStatusText", "Whether the target portrait shows numeric health/mana values", 0x10, "0", nullptr, GAME);
    CVar::Register("statusTextPercentage", "Whether numeric health/mana values are shown as raw values or percentages", 0x10, "0", nullptr, GAME);

    CVar::Register("showPartyBackground", "Show a background behind party members", 0x10, "0", nullptr, GAME);
    CVar::Register("partyBackgroundOpacity", "The opacity of the party background", 0x10, "0.5", nullptr, GAME);
    CVar::Register("hidePartyInRaid", "Whether to hide the party UI while in a raid", 0x10, "0", nullptr, GAME);
    CVar::Register("showPartyPets", "Whether to show pets in the party UI", 0x20, "1", nullptr, GAME);
    CVar::Register("showRaidRange", "Show range indicator in raid UI", 0x20, "0", nullptr, GAME);

    CVar::Register("showArenaEnemyFrames", "Show arena enemy frames while in an Arena", 0x20, "1", nullptr, GAME);
    CVar::Register("showArenaEnemyCastbar", "Show the spell enemies are casting on the Arena Enemy frames", 0x20, "1", nullptr, GAME);
    CVar::Register("showArenaEnemyPets", "Show the enemy team's pets on the ArenaEnemy frames", 0x20, "1", nullptr, GAME);

    CVar::Register("fullSizeFocusFrame", "Increases the size of the focus frame to that of the target frame", 0x20, "0", nullptr, GAME);

    // TODO
}
