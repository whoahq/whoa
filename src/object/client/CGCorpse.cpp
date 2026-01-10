#include "object/client/CGCorpse.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGCorpse::TotalFields() {
    return CGObject::TotalFields() + 30;
}

uint32_t CGCorpse::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved() + 3;
}
