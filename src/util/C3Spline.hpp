#ifndef UTIL_C3_SPLINE_HPP
#define UTIL_C3_SPLINE_HPP

#include <common/DataStore.hpp>

// TODO move these classes to typhoon
class C3Spline {
    public:
        // TODO
};

class C3Spline_CatmullRom : public C3Spline {
    public:
        // TODO
};

// TODO move this operator>> to util/DataStore.hpp
CDataStore& operator>>(CDataStore& msg, C3Spline_CatmullRom& spline);

#endif
