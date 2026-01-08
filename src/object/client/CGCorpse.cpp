#include "object/client/CGCorpse.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGCorpse::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved() + 3;
}
