#ifndef EVENT_C_EVENT_HPP
#define EVENT_C_EVENT_HPP

#include "event/Types.hpp"
#include <cstdint>
#include <common/Ref.hpp>

class CEvent : public TRefCnt {
    public:
        // Member variables
        uint32_t id;
        void* param;
};

class CCharEvent : public CEvent, public EVENT_DATA_CHAR {
    public:
        // Member functions
        CCharEvent& operator=(const EVENT_DATA_CHAR&);
};

class CKeyEvent : public CEvent, public EVENT_DATA_KEY {
    public:
        // Member functions
        CKeyEvent& operator=(const EVENT_DATA_KEY&);
};

class CMouseEvent : public CEvent, public EVENT_DATA_MOUSE {
    public:
        // Member functions
        CMouseEvent& operator=(const EVENT_DATA_MOUSE&);
};

class CSizeEvent : public CEvent, public EVENT_DATA_SIZE {
    public:
        // Member functions
        CSizeEvent& operator=(const EVENT_DATA_SIZE&);
};

#endif
