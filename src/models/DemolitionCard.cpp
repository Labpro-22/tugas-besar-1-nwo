 #include "models/DemolitionCard.hpp"
 #include "models/Player.hpp"
#include "core/GameManager.hpp"
 #include <iostream>

DemolitionCard::DemolitionCard()
    : SkillCard("DemolitionCard: Hancurkan 1 bangunan milik lawan", 0, 0) {}
 
void DemolitionCard::activate(Player& player, GameManager& gm) {
    std::vector<std::pair<Player*, StreetTile*>> buildingTargets;
 
    for (Player* other : gm.getAllPlayers()) {
        if (other->getUsername() == player.getUsername()) continue;
        if (other->getStatus() == "BANKRUPT") continue;
 
        for (PropertyTile* prop : other->getOwnedProperties()) {
            // Dynamic cast untuk cek apakah ini StreetTile
            StreetTile* street = dynamic_cast<StreetTile*>(prop);
            if (street && street->getBuildingCount() > 0) {
                buildingTargets.push_back({other, street});
            }
        }
    }
 
    if (buildingTargets.empty()) {
        std::cout << "[DEMOLITION CARD] Tidak ada properti lawan yang memiliki bangunan.\n";
        return;
    }
 
    // Tampilkan pilihan
    std::cout << "[DEMOLITION CARD] Pilih properti yang ingin dihancurkan bangunannya:\n";
    for (int i = 0; i < (int)buildingTargets.size(); i++) {
        auto& [owner, street] = buildingTargets[i];
        int count = street->getBuildingCount();
        std::string buildingDesc = (count == 5) ? "Hotel" : std::to_string(count) + " rumah";
        std::cout << "  " << (i + 1) << ". " << street->getName() << " (" << owner->getUsername() << ") - " << buildingDesc << "\n";
    }
    std::cout << "Pilihan (1-" << buildingTargets.size() << "): ";
 
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > (int)buildingTargets.size()) {
        std::cout << "Pilihan tidak valid. DemolitionCard dibatalkan.\n";
        return;
    }
 
    auto& [targetOwner, targetStreet] = buildingTargets[choice - 1];
    int beforeCount = targetStreet->getBuildingCount();
 
    targetStreet->demolishBuilding();
 
    int afterCount = targetStreet->getBuildingCount();
    std::string beforeDesc = (beforeCount == 5) ? "Hotel" : std::to_string(beforeCount) + " rumah";
    std::string afterDesc  = (afterCount  == 0) ? "kosong" : std::to_string(afterCount) + " rumah";
 
    std::cout << "[DEMOLITION CARD] Bangunan di " << targetStreet->getName() << " milik " << targetOwner->getUsername() << " dihancurkan! (" << beforeDesc << " → " << afterDesc << ")\n";
 
    gm.getLogger().logAction(0, player.getUsername(), "KARTU",
        "DemolitionCard: Hancurkan bangunan di " + targetStreet->getName() + " milik " + targetOwner->getUsername());
}