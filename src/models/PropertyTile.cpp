#include "models/PropertyTile.hpp"
#include "utils/IllegalMortgageException.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
using namespace std;
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
    // 1. Cek apakah properti udah digadai duluan
    if (propertyStatus == "MORTGAGED") {
        throw IllegalMortgageException(this->name, "Properti sudah dalam status digadaikan!");
    }

    // 2. FAKTA: Cek apakah masih ada rumah/hotel di tanah ini!
    if (this->getBuildingCount() > 0) {
        throw IllegalMortgageException(this->name, "Masih ada bangunan di atasnya. Jual bangunan dulu!");
    }

    // Kalau lolos semua syarat, baru sah digadai
    propertyStatus = "MORTGAGED";
}
void PropertyTile::redeemProperty() { 
    propertyStatus = "OWNED"; 
}

void PropertyTile::onLanded(Player& player, GameManager& gm) {
    if (ownerName == "BANK") {
        player.setStatus("PROMPT_BUY_" + this->code); // Sinyal: Tawaran Beli
    } else if (ownerName == player.getUsername() || propertyStatus == "MORTGAGED") {
        player.setStatus("TURN_ENDED");
    } else {
        // Bayar Sewa Otomatis
        Player* owner = nullptr;
        for (Player* p : gm.getAllPlayers()) {
            if (p->getUsername() == ownerName) { owner = p; break; }
        }
        if (owner) {
            int rent = calculateRent(gm.getDice(), 0); 
            if (player.getBalance() < rent) throw InsufficientFundsException(rent, player.getBalance());
            player -= rent; *owner += rent;
            gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "RENT", "Bayar M" + to_string(rent));
            player.setStatus("TURN_ENDED");
        }
    }
}