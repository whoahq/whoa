#include "object/client/CGDynamicObject.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGDynamicObject::TotalFields() {
    return CGObject::TotalFields() + 6;
}

uint32_t CGDynamicObject::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved();
}
