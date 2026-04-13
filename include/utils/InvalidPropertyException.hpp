#pragma once

#include "utils/NimonspoliException.hpp"

class InvalidPropertyException : public NimonspoliException {
private:
    std::string propertyCode;
public:
    InvalidPropertyException(std::string code) 
        : NimonspoliException("Kode properti tidak ditemukan: " + code), 
          propertyCode(code) {}
    
    std::string getCode() const { return propertyCode; }
};
