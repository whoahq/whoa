#include "object/client/CGGameObject.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGGameObject::TotalFields() {
    return CGObject::TotalFields() + 12;
}

uint32_t CGGameObject::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved() + 4;
}
