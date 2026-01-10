#include "object/client/CGUnit.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGUnit::TotalFields() {
    return CGObject::TotalFields() + 142;
}

uint32_t CGUnit::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved() + 123;
}
