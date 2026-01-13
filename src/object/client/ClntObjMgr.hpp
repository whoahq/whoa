#ifndef OBJECT_CLIENT_CLNT_OBJ_MGR_HPP
#define OBJECT_CLIENT_CLNT_OBJ_MGR_HPP

#include "object/client/CGObject_C.hpp"
#include <storm/Hash.hpp>
#include <cstdint>

class ClientConnection;

class ClntObjMgr {
    public:
        // Member variables
        TSHashTable<CGObject_C, CHashKeyGUID> m_objects;
        // TODO
        WOWGUID m_activePlayer = 0;
        uint32_t m_mapID = 0;
        ClientConnection* m_net = nullptr;
};

#endif
