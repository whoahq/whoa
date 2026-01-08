#ifndef OBJECT_CLIENT_MESSAGE_HANDLERS_HPP
#define OBJECT_CLIENT_MESSAGE_HANDLERS_HPP

#include "net/Types.hpp"
#include <cstdint>

class CDataStore;

int32_t ObjectCompressedUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t ObjectUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t OnObjectDestroy(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

#endif
