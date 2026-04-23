#include "models/RailroadTile.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
using namespace std;

RailroadTile::RailroadTile(int idx, string c, string n, int price, int mortgage, map<int, int> rents)
    : PropertyTile(idx, c, n, "RAILROAD", price, mortgage), rentTable(rents) {}
void RailroadTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", "Mendarat di Stasiun " + name);

    if (ownerName == "BANK") {
        // FAKTA: Lempar ke UI buat nawarin Beli!
        player.setStatus("PROMPT_BUY_" + this->code); 
    } 
    else if (ownerName == player.getUsername() || propertyStatus == "MORTGAGED") {
        player.setStatus("TURN_ENDED");
    } 
    else {
        // Hitung aset stasiun milik owner
        Player* owner = nullptr;
        for (Player* p : gm.getAllPlayers()) {
            if (p->getUsername() == ownerName) { owner = p; break; }
        }
        
        if (owner != nullptr) {
            int ownerPropCount = 0;
            for (PropertyTile* prop : owner->getOwnedProperties()) {
                if (prop->getType() == "RAILROAD") ownerPropCount++;
            }
            
            int rentAmount = calculateRent(gm.getDice(), ownerPropCount);
            if (player.getBalance() < rentAmount) throw InsufficientFundsException(rentAmount, player.getBalance());
            
            player -= rentAmount;
            *owner += rentAmount;
            gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "RENT", "Bayar tiket M" + to_string(rentAmount) + " ke " + ownerName);
            player.setStatus("TURN_ENDED");
        }
    }
}
int RailroadTile::calculateRent(const Dice& dice, int ownerPropertyCount) const {
    try {
        return rentTable.at(ownerPropertyCount);
    } catch (InsufficientFundsException) {
        return 0;
    }
}

int RailroadTile::getCurrentRent(const Dice& d, const GameManager& gm) const {
    if (ownerName == "BANK" || propertyStatus == "MORTGAGED") return 0;
    int count = 0;
    // gm.getAllPlayers() otomatis manggil versi const sekarang!
    for (const Player* p : gm.getAllPlayers()) {
        if (p->getUsername() == ownerName) {
            for (const PropertyTile* prop : p->getOwnedProperties()) {
                if (prop->getType() == "RAILROAD") count++;
            }
            break;
        }
    }
    return this->calculateRent(d, count);
}