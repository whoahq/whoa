#include "net/grunt/ClientLink.hpp"
#include "net/connection/WowConnection.hpp"
#include "net/grunt/ClientResponse.hpp"
#include "net/grunt/Command.hpp"
#include "net/srp/SRP6_Random.hpp"
#include <cstring>
#include <new>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

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

    this->SetState(0);

    if (this->m_timer.m_thread.Valid()) {
        this->m_interval = 100;
        this->m_timer.Insert(*this);
    }
}

void Grunt::ClientLink::Call() {
    // TODO
    // this->CheckExpired(false);

    this->m_critSect.Enter();

    if (this->m_state == 2) {
        this->m_clientResponse->GetLogonMethod();
    } else if (this->m_state == 6 && !this->m_clientResponse->OnlineIdle()) {
        this->Disconnect();
    }

    this->m_critSect.Leave();
}

int32_t Grunt::ClientLink::CmdAuthLogonChallenge(CDataStore& msg) {
    if (msg.m_read > msg.m_size || msg.m_size - msg.m_read < 2) {
        return 0;
    }

    uint8_t v30;
    msg.Get(v30);

    if (v30 != 0) {
        return 1;
    }

    uint8_t result;
    msg.Get(result);

    // Auth failure (success == 0)
    if (result != 0) {
        if (msg.m_read > msg.m_size) {
            return 1;
        }

        this->SetState(2);

        if (result >= GRUNT_RESULT_LAST) {
            // TODO WLog error
        }

        this->m_clientResponse->LogonResult(static_cast<Grunt::Result>(result), nullptr, 0, 0);

        return 2;
    }

    if (msg.m_read > msg.m_size) {
        return 0;
    }

    if (msg.m_size - msg.m_read < 33) {
        return 0;
    }

    uint8_t* serverPublicKey;
    msg.GetDataInSitu(reinterpret_cast<void*&>(serverPublicKey), 32);

    uint8_t generatorLen;
    msg.Get(generatorLen);

    // TODO
    // if (!msg.Sub8CBBF0(v31 + 1)) {
    //     return 0;
    // }

    uint8_t* generator;
    msg.GetDataInSitu(reinterpret_cast<void*&>(generator), generatorLen);

    uint8_t largeSafePrimeLen;
    msg.Get(largeSafePrimeLen);

    // TODO
    // if (!msg.sub_8CBBF0(v32 + 48)) {
    //     return 0;
    // }

    uint8_t* largeSafePrime;
    msg.GetDataInSitu(reinterpret_cast<void*&>(largeSafePrime), largeSafePrimeLen);

    uint8_t* salt;
    msg.GetDataInSitu(reinterpret_cast<void*&>(salt), 32);

    uint8_t* versionChallenge;
    msg.GetDataInSitu(reinterpret_cast<void*&>(versionChallenge), 16);

    // TODO
    // if (!msg.Sub8CBBF0(1)) {
    //     return 0;
    // }

    uint8_t logonFlags;
    msg.Get(logonFlags);

    uint32_t pinGridSeed = 0;
    uint8_t* pinSalt = nullptr;

    uint8_t matrixWidth = 0;
    uint8_t matrixHeight = 0;
    uint8_t matrixDigitCount = 0;
    uint8_t matrixChallengeCount = 0;
    uint64_t matrixSeed = 0;

    uint8_t tokenRequired = 0;

    // PIN
    if (logonFlags & 0x1) {
        // TODO
        // if (!msg.Sub8CBBF0(20)) {
        //     return 0;
        // }

        msg.Get(pinGridSeed);
        msg.GetDataInSitu(reinterpret_cast<void*&>(pinSalt), 16);
    }

    // MATRIX
    if (logonFlags & 0x2) {
        // TODO
        /*
        if (msg.Sub8CBBF0(12)) {
            msg.Get(matrixWidth);
            msg.Get(matrixHeight);
            msg.Get(matrixDigitCount);
            msg.Get(matrixChallengeCount);
            msg.Get(matrixSeed);

            if ((logonFlags & 0x2) && matrixChallengeCount == 0) {
                return 1;
            }
        } else {
            return 0;
        }
        */
    }

    // TOKEN (authenticator)
    if (logonFlags & 0x4) {
        // TODO
        // if (!msg.Sub8CBBF0(1)) {
        //     return 0;
        // }

        msg.Get(tokenRequired);
    }

    if (msg.m_read > msg.m_size) {
        return 1;
    }

    memcpy(this->m_serverPublicKey, serverPublicKey, sizeof(this->m_serverPublicKey));

    char randomSeed[16];
    // TODO
    // OsSecureRandom(randomSeed, sizeof(randomSeed));
    SRP6_Random srpRandom(randomSeed, sizeof(randomSeed));

    if (this->m_srpClient.CalculateProof(largeSafePrime, largeSafePrimeLen, generator, generatorLen, salt, 32, serverPublicKey, 32, srpRandom)) {
        this->SetState(2);
        this->m_clientResponse->LogonResult(GRUNT_RESULT_5, nullptr, 0, 0);
    } else {
        this->SetState(4);
        this->m_clientResponse->SetPinInfo(logonFlags & 0x1, pinGridSeed, pinSalt);
        // TODO
        // this->m_clientResponse->SetMatrixInfo(logonFlags & 0x2, matrixWidth, matrixHeight, matrixDigitCount, matrixDigitCount, 0, matrixChallengeCount, matrixSeed, this->m_srpClient.buf20, 40);
        this->m_clientResponse->SetTokenInfo(logonFlags & 0x4, tokenRequired);
        this->m_clientResponse->GetVersionProof(versionChallenge);
    }

    return 2;
}

int32_t Grunt::ClientLink::CmdAuthLogonProof(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdAuthReconnectChallenge(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdAuthReconnectProof(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdRealmList(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdXferData(CDataStore& msg) {
    // TODO
    return 0;
}

int32_t Grunt::ClientLink::CmdXferInitiate(CDataStore& msg) {
    // TODO
    return 0;
}

void Grunt::ClientLink::Connect(const char* a2) {
    if (this->m_state) {
        return;
    }

    this->SetState(1);

    auto connectionMem = SMemAlloc(sizeof(WowConnection), __FILE__, __LINE__, 0x0);
    auto connection = new (connectionMem) WowConnection(this, nullptr);

    this->m_connection = connection;
    this->m_connection->SetType(WOWC_TYPE_STREAM);

    auto port = SStrChr(a2, ':');
    if (port) {
        this->m_connection->Connect(a2, 5000);
    } else {
        this->m_connection->Connect(a2, 3724, 5000);
    }
}

void Grunt::ClientLink::Disconnect() {
    this->m_critSect.Enter();

    if (this->m_connection) {
        this->m_connection->Disconnect();
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::LogonNewSession(const Grunt::ClientLink::Logon& logon) {
    this->SetState(3);

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

    uint8_t opcode = 0;
    clientChallenge.Put(opcode);

    uint8_t protocol = 8;
    clientChallenge.Put(protocol);

    this->PackLogon(clientChallenge, logon);
    clientChallenge.Finalize();

    this->Send(clientChallenge);
}

void Grunt::ClientLink::PackLogon(CDataStore& msg, const Logon& logon) {
    uint32_t startPos = msg.m_size;
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

    msg.Set(startPos, msg.m_size - startPos - 2);
}

void Grunt::ClientLink::ProveVersion(const uint8_t* versionChecksum) {
    CDataStoreCache<1024> command;

    // TODO cd keys
    // Grunt::CdKey cdKeys;
    // if (!this->m_loginResponse->GetCdKeys(cdKeys)) {
    //     cdKeys.int0 = 0;
    // }

    if (this->m_state == 4) {
        command.Put(static_cast<uint8_t>(CMD_AUTH_LOGON_PROOF));
        command.PutData(this->m_srpClient.clientPublicKey, sizeof(this->m_srpClient.clientPublicKey));
        command.PutData(this->m_srpClient.clientProof, sizeof(this->m_srpClient.clientProof));

        uint8_t versionProof[SHA1_DIGEST_SIZE];
        SHA1_CONTEXT ctx;
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, this->m_srpClient.clientPublicKey, sizeof(this->m_srpClient.clientPublicKey));
        SHA1_Update(&ctx, versionChecksum, 20);
        SHA1_Final(versionProof, &ctx);
        command.PutData(versionProof, sizeof(versionProof));

        // TODO cd keys
        command.Put(static_cast<uint8_t>(0));

        // TODO pin / matrix / token fields
        uint8_t authFlags = 0x0;
        command.Put(authFlags);
    } else {
        // TODO
    }

    command.Finalize();

    this->Send(command);
}

void Grunt::ClientLink::Send(CDataStore& msg) {
    this->m_critSect.Enter();

    if (this->m_connection) {
        void* data;
        msg.GetDataInSitu(data, msg.m_size);

        this->m_connection->SendRaw(static_cast<uint8_t*>(data), msg.m_size, false);
    }

    this->m_critSect.Leave();
}

void Grunt::ClientLink::SetState(int32_t state) {
    this->m_critSect.Enter();

    this->m_state = state;

    this->m_critSect.Leave();
}

void Grunt::ClientLink::WCCantConnect(WowConnection* conn, uint32_t timeStamp, NETCONNADDR* addr) {
    // TODO
}

void Grunt::ClientLink::WCConnected(WowConnection* conn, WowConnection* inbound, uint32_t timeStamp, const NETCONNADDR* addr) {
    this->m_critSect.Enter();

    this->SetState(2);

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
        auto remainingBytes = this->m_datastore1B0.m_size - pos;
        this->m_datastore1B0.m_read = pos;
        void* remainingData;
        this->m_datastore1B0.GetDataInSitu(remainingData, remainingBytes);
        this->m_datastore1B0.m_read = -1;
        this->m_datastore1B0.Reset();
        this->m_datastore1B0.PutData(remainingData, remainingBytes);
    } else {
        this->m_datastore1B0.m_read = -1;
        this->m_datastore1B0.Reset();
        this->Disconnect();
    }
}
