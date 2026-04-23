#include "models/CommunityChestCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include <iostream>
using namespace std;
CommunityChestCard::CommunityChestCard(std::string desc, std::string effect, int amt) : Card(desc), effectType(effect), amount(amt){}
void CommunityChestCard::execute(Player& player, GameManager& gm) {
    cout << "[COMMUNITY CHEST] " << description << endl;
    if (effectType == "COLLECT") {
        player += amount;
        gm.getBank().dispense(amount,gm);
    } else if (effectType == "PAY") {
        player -= amount; 
        gm.getBank().collect(amount,gm);
    }
}
