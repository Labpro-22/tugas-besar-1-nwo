#include "models/Tile.hpp"

Tile::Tile(int idx, std::string c, std::string n, std::string t) 
    : index(idx), code(c), name(n), type(t) {}

int Tile::getIndex() const { 
    return index; 
}

std::string Tile::getCode() const { 
    return code; 
}

std::string Tile::getName() const { 
    return name; 
}

std::string Tile::getType() const { 
    return type; 
}