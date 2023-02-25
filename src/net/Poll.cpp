#include "net/Poll.hpp"
#include "net/connection/RealmConnection.hpp"

int32_t PollNet(const void* a1, void* a2) {
    RealmConnection::PollNet();

    // TODO

    return 1;
}
