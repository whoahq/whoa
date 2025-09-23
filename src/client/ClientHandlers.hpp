#ifndef CLIENT_CLIENT_HANDLERS_HPP
#define CLIENT_CLIENT_HANDLERS_HPP

#include "net/Types.hpp"
#include <cstdint>

class CDataStore;

int32_t LoginVerifyWorldHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t NewWorldHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t NotifyHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t PlayedTimeHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t TransferAbortedHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

int32_t TransferPendingHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg);

#endif
