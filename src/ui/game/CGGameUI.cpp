#include "ui/game/CGGameUI.hpp"
#include "client/Client.hpp"
#include "ui/CScriptObject.hpp"
#include "ui/FrameXML.hpp"
#include "ui/Key.hpp"
#include "ui/game/BattlefieldInfoScript.hpp"
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
#include "ui/game/ScriptEvents.hpp"
#include "ui/game/UIBindingsScript.hpp"
#include "ui/simple/CSimpleTop.hpp"
#include "util/CStatus.hpp"
#include <common/MD5.hpp>

WOWGUID CGGameUI::s_currentObjectTrack;
CScriptObject* CGGameUI::s_gameTooltip;
WOWGUID CGGameUI::s_lockedTarget;
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

    CharacterInfoRegisterScriptFunctions();

    // TODO

    BattlefieldInfoRegisterScriptFunctions();

    // TODO

    GMTicketInfoRegisterScriptFunctions();

    // TODO
}

WOWGUID& CGGameUI::GetCurrentObjectTrack() {
    return CGGameUI::s_currentObjectTrack;
}

WOWGUID& CGGameUI::GetLockedTarget() {
    return CGGameUI::s_lockedTarget;
}

void CGGameUI::Initialize() {
    // TODO

    CGGameUI::s_simpleTop = STORM_NEW(CSimpleTop);

    // TODO

    LoadScriptFunctions();

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
}

void CGGameUI::InitializeGame() {
    // TODO

    CGGameUI::Initialize();

    // TODO
}

int32_t CGGameUI::IsRaidMember(const WOWGUID& guid) {
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
