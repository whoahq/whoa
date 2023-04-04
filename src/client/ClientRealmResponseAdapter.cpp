#include "client/ClientRealmResponseAdapter.hpp"

void AccountDataInitialize(bool a1) {
    // TODO
}

void ClientRealmResponseAdapter::HandleAuthResponse(RealmConnection* realmConnection, uint8_t authResult) {
    auto clientConnection = static_cast<ClientConnection*>(realmConnection);

    // AUTH_WAIT_QUEUE
    if (authResult == 27) {
        clientConnection->AccountLogin_Queued();
    } else {
        clientConnection->AccountLogin_Finish(authResult);
    }

    AccountDataInitialize(true);
}
