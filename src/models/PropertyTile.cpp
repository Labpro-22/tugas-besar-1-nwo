#include "models/PropertyTile.hpp"
#include "models/Player.hpp"

PropertyTile::PropertyTile(int idx, std::string c, std::string n, std::string t, int price, int mortgage)
    : Tile(idx, c, n, t), buyPrice(price), mortgageValue(mortgage), 
      ownerName("BANK"), propertyStatus("BANK") {}

int PropertyTile::getBuyPrice() const { 
    return buyPrice; 
}
int PropertyTile::getMortgageValue() const { 
    return mortgageValue; 
}
std::string PropertyTile::getOwner() const { 
    return ownerName; 
}

void PropertyTile::setOwner(std::string owner) { 
    ownerName = owner; 
    propertyStatus = (owner == "BANK") ? "BANK" : "OWNED"; 
}

std::string PropertyTile::getPropertyStatus() const { 
    return propertyStatus; 
}

void PropertyTile::mortgageProperty() { 
    propertyStatus = "MORTGAGED"; 
}
void PropertyTile::redeemProperty() { 
    propertyStatus = "OWNED"; 
}

void PropertyTile::onLanded(Player& player, GameManager& gm) {
    if (ownerName == "BANK") {
        // Tawarkan beli ke player
    } else if (ownerName != player.getUsername() && propertyStatus == "OWNED") {
        // Bayar sewa: player -= calculateRent(...)
    }
}