#ifndef OBJECT_CLIENT_CG_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_OBJECT_C_HPP

#include "object/Types.hpp"
#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGObject.hpp"
#include "util/GUID.hpp"
#include <storm/Hash.hpp>
#include <storm/List.hpp>

class CGObject_C : public CGObject, public TSHashObject<CGObject_C, CHashKeyGUID> {
    public:
        // Public member variables
        TSLink<CGObject_C> m_link;
        // TODO
        uint32_t m_lockCount        : 16;
        uint32_t m_disabled         : 1;
        uint32_t m_inReenable       : 1;
        uint32_t m_postInited       : 1;
        uint32_t m_flag19           : 1;
        uint32_t m_disablePending   : 1;

        // Public member functions
        CGObject_C() = default;
        CGObject_C(uint32_t time, CClientObjCreate& objCreate);
        void AddWorldObject();
        int32_t IsInReenable();
        void Reenable();
        void SetBlock(uint32_t block, uint32_t value);
        void SetDisablePending(int32_t pending);
        void SetStorage(uint32_t* storage, uint32_t* saved);
        void SetTypeID(OBJECT_TYPE_ID typeID);
};

#endif
