#pragma once

#include "utils/NimonspoliException.hpp"
class IllegalMortgageException : public NimonspoliException {
private:
    std::string propertyName;
public:
    IllegalMortgageException(std::string propName, std::string reason) 
        : NimonspoliException("Gagal menggadai " + propName + ": " + reason), propertyName(propName) {}
    std::string getPropertyName() const { return propertyName; }
};