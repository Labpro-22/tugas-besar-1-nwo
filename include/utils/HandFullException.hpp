#pragma once

#include "utils/NimonspoliException.hpp"

class HandFullException : public NimonspoliException {
public:
    HandFullException() 
        : NimonspoliException("Slot kartu sudah penuh (maksimal 3 kartu).") {}
};