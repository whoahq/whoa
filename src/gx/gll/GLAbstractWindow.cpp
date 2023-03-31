#include "gx/gll/GLAbstractWindow.h"
#include <cmath>

int32_t GLAbstractWindow::GetBackingWidth() {
    return static_cast<int32_t>(std::floor(this->GetBackingRect().size.width));
}

int32_t GLAbstractWindow::GetBackingHeight() {
    return static_cast<int32_t>(std::floor(this->GetBackingRect().size.height));
}

int32_t GLAbstractWindow::GetWidth() {
    return static_cast<int32_t>(std::floor(this->GetRect().size.width));
}

int32_t GLAbstractWindow::GetHeight() {
    return static_cast<int32_t>(std::floor(this->GetRect().size.height));
}
