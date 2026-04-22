#include "models/Card.hpp"
#include <iostream>

Card::Card(std::string desc) : description(desc){}
Card::~Card(){}

std::string Card::getDescription() const {
    return description;
}