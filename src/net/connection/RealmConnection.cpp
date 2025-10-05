#include "net/connection/RealmConnection.hpp"
#include "net/connection/RealmResponse.hpp"
#include "net/Types.hpp"
#include <common/DataStore.hpp>
#include <common/SHA1.hpp>
#include <storm/String.hpp>

SCritSect RealmConnection::s_AllRealmConnectionsCrit;
STORM_LIST(RealmConnection::REALMCONNECTIONNODE) RealmConnection::s_AllRealmConnections;

int32_t RealmConnection::MessageHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    auto connection = static_cast<RealmConnection*>(param);
    int32_t result = 0;

    switch (msgId) {
    case SMSG_AUTH_RESPONSE: {
        result = connection->HandleAuthResponse(msgId, time, msg);
        break;
    }

    case SMSG_CREATE_CHAR: {
        // TODO
        break;
    }

    case SMSG_ENUM_CHARACTERS_RESULT: {
        result = connection->HandleCharEnum(msgId, time, msg);
        break;
    }

    case SMSG_DELETE_CHAR: {
        // TODO
        break;
    }

    case SMSG_CHARACTER_LOGIN_FAILED: {
        // TODO
        break;
    }

    case SMSG_ADDON_INFO: {
        // TODO
        break;
    }

    case SMSG_LOGOUT_CANCEL_ACK: {
        // TODO
        break;
    }

    case SMSG_LOGOUT_COMPLETE: {
        // TODO
        break;
    }

    case SMSG_LOGOUT_RESPONSE: {
        // TODO
        break;
    }

    default:
        break;
    }

    if (!msg->IsRead()) {
        // TODO
    }

    return result;
}

void RealmConnection::PollNet() {
    RealmConnection::s_AllRealmConnectionsCrit.Enter();

    for (auto node = RealmConnection::s_AllRealmConnections.Head(); node; node = RealmConnection::s_AllRealmConnections.Next(node)) {
        node->connection->PollEventQueue();
    }

    RealmConnection::s_AllRealmConnectionsCrit.Leave();
}

RealmConnection::RealmConnection(RealmResponse* realmResponse) {
    this->m_realmResponse = realmResponse;

    // TODO

    this->Initialize();

    this->SetMessageHandler(SMSG_AUTH_CHALLENGE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_AUTH_RESPONSE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_ADDON_INFO, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_ENUM_CHARACTERS_RESULT, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CREATE_CHAR, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CHARACTER_LOGIN_FAILED, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_COMPLETE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_CANCEL_ACK, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_LOGOUT_RESPONSE, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_DELETE_CHAR, &RealmConnection::MessageHandler, this);
    this->SetMessageHandler(SMSG_CACHE_VERSION, &RealmConnection::MessageHandler, this);

    RealmConnection::s_AllRealmConnectionsCrit.Enter();
    auto node = RealmConnection::s_AllRealmConnections.NewNode(2, 0, 0x0);
    node->connection = this;
    RealmConnection::s_AllRealmConnectionsCrit.Leave();
}

int32_t RealmConnection::HandleAuthChallenge(AuthenticationChallenge* challenge) {
    // TODO

    // TODO switch to WDataStore
    CDataStore msg;

    uint32_t localChallenge;

    msg.Put(static_cast<uint32_t>(CMSG_AUTH_SESSION));

    msg.Put(static_cast<uint32_t>(12340));
    msg.Put(static_cast<uint32_t>(this->GetLoginData().m_loginServerID));
    msg.PutString(this->GetLoginData().m_account);
    msg.Put(static_cast<uint32_t>(this->GetLoginData().m_loginServerType));

    // TODO
    msg.Put(localChallenge);

    // TODO
    msg.Put(static_cast<uint32_t>(0));
    msg.Put(static_cast<uint32_t>(0));
    msg.Put(static_cast<uint32_t>(1));

    // TODO
    msg.Put(static_cast<uint64_t>(0));

    uint32_t msgId = 0;

    SHA1_CONTEXT ctx;
    SHA1_Init(&ctx);

    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(this->GetLoginData().m_account), SStrLen(this->GetLoginData().m_account));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&msgId), sizeof(msgId));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&localChallenge), sizeof(localChallenge));
    SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(&challenge->uint0), sizeof(challenge->uint0));
    SHA1_Update(&ctx, this->GetLoginData().m_sessionKey, sizeof(this->GetLoginData().m_sessionKey));

    uint8_t clientProof[SHA1_DIGEST_SIZE];
    SHA1_Final(clientProof, &ctx);

    msg.PutData(clientProof, sizeof(clientProof));

    // TODO addons
    msg.Put(static_cast<uint32_t>(0));

    msg.Finalize();

    this->Send(&msg);

    return 1;
}

int32_t RealmConnection::HandleAuthResponse(uint32_t msgId, uint32_t time, CDataStore* msg) {
    if (this->m_realmResponse) {
        this->m_realmResponse->GameServerResult(this, "SMSG_AUTH_RESPONSE", nullptr, nullptr);
    }

    uint8_t authResult;
    msg->Get(authResult);

    // AUTH_OK or AUTH_WAIT_QUEUE
    if (authResult == 12 || authResult == 27) {
        // AUTH_OK
        if (authResult == 12) {
            this->m_authenticated = 1;
        }

        if (msg->Size() - msg->Tell() >= 10 + (authResult == 27 ? 5 : 0)) {
            msg->Get(this->m_billingTimeRemaining);
            msg->Get(this->m_billingFlags);
            msg->Get(this->m_billingTimeRested);
            msg->Get(this->m_accountExpansion);
        }

        // AUTH_WAIT_QUEUE
        if (authResult == 27) {
            msg->Get(this->m_queuePosition);

            uint8_t freeCharacterMigration;
            msg->Get(freeCharacterMigration);
            this->m_freeCharacterMigration = freeCharacterMigration;
        }
    }

    this->m_realmResponse->HandleAuthResponse(this, authResult);

    return 1;
}

int32_t RealmConnection::HandleCharEnum(uint32_t msgId, uint32_t time, CDataStore* msg) {
    if (this->m_realmResponse) {
        this->m_realmResponse->GameServerResult(this, "SMSG_CHAR_ENUM", nullptr, nullptr);
    }

    int32_t characterLimitExceeded = 0;

    uint8_t count;
    msg->Get(count);

    if (count > 10) {
        count = 0;
        characterLimitExceeded = 1;
    }

    this->m_characterList.Clear();
    this->m_characterList.SetCount(count);

    for (uint32_t i = 0; i < count; i++) {
        auto& character = this->m_characterList[i];

        msg->Get(character.guid);

        msg->GetString(character.name, 48);

        msg->Get(character.raceID);
        msg->Get(character.classID);
        msg->Get(character.sexID);

        msg->Get(character.skinID);
        msg->Get(character.faceID);
        msg->Get(character.hairStyleID);
        msg->Get(character.hairColorID);
        msg->Get(character.facialHairStyleID);

        msg->Get(character.experienceLevel);

        msg->Get(character.zoneID);
        msg->Get(character.mapID);
        msg->Get(character.position.x);
        msg->Get(character.position.y);
        msg->Get(character.position.z);

        msg->Get(character.guildID);

        msg->Get(character.flags);
        msg->Get(character.customizeFlags);
        msg->Get(character.firstLogin);

        msg->Get(character.petDisplayInfoID);
        msg->Get(character.petExperienceLevel);
        msg->Get(character.petCreatureFamilyID);

        for (auto& item : character.items) {
            msg->Get(item.displayID);
            msg->Get(item.type);
            msg->Get(item.auraID);
        }
    }

    int32_t listSuccess = 0;

    if (!characterLimitExceeded) {
        if (!msg->IsRead()) {
            // TODO what are these fields?
            uint32_t unknown;
            for (uint32_t i = 0; i < 10; i++) {
                msg->Get(unknown);
            }
        }

        if (msg->IsRead()) {
            listSuccess = 1;
        }
    }

    if (!listSuccess) {
        this->m_characterList.Clear();
    }

    this->m_realmResponse->CharacterListReceived(this, this->m_characterList, listSuccess);

    return 1;
}

void RealmConnection::RequestCharacterEnum() {
    CDataStore msg;

    msg.Put(static_cast<uint32_t>(CMSG_ENUM_CHARACTERS));

    msg.Finalize();

    this->Send(&msg);
}

void RealmConnection::SetSelectedRealm(uint32_t a2, uint32_t a3, uint32_t a4) {
    // TODO
}
