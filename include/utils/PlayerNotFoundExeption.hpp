#pragma once


#include "utils/NimonspoliException.hpp"

class PlayerNotFoundException : public NimonspoliException {
private:
    std::string username;
public:
    PlayerNotFoundException(std::string name) 
        : NimonspoliException("Pemain tidak ditemukan dalam sistem: " + name), username(name) {}
    std::string getUsername() const { return username; }
};