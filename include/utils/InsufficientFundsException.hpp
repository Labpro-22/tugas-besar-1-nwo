#pragma once

#include "utils/NimonspoliException.hpp"

class InsufficientFundsException : public NimonspoliException {
private:
    int required;
    int available;
public:
    InsufficientFundsException(int req, int avl) 
        : NimonspoliException("Dana tidak mencukupi untuk transaksi."), 
          required(req), available(avl) {}
    
    int getRequired() const { return required; }
    int getAvailable() const { return available; }
};