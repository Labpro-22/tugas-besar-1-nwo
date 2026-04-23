#include "models/Card.hpp"
#include <iostream>

Card::Card(std::string desc) : description(desc){}

std::string Card::getDescription() const {
    return description;
}