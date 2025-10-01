#include "net/grunt/ClientLink.hpp"
#include "net/connection/WowConnection.hpp"
#include "net/grunt/ClientResponse.hpp"
#include "net/grunt/Command.hpp"
#include "net/login/Types.hpp"
#include "net/srp/SRP6_Random.hpp"
#include <common/MD5.hpp>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <cstring>
#include <new>

#define SERVER_PUBLIC_KEY_LEN 32
#define SALT_LEN 32
#define PIN_SALT_LEN 16
#define SERVER_PROOF_LEN 20

Grunt::Command<Grunt::ClientLink> Grunt::s_clientCommands[] = {
    { Grunt::ClientLink::CMD_AUTH_LOGON_CHALLENGE, "ClientLink::CMD_AUTH_LOGON_CHALLENGE", &Grunt::ClientLink::CmdAuthLogonChallenge, 0 },
    { Grunt::ClientLink::CMD_AUTH_LOGON_PROOF, "ClientLink::CMD_AUTH_LOGON_PROOF", &Grunt::ClientLink::CmdAuthLogonProof, 0 },
    { Grunt::ClientLink::CMD_AUTH_RECONNECT_CHALLENGE, "ClientLink::CMD_AUTH_RECONNECT_CHALLENGE", &Grunt::ClientLink::CmdAuthReconnectChallenge, 0 },
    { Grunt::ClientLink::CMD_AUTH_RECONNECT_PROOF, "ClientLink::CMD_AUTH_RECONNECT_PROOF", &Grunt::ClientLink::CmdAuthReconnectProof, 0 },
    { Grunt::ClientLink::CMD_REALM_LIST, "ClientLink::CMD_REALM_LIST", &Grunt::ClientLink::CmdRealmList, 0 },
    { Grunt::ClientLink::CMD_XFER_INITIATE, "ClientLink::CMD_XFER_INITIATE", &Grunt::ClientLink::CmdXferInitiate, 0 },
    { Grunt::ClientLink::CMD_XFER_DATA, "ClientLink::CMD_XFER_DATA", &Grunt::ClientLink::CmdXferData, 0 },
};

Grunt::ClientLink::ClientLink(Grunt::ClientResponse& clientResponse) {
    // TODO

    this->m_clientResponse = &clientResponse;

    this->SetState(STATE_NONE);

    if (this->m_timer.m_thread.Valid()) {
        this->m_interval = 100;
        this->m_timer.Insert(*this);
    }
}

void Grunt::ClientLink::Call() {
    // TODO
    // this->CheckExpired(false);

    this->m_critSect.Enter();

    if (this->m_state == STATE_CONNECTED) {
        this->m_clientResponse->GetLogonMethod();
    } else if (this->m_state == STATE_AUTHENTICATED && !this->m_clientResponse->OnlineIdle()) {
        this->Disconnect();
    }

    this->m_critSect.Leave();
}

int32_t Grunt::ClientLink::CmdAuthLogonChallenge(CDataStore& msg) {
    uint8_t protocol;
    uint8_t result;

    if (!CanRead(msg, sizeof(protocol) + sizeof(result))) {
        return 0;
    }

    msg.Get(protocol);

    if (protocol != 0) {
        return 1;
    }

    msg.Get(result);

    // Logon challenge failure (result != 0)

    if (result != 0) {
        if (!msg.IsValid()) {
            return 1;
        }

        this->SetState(STATE_CONNECTED);

        if (result >= GRUNT_RESULT_LAST) {
            // TODO WLog error
        }

        this->m_clientResponse->LogonResult(static_cast<Grunt::Result>(result), nullptr, 0, 0);

        return 2;
    }

    // Logon challenge success (result == 0)

    uint8_t* serverPublicKey;
    uint8_t generatorLen;

    if (!CanRead(msg, SERVER_PUBLIC_KEY_LEN + sizeof(generatorLen))) {
        return 0;
    }

    msg.GetDataInSitu(reinterpret_cast<void*&>(serverPublicKey), SERVER_PUBLIC_KEY_LEN);
    msg.Get(generatorLen);

    uint8_t* generator;
    uint8_t largeSafePrimeLen;

    if (!CanRead(msg, generatorLen + sizeof(largeSafePrimeLen))) {
        return 0;
    }

    msg.GetDataInSitu(reinterpret_cast<void*&>(generator), generatorLen);
    msg.Get(largeSafePrimeLen);

    uint8_t* largeSafePrime;
    uint8_t* salt;
    uint8_t* versionChallenge;

    if (!CanRead(msg, largeSafePrimeLen + SALT_LEN + LOGIN_VERSION_CHALLENGE_LEN)) {
        return 0;
    }

    msg.GetDataInSitu(reinterpret_cast<void*&>(largeSafePrime), largeSafePrimeLen);
    msg.GetDataInSitu(reinterpret_cast<void*&>(salt), SALT_LEN);
    msg.GetDataInSitu(reinterpret_cast<void*&>(versionChallenge), LOGIN_VERSION_CHALLENGE_LEN);

    uint8_t logonFlags;

    if (!CanRead(msg, sizeof(logonFlags))) {
        return 0;
    }

    msg.Get(logonFlags);

    bool pinEnabled = logonFlags & 0x1;
    bool matrixEnabled = logonFlags & 0x2;
    bool tokenEnabled = logonFlags & 0x4;

    // PIN (0x1)

    uint32_t pinGridSeed = 0;
    uint8_t* pinSalt = nullptr;

    if (pinEnabled) {
        if (!CanRead(msg, sizeof(pinGridSeed) + PIN_SALT_LEN)) {
            return 0;
        }

        msg.Get(pinGridSeed);
        msg.GetDataInSitu(reinterpret_cast<void*&>(pinSalt), 16);
    }

    // MATRIX (0x2)

    uint8_t matrixWidth = 0;
    uint8_t matrixHeight = 0;
    uint8_t matrixDigitCount = 0;
    uint8_t matrixChallengeCount = 0;
    uint64_t matrixSeed = 0;

    if (matrixEnabled) {
        if (!CanRead(msg, sizeof(matrixWidth) + sizeof(matrixHeight) + sizeof(matrixDigitCount) + sizeof(matrixChallengeCount) + sizeof(matrixSeed))) {
            return 0;
        }

        msg.Get(matrixWidth);
        msg.Get(matrixHeight);
        msg.Get(matrixDigitCount);
        msg.Get(matrixChallengeCount);
        msg.Get(matrixSeed);

        if (matrixChallengeCount == 0) {
            return 1;
        }
    }

    // TOKEN (aka authenticator) (0x4)

    uint8_t tokenRequired = 0;

    if (tokenEnabled) {
        if (!CanRead(msg, sizeof(tokenRequired))) {
            return 0;
        }

        msg.Get(tokenRequired);
    }

    if (!msg.IsValid()) {
        return 1;
    }

    memcpy(this->m_serverPublicKey, serverPublicKey, sizeof(this->m_serverPublicKey));

    char randomSeed[16];
    // TODO
    // OsSecureRandom(randomSeed, sizeof(randomSeed));
    SRP6_Random srpRandom(randomSeed, sizeof(randomSeed));

    // Calculate proof

    auto calculateResult = this->m_srpClient.CalculateProof(
            largeSafePrime,
            largeSafePrimeLen,
            generator,
            generatorLen,
            salt,
            SALT_LEN,
            serverPublicKey,
            SERVER_PUBLIC_KEY_LEN,
            srpRandom
        );

    // Calculate proof failure

    if (calculateResult != SRP6_OK) {
        this->SetState(STATE_CONNECTED);

        this->m_clientResponse->LogonResult(GRUNT_RESULT_5, nullptr, 0, 0);

        return 2;
    }

    // Calculate proof success

    this->SetState(STATE_CONNECT_VERSION);

    this->m_clientResponse->SetPinInfo(pinEnabled, pinGridSeed, pinSalt);

    this->m_clientResponse->SetMatrixInfo(
        matrixEnabled,
        matrixWidth,
        matrixHeight,
        matrixDigitCount,
        matrixDigitCount,
        false,
        matrixChallengeCount,
        matrixSeed,
        this->m_srpClient.sessionKey,
        40
    );

    this->m_clientResponse->SetTokenInfo(tokenEnabled, tokenRequired);

    this->m_clientResponse->GetVersionProof(versionChallenge);

    return 2;
}

int32_t Grunt::ClientLink::CmdAuthLogonProof(CDataStore& msg) {
    if (!msg.IsValid()) {
        return 0;
    }

    if (msg.IsRead()) {
        return 0;
    }

    uint8_t result;
    msg.Get(result);

    // Logon proof failure (result != 0)

    if (result != 0) {
        if (result == 4) {
            // TODO
        }

        if (!msg.IsValid()) {
            return 1;
        }

        if (result != 10) {
            this->SetState(STATE_CONNECTED);

            // TODO range check on result

            this->m_clientResponse->LogonResult(static_cast<Grunt::Result>(result), nullptr, 0, 0);
        }

        return 2;
    }

    // Logon proof success (result == 0)

    uint8_t* serverProof;
    uint32_t accountFlags = 0x0;
    uint32_t surveyID;

    if (!CanRead(msg, SERVER_PROOF_LEN + sizeof(accountFlags))) {
        return 0;
    }

    msg.GetDataInSitu(reinterpret_cast<void*&>(serverProof), SERVER_PROOF_LEN);
    msg.Get(accountFlags);
    msg.Get(surveyID);

    uint16_t logonFlags = 0x0;

    if (!CanRead(msg, sizeof(logonFlags))) {
        return 0;
    }

    msg.Get(logonFlags);

    if (!msg.IsValid()) {
        return 1;
    }

    // Verify server proof

    auto verifyResult = this->m_srpClient.VerifyServerProof(serverProof, SERVER_PROOF_LEN);

    // Verify server proof failure

    if (verifyResult != SRP6_OK) {
        this->SetState(STATE_CONNECTED);

        this->m_clientResponse->LogonResult(Grunt::GRUNT_RESULT_11, nullptr, 0, 0x0);

        return 2;
    }

    // Verify server proof success

    this->m_accountFlags = accountFlags;
    // TODO
    // this->uint94 = 0;
    this->m_surveyID = surveyID;

    this->SetState(STATE_AUTHENTICATED);

    this->m_clientResponse->LogonResult(Grunt::GRUNT_RESULT_0, this->m_srpClient.sessionKey, 40, logonFlags);

    return 2;
}

int32_t Grunt::ClientLink::CmdAuthReconnectChallenge(CDataStore& msg) {
    uint8_t result;
    uint8_t* reconnectChallenge;
    uint8_t* versionChallenge;

    if (!CanRead(msg, sizeof(result) + LOGIN_RECONNECT_CHALLENGE_LEN)) {
        return 0;
    }

    msg.Get(result);

    // Reconnect challenge failure (result != 0)

    if (result != 0) {
        return 1;
    }

    // Reconnect challenge success (result == 0)

    msg.GetDataInSitu(reinterpret_cast<void*&>(reconnectChallenge), LOGIN_RECONNECT_CHALLENGE_LEN);
    msg.GetDataInSitu(reinterpret_cast<void*&>(versionChallenge), LOGIN_VERSION_CHALLENGE_LEN);

    if (!msg.IsValid()) {
        return 1;
    }

    // During reconnects, the client appears to overwrite the first half of m_serverPublicKey with
    // the received reconnect challenge
    memcpy(this->m_serverPublicKey, reconnectChallenge, LOGIN_RECONNECT_CHALLENGE_LEN);

    this->SetState(STATE_RECONNECT_VERSION);

    this->m_clientResponse->GetVersionProof(versionChallenge);

    return 2;
}

int32_t Grunt::ClientLink::CmdAuthReconnectProof(CDataStore& msg) {
    if (!msg.IsValid()) {
        return 0;
    }

    if (msg.IsRead()) {
        return 0;
    }

    uint8_t result;
    msg.Get(result);

    if (!msg.IsValid()) {
        return 1;
    }

    // Reconnect proof failure (result != 0)

    if (result != 0) {
        this->SetState(STATE_CONNECTED);

        if (result >= 33) {
            result = -1;
        }

        this->m_clientResponse->ReconnectResult(static_cast<Grunt::Result>(result), nullptr, 0,  0x0);

        return 2;
    }

    // Reconnect proof success (result == 0)

    uint16_t reconnectFlags = 0x0;

    if (!CanRead(msg, sizeof(reconnectFlags))) {
        return 0;
    }

    msg.Get(reconnectFlags);

    this->SetState(STATE_AUTHENTICATED);

    this->m_clientResponse->ReconnectResult(Grunt::GRUNT_RESULT_0, this->m_reconnectSessionKey, 40, reconnectFlags);

    return 2;
}

int32_t Grunt::ClientLink::CmdRealmList(CDataStore& msg) {
    if (msg.Tell() > msg.Size() || msg.Size() - msg.Tell() < 2) {
        return 0;
    }

    uint16_t size;
    msg.Get(size);

    if (msg.Tell() > msg.Size() || msg.Size() - msg.Tell() < size) {
        return 0;
    }

    uint32_t startData = msg.Tell();

    uint32_t padding;
    msg.Get(padding);

    uint32_t startList = msg.Tell();

    uint16_t count;
    msg.Get(count);

    for (uint32_t i = 0; i < count && msg.Tell() < msg.Size(); i++) {
        uint8_t realmType;
        msg.Get(realmType);

        uint8_t locked;
        msg.Get(locked);

        uint8_t realmFlags;
        msg.Get(realmFlags);

        char realmName[256];
        msg.GetString(realmName, sizeof(realmName));

        char realmAddress[256];
        msg.GetString(realmAddress, sizeof(realmAddress));

        float population;
        msg.Get(population);

        uint8_t numChars;
        msg.Get(numChars);

        uint8_t realmCategory;
        msg.Get(realmCategory);

        uint8_t sort;
        msg.Get(sort);

        // TODO SPECIFY_BUILD
        if (realmFlags & 0x4) {
            uint8_t major;
            msg.Get(major);

            uint8_t minor;
            msg.Get(minor);

            uint8_t patch;
            msg.Get(patch);

            uint16_t revision;
            msg.Get(revision);
        }
    }

    uint16_t padding2;
    msg.Get(padding2);

    if (msg.Tell() <= msg.Size() && msg.Tell() - startData == size) {
        uint32_t endData = msg.Tell();
        msg.Seek(startList);

        this->m_clientResponse->RealmListResult(&msg);

        msg.Seek(endData);

        return 2;
    }

    return 1;
}

int32_t Grunt::ClientLink::CmdXferData(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdXferInitiate(CDataStore& msg) {
    // TODO
    return 0;
}

void Grunt::ClientLink::Connect(const char* loginServer) {
    if (this->m_state) {
        return;
    }

    this->SetState(STATE_CONNECTING);

    auto connectionMem = SMemAlloc(sizeof(WowConnection), __FILE__, __LINE__, 0x0);
    auto connection = new (connectionMem) WowConnection(this, nullptr);

    this->m_connection = connection;
    this->m_connection->SetType(WOWC_TYPE_STREAM);

    auto port = SStrChr(loginServer, ':');
    if (port) {
        this->m_connection->Connect(loginServer, 5000);
    } else {
        this->m_connection->Connect(loginServer, 3724, 5000);
    }
}

void Grunt::ClientLink::Disconnect() {
    this->m_critSect.Enter();

    if (this->m_connection) {
        this->m_connection->Disconnect();
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::GetRealmList() {
    if (this->m_state != STATE_AUTHENTICATED) {
        return;
    }

    CDataStoreCache<1024> realmListCmd;

    uint8_t opcode = CMD_REALM_LIST;
    realmListCmd.Put(opcode);

    uint32_t padding = 0;
    realmListCmd.Put(padding);

    realmListCmd.Finalize();

    this->Send(realmListCmd);
}

void Grunt::ClientLink::LogonNewSession(const Grunt::ClientLink::Logon& logon) {
    this->SetState(STATE_AUTH_CHALLENGE);

    SStrCopy(this->m_accountName, logon.accountName, sizeof(this->m_accountName));
    SStrUpper(this->m_accountName);

    char* password = static_cast<char*>(alloca(SStrLen(logon.password) + 1));
    SStrCopy(password, logon.password, STORM_MAX_STR);
    SStrUpper(password);

    static char accountNameUnDecorated[1280];
    SStrCopy(accountNameUnDecorated, this->m_accountName, STORM_MAX_STR);
    auto decoration = const_cast<char*>(SStrChr(accountNameUnDecorated, '#'));
    if (decoration) {
        *decoration = '\0';
    }

    this->m_srpClient.BeginAuthentication(accountNameUnDecorated, password);

    CDataStoreCache<1024> clientChallenge;

    uint8_t opcode = CMD_AUTH_LOGON_CHALLENGE;
    clientChallenge.Put(opcode);

    uint8_t protocol = 8;
    clientChallenge.Put(protocol);

    this->PackLogon(clientChallenge, logon);
    clientChallenge.Finalize();

    this->Send(clientChallenge);
}

void Grunt::ClientLink::LogonStoredSession(const Grunt::ClientLink::Logon& logon) {
    this->SetState(STATE_AUTH_CHALLENGE);

    SStrCopy(this->m_accountName, logon.accountName, sizeof(this->m_accountName));
    SStrUpper(this->m_accountName);

    // logon.password is copy of session key from original session
    memcpy(this->m_reconnectSessionKey, logon.password, sizeof(this->m_reconnectSessionKey));

    CDataStoreCache<1024> clientChallenge;

    uint8_t opcode = CMD_AUTH_RECONNECT_CHALLENGE;
    clientChallenge.Put(opcode);

    uint8_t protocol = 8;
    clientChallenge.Put(protocol);

    this->PackLogon(clientChallenge, logon);

    clientChallenge.Finalize();

    this->Send(clientChallenge);
}

void Grunt::ClientLink::PackLogon(CDataStore& msg, const Logon& logon) {
    uint32_t startPos = msg.Size();
    uint16_t tmpSize = 0;
    msg.Put(tmpSize);

    msg.Put(logon.programID);
    msg.Put(logon.version[0]);
    msg.Put(logon.version[1]);
    msg.Put(logon.version[2]);
    msg.Put(logon.build);
    msg.Put(logon.processorID);
    msg.Put(logon.osID);
    msg.Put(logon.locale);
    msg.Put(logon.tz);

    msg.Put(this->m_clientIP);

    auto accountNameLen = static_cast<uint8_t>(SStrLen(this->m_accountName));
    msg.Put(accountNameLen);
    msg.PutData(this->m_accountName, accountNameLen);

    msg.Set(startPos, msg.Size() - startPos - 2);
}

void Grunt::ClientLink::ProveVersion(const uint8_t* versionChecksum) {
    STORM_ASSERT(this->m_state == STATE_CONNECT_VERSION || this->m_state == STATE_RECONNECT_VERSION);

    CDataStoreCache<1024> command;

    // TODO cd keys
    // Grunt::CdKey cdKeys;
    // if (!this->m_loginResponse->GetCdKeys(cdKeys)) {
    //     cdKeys.int0 = 0;
    // }

    if (this->m_state == STATE_CONNECT_VERSION) {
        command.Put(static_cast<uint8_t>(CMD_AUTH_LOGON_PROOF));

        SHA1_CONTEXT sha1;

        // Client public key (A)

        command.PutData(this->m_srpClient.clientPublicKey, sizeof(this->m_srpClient.clientPublicKey));

        // Client auth proof (M1)

        command.PutData(this->m_srpClient.clientProof, sizeof(this->m_srpClient.clientProof));

        // Client version proof

        uint8_t clientVersionProof[SHA1_DIGEST_SIZE];

        SHA1_Init(&sha1);
        SHA1_Update(&sha1, this->m_srpClient.clientPublicKey, sizeof(this->m_srpClient.clientPublicKey));
        SHA1_Update(&sha1, versionChecksum, LOGIN_VERSION_CHECKSUM_LEN);
        SHA1_Final(clientVersionProof, &sha1);

        command.PutData(clientVersionProof, sizeof(clientVersionProof));

        // TODO cd keys
        command.Put(static_cast<uint8_t>(0));

        // TODO pin / matrix / token fields
        uint8_t authFlags = 0x0;
        command.Put(authFlags);
    } else {
        command.Put(static_cast<uint8_t>(CMD_AUTH_RECONNECT_PROOF));

        MD5_CTX md5;
        SHA1_CONTEXT sha1;

        // Client salt

        uint8_t clientSalt[16];

        MD5Init(&md5);
        MD5Update(&md5, reinterpret_cast<uint8_t*>(this->m_accountName), SStrLen(this->m_accountName));
        char randomSeed[16];
        // TODO
        // OsSecureRandom(randomSeed, sizeof(randomSeed));
        MD5Update(&md5, reinterpret_cast<uint8_t*>(randomSeed), sizeof(randomSeed));
        MD5Final(clientSalt, &md5);

        command.PutData(clientSalt, sizeof(clientSalt));

        // Client auth proof

        uint8_t clientAuthProof[SHA1_DIGEST_SIZE];

        // Stored in m_serverPublicKey after receiving CMD_AUTH_RECONNECT_CHALLENGE
        auto reconnectChallenge = reinterpret_cast<uint8_t*>(this->m_serverPublicKey);

        SHA1_Init(&sha1);
        SHA1_Update(&sha1, reinterpret_cast<uint8_t*>(this->m_accountName), SStrLen(this->m_accountName));
        SHA1_Update(&sha1, clientSalt, sizeof(clientSalt));
        SHA1_Update(&sha1, reconnectChallenge, LOGIN_RECONNECT_CHALLENGE_LEN);
        SHA1_Update(&sha1, this->m_reconnectSessionKey, sizeof(this->m_reconnectSessionKey));
        SHA1_Final(clientAuthProof, &sha1);

        command.PutData(clientAuthProof, sizeof(clientAuthProof));

        // Client version proof

        uint8_t clientVersionProof[SHA1_DIGEST_SIZE];

        SHA1_Init(&sha1);
        SHA1_Update(&sha1, clientSalt, sizeof(clientSalt));
        SHA1_Update(&sha1, versionChecksum, LOGIN_VERSION_CHECKSUM_LEN);
        SHA1_Final(clientVersionProof, &sha1);

        command.PutData(clientVersionProof, sizeof(clientVersionProof));

        // TODO cd keys
        command.Put(static_cast<uint8_t>(0));
    }

    command.Finalize();

    this->Send(command);
}

void Grunt::ClientLink::Send(CDataStore& msg) {
    this->m_critSect.Enter();

    if (this->m_connection) {
        void* data;
        msg.GetDataInSitu(data, msg.Size());

        this->m_connection->SendRaw(static_cast<uint8_t*>(data), msg.Size(), false);
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::SetState(STATE state) {
    this->m_critSect.Enter();

    this->m_state = state;

    this->m_critSect.Leave();
}

void Grunt::ClientLink::WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {
    // TODO
}

void Grunt::ClientLink::WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr) {
    this->m_critSect.Enter();

    this->SetState(STATE_CONNECTED);

    int32_t connected = this->m_clientResponse->Connected(addr->peerAddr);

    // TODO
    // this->m_clientIP = OsNetAddrGetAddress(&addr->selfAddr, 0);

    this->m_critSect.Leave();

    if (!connected) {
        this->Disconnect();
    }
}

void Grunt::ClientLink::WCDataReady(WowConnection* conn, uint32_t timeStamp, uint8_t* data, int32_t len) {
    this->m_datastore1B0.PutArray(data, len);
    this->m_datastore1B0.Finalize();

    uint32_t pos = 0;
    if (Grunt::Command<Grunt::ClientLink>::Process(this->m_datastore1B0, Grunt::s_clientCommands, 7u, *this, pos)) {
        auto remainingBytes = this->m_datastore1B0.Size() - pos;
        this->m_datastore1B0.Seek(pos);
        void* remainingData;
        this->m_datastore1B0.GetDataInSitu(remainingData, remainingBytes);
        this->m_datastore1B0.Seek(-1);
        this->m_datastore1B0.Reset();
        this->m_datastore1B0.PutData(remainingData, remainingBytes);
    } else {
        this->m_datastore1B0.Seek(-1);
        this->m_datastore1B0.Reset();
        this->Disconnect();
    }
}
