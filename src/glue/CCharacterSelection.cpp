#include "glue/CCharacterSelection.hpp"
#include "client/ClientServices.hpp"
#include "glue/CGlueMgr.hpp"
#include "glue/CRealmList.hpp"
#include "model/CM2Shared.hpp"
#include "net/Connection.hpp"
#include "ui/CSimpleModelFFX.hpp"

TSGrowableArray<CharacterSelectionDisplay> CCharacterSelection::s_characterList;
CSimpleModelFFX* CCharacterSelection::s_modelFrame;
uint32_t CCharacterSelection::s_restrictHuman;
uint32_t CCharacterSelection::s_restrictDwarf;
uint32_t CCharacterSelection::s_restrictGnome;
uint32_t CCharacterSelection::s_restrictNightElf;
uint32_t CCharacterSelection::s_restrictDraenei;
uint32_t CCharacterSelection::s_restrictOrc;
uint32_t CCharacterSelection::s_restrictTroll;
uint32_t CCharacterSelection::s_restrictTauren;
uint32_t CCharacterSelection::s_restrictUndead;
uint32_t CCharacterSelection::s_restrictBloodElf;
int32_t CCharacterSelection::s_selectionIndex;

void CCharacterSelection::ClearCharacterList() {
    // TODO
}

void CCharacterSelection::ClearCharacterModel() {
    // TODO
}

void CCharacterSelection::EnumerateCharactersCallback(const CHARACTER_INFO& info, void* param) {
    auto display = CCharacterSelection::s_characterList.New();
    display->info = info;

    // TODO
}

void CCharacterSelection::OnGetCharacterList() {
    CCharacterSelection::s_characterList.SetCount(0);

    CCharacterSelection::ClearCharacterModel();

    CGlueMgr::GetCharacterList();
}

void CCharacterSelection::RenderPrep() {
    // TODO
}

void CCharacterSelection::SetBackgroundModel(const char* modelPath) {
    if (!CCharacterSelection::s_modelFrame || !modelPath || !*modelPath) {
        return;
    }

    auto model = CCharacterSelection::s_modelFrame->m_model;

    // Check if already set
    if (model && !SStrCmpI(modelPath, model->m_shared->m_filePath, STORM_MAX_STR)) {
        return;
    }

    CCharacterSelection::s_modelFrame->SetModel(modelPath);

    // TODO BYTE1(CCharacterSelection::m_modelFrame->simplemodelffx_dword510[3]) = 1;

    model = CCharacterSelection::s_modelFrame->m_model;

    if (model) {
        // TODO lighting callback + arg

        model->IsDrawable(1, 1);
    }
}

void CCharacterSelection::ShowCharacter() {
    // TODO
}

void CCharacterSelection::UpdateCharacterList() {
    // TODO

    CCharacterSelection::s_characterList.SetCount(0);

    CCharacterSelection::s_restrictHuman = 0;
    CCharacterSelection::s_restrictDwarf = 0;
    CCharacterSelection::s_restrictGnome = 0;
    CCharacterSelection::s_restrictNightElf = 0;
    CCharacterSelection::s_restrictDraenei = 0;
    CCharacterSelection::s_restrictOrc = 0;
    CCharacterSelection::s_restrictTroll = 0;
    CCharacterSelection::s_restrictTauren = 0;
    CCharacterSelection::s_restrictUndead = 0;
    CCharacterSelection::s_restrictBloodElf = 0;

    // Enumerate characters

    ClientServices::Connection()->EnumerateCharacters(CCharacterSelection::EnumerateCharactersCallback, nullptr);

    // No characters

    if (CCharacterSelection::s_characterList.Count() == 0) {
        CCharacterSelection::s_selectionIndex = 0;

        // TODO Sub4E3CD0();

        FrameScript_SignalEvent(8, "%d", CCharacterSelection::s_selectionIndex + 1);

        CCharacterSelection::ClearCharacterModel();

        FrameScript_SignalEvent(7, nullptr);

        return;
    }

    // At least one character

    CCharacterSelection::s_restrictHuman = ClientServices::Connection()->m_restrictHuman;
    CCharacterSelection::s_restrictDwarf = ClientServices::Connection()->m_restrictDwarf;
    CCharacterSelection::s_restrictGnome = ClientServices::Connection()->m_restrictGnome;
    CCharacterSelection::s_restrictNightElf = ClientServices::Connection()->m_restrictNightElf;
    CCharacterSelection::s_restrictDraenei = ClientServices::Connection()->m_restrictDraenei;
    CCharacterSelection::s_restrictOrc = ClientServices::Connection()->m_restrictOrc;
    CCharacterSelection::s_restrictTroll = ClientServices::Connection()->m_restrictTroll;
    CCharacterSelection::s_restrictTauren = ClientServices::Connection()->m_restrictTauren;
    CCharacterSelection::s_restrictUndead = ClientServices::Connection()->m_restrictUndead;
    CCharacterSelection::s_restrictBloodElf = ClientServices::Connection()->m_restrictBloodElf;

    CRealmList::s_preferredCategory = 0;

    // TODO g_lastCharacterIndex stuff
    int32_t selectionIndex = 0;

    CCharacterSelection::s_selectionIndex = selectionIndex;

    // TODO Sub4E3CD0();

    FrameScript_SignalEvent(8, "%d", CCharacterSelection::s_selectionIndex + 1);
}
