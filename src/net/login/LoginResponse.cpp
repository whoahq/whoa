#include "net/login/LoginResponse.hpp"
#include "net/grunt/Grunt.hpp"
#include <common/DataStore.hpp>
#include <storm/String.hpp>

void LoginResponse::HandleRealmData(uint32_t a2, CDataStore* msg) {
    this->m_realmList.Clear();

    if (a2 || !msg) {
        this->RealmEnumCallback(a2);
        return;
    }

    uint16_t count;
    msg->Get(count);

    this->m_realmList.SetCount(count);

    for (uint32_t i = 0; i < count; i++) {
        auto& realm = this->m_realmList[i];

        realm.id = i;

        msg->Get(realm.type);
        msg->Get(realm.locked);
        msg->Get(realm.flags);
        msg->GetString(realm.name, sizeof(realm.name));
        msg->GetString(realm.address, sizeof(realm.address));
        msg->Get(realm.population);
        msg->Get(realm.numChars);
        msg->Get(realm.category);

        if (this->GetLoginServerType() == 1) {
            msg->Get(realm.uint130);
            msg->Get(realm.uint134);
            msg->Get(realm.sort);
            msg->Get(realm.uint13C);
        } else {
            uint8_t sort;
            msg->Get(sort);
            realm.sort = sort;

            realm.uint130 = 0;
            realm.uint134 = 0;
            realm.uint13C = 0;
        }

        if (realm.flags & 0x4) {
            msg->Get(realm.majorVersion);
            msg->Get(realm.minorVersion);
            msg->Get(realm.patchVersion);
            msg->Get(realm.revision);
        } else {
            realm.majorVersion = 0;
            realm.minorVersion = 0;
            realm.patchVersion = 0;
            realm.revision = 0;
        }

        if (msg->Tell() > msg->Size()) {
            break;
        }

        if (realm.population == 600.0f) {
            realm.population = 0.0f;
            realm.flags |= 0x20;
        } else if (realm.population == 200.0f) {
            realm.population = 0.001f;
            realm.flags |= 0x40;
        } else if (realm.population == 400.f) {
            realm.population = 8.0f;
            realm.flags |= 0x80;
        }

        // TODO name manipulation
    }

    msg->Get(reinterpret_cast<uint16_t&>(this->uint10));

    // Overrun or underrun
    if (msg->Tell() > msg->Size() || !msg->IsRead()) {
        this->m_realmList.Clear();
        this->RealmEnumCallback(4);
    }

    // Success
    this->RealmEnumCallback(a2);
}

void LoginResponse::UpdateLoginStatus(LOGIN_STATE state, LOGIN_RESULT result, const char* addrStr, uint16_t flags) {
    this->m_loginState = state;
    this->m_loginResult = result;

    char stateStr[64];
    SStrCopy(stateStr, Grunt::g_LoginStateStringNames[state], sizeof(stateStr));

    char resultStr[64];
    SStrCopy(resultStr, Grunt::g_LoginResultStringNames[result], sizeof(resultStr));

    this->LoginServerStatus(state, result, addrStr, stateStr, resultStr, flags);
}
