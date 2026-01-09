#include "object/client/MessageHandlers.hpp"
#include "util/Unimplemented.hpp"
#include "util/Zlib.hpp"
#include <common/DataStore.hpp>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>

int32_t ObjectCompressedUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    uint32_t origSize;
    msg->Get(origSize);

    uint32_t sourceSize = msg->Size() - msg->Tell();

    void* source;
    msg->GetDataInSitu(source, sourceSize);

    void* buffer;
    bool bufferOnStack;

    // Stack allocate buffer if original size is less than 8KB
    if (origSize >= 8192) {
        buffer = STORM_ALLOC(origSize);
        bufferOnStack = false;
    } else {
        buffer = alloca(origSize);
        bufferOnStack = true;
    }

    auto dest = buffer;
    auto destSize = origSize;

    auto zlibResult = ZlibDecompress(dest, &destSize, source, sourceSize);

    // Error during decompression

    if (zlibResult != 0) {
        if (!bufferOnStack) {
            STORM_FREE(buffer);
        }

        return 0;
    }

    // Successful decompression

    STORM_ASSERT(origSize == destSize);

    // TODO WDataStore
    CDataStore decompMsg;
    decompMsg.PutData(dest, destSize);
    decompMsg.Finalize();

    int32_t result = ObjectUpdateHandler(nullptr, SMSG_UPDATE_OBJECT, time, &decompMsg);

    if (!bufferOnStack) {
        STORM_FREE(buffer);
    }

    return result;
}

int32_t ObjectUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t OnObjectDestroy(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}
