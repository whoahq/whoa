#ifndef OBJECT_CLIENT_CG_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_OBJECT_C_HPP

#include "object/Types.hpp"
#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGObject.hpp"
#include "util/GUID.hpp"
#include <storm/Hash.hpp>
#include <storm/List.hpp>

class CM2Model;

class CGObject_C : public CGObject, public TSHashObject<CGObject_C, CHashKeyGUID> {
    public:
        // Public member variables
        TSLink<CGObject_C> m_link;
        uint32_t m_disableTimeMs;
        // TODO
        CM2Model* m_model;
        // TODO
        uint32_t m_lockCount        : 16;
        uint32_t m_disabled         : 1;
        uint32_t m_inReenable       : 1;
        uint32_t m_postInited       : 1;
        uint32_t m_flag19           : 1;
        uint32_t m_disablePending   : 1;
        // TODO

        // Virtual public member functions
        virtual ~CGObject_C();
        virtual void Disable();
        void Reenable();
        void PostReenable();
        virtual void HandleOutOfRange(OUT_OF_RANGE_TYPE type) {};
        // TODO
        virtual int32_t CanHighlight();

        // Public member functions
        CGObject_C() = default;
        CGObject_C(uint32_t time, CClientObjCreate& objCreate);
        void AddWorldObject();
        int32_t IsInReenable();
        int32_t IsObjectLocked();
        void PostInit(uint32_t time, const CClientObjCreate& init, bool a4);
        void SetBlock(uint32_t block, uint32_t value);
        void SetDisablePending(int32_t pending);
        void SetObjectLocked(int32_t locked);
        void SetStorage(uint32_t* storage, uint32_t* saved);
        void SetTypeID(OBJECT_TYPE_ID typeID);
};

#endif
