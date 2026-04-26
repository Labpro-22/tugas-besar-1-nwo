#include "models/UtilityTile.hpp"
#include "models/Player.hpp"
#include "utils/Dice.hpp"
#include "models/PropertyTile.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
using namespace std;
UtilityTile::UtilityTile(int idx, string c, string n, int price, int mortgage, map<int, int> multipliers)
    : PropertyTile(idx, c, n, "UTILITY", price, mortgage), multiplierTable(multipliers) {}
int UtilityTile::calculateRent(const Dice& dice, int ownerPropertyCount) const {
    if (propertyStatus == "MORTGAGED") return 0;
    int multiplier = 4; // Default
    if (multiplierTable.count(ownerPropertyCount)) {
        multiplier = multiplierTable.at(ownerPropertyCount);
    }
    
    return dice.getTotal() * multiplier;
}
int UtilityTile::getCurrentRent(const Dice& d, const GameManager& gm) const {
    if (ownerName == "BANK" || propertyStatus == "MORTGAGED") return 0;

    int count = 0;
    for (const Player* p : gm.getAllPlayers()) {
        if (p->getUsername() == ownerName) {
            for (const PropertyTile* prop : p->getOwnedProperties()) {
                if (prop->getType() == "UTILITY") count++;
            }
            break;
        }
    }
    return this->calculateRent(d, count);
}
void UtilityTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", "Mendarat di Utilitas " + name);

    if (ownerName == "BANK") {
        player.setStatus("PROMPT_BUY_" + this->code); 
    } 
    else if (ownerName == player.getUsername() || propertyStatus == "MORTGAGED") {
        player.setStatus("TURN_ENDED");
    } 
    else {
        Player* owner = nullptr;
        for (Player* p : gm.getAllPlayers()) {
            if (p->getUsername() == ownerName) { owner = p; break; }
        }
        
        if (owner != nullptr) {
            int ownerUtilCount = 0;
            for (PropertyTile* prop : owner->getOwnedProperties()) {
                if (prop->getType() == "UTILITY") ownerUtilCount++;
            }
            
            int rentAmount = calculateRent(gm.getDice(), ownerUtilCount);
            if (player.getBalance() < rentAmount) throw InsufficientFundsException(rentAmount, player.getBalance());
            
            player -= rentAmount;
            *owner += rentAmount;
            gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "RENT", "Bayar tagihan M" + to_string(rentAmount) + " ke " + ownerName);
            player.setStatus("TURN_ENDED");
        }
    }
}