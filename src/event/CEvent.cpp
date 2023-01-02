#include "event/CEvent.hpp"
#include "gx/Coordinate.hpp"

CCharEvent& CCharEvent::operator=(const EVENT_DATA_CHAR& data) {
    this->ch = data.ch;
    this->metaKeyState = data.metaKeyState;
    this->repeat = data.repeat;

    return *this;
}

CKeyEvent& CKeyEvent::operator=(const EVENT_DATA_KEY& data) {
    this->key = data.key;
    this->metaKeyState = data.metaKeyState;
    this->repeat = data.repeat;
    this->time = data.time;

    return *this;
}

CMouseEvent& CMouseEvent::operator=(const EVENT_DATA_MOUSE& data) {
    this->mode = data.mode;
    this->button = data.button;
    this->buttonState = data.buttonState;
    this->metaKeyState = data.metaKeyState;
    this->flags = data.flags;
    this->time = data.time;
    this->wheelDistance = data.wheelDistance;

    NDCToDDC(data.x, data.y, &this->x, &this->y);

    return *this;
}

CSizeEvent& CSizeEvent::operator=(const EVENT_DATA_SIZE& data) {
    this->w = data.w;
    this->h = data.h;

    return *this;
}
