#pragma once
#include "utils/NimonspoliException.hpp"

class IllegalBuildException : public NimonspoliException {
private:
    std::string propertyName;
public:
    IllegalBuildException(std::string propName, std::string reason) 
        : NimonspoliException("Gagal membangun di " + propName + ": " + reason), propertyName(propName) {}
    std::string getPropertyName() const { return propertyName; }
};