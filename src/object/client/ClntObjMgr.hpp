#ifndef OBJECT_CLIENT_CLNT_OBJ_MGR_HPP
#define OBJECT_CLIENT_CLNT_OBJ_MGR_HPP

#include <cstdint>

class ClientConnection;

class ClntObjMgr {
    public:
        // Member variables
        WOWGUID m_activePlayer = 0;
        uint32_t m_mapID = 0;
        ClientConnection* m_net = nullptr;
};

#endif
