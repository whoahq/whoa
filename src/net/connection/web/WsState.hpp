#ifndef NET_CONNECTION_WEB_WS_STATE_HPP
#define NET_CONNECTION_WEB_WS_STATE_HPP

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <emscripten/websocket.h>

struct WsRecvBuffer {
    uint8_t* data = nullptr;
    int32_t size = 0;
    int32_t capacity = 0;

    void append(const uint8_t* src, int32_t len) {
        if (size + len > capacity) {
            int32_t newCap = std::max(capacity ? capacity * 2 : 4096, size + len);
            data = static_cast<uint8_t*>(realloc(data, newCap));
            capacity = newCap;
        }
        memcpy(data + size, src, len);
        size += len;
    }

    int32_t read(uint8_t* dst, int32_t maxLen) {
        int32_t toRead = std::min(maxLen, size);
        if (toRead > 0) {
            memcpy(dst, data, toRead);
            size -= toRead;
            if (size > 0) {
                memmove(data, data + toRead, size);
            }
        }
        return toRead;
    }

    void clear() {
        free(data);
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

struct WsState {
    EMSCRIPTEN_WEBSOCKET_T ws = 0;
    WsRecvBuffer recvBuf;
    bool connectPending = false;
    bool closePending = false;
    bool errorPending = false;
    char connectHost[256] = {0};
};

#endif
