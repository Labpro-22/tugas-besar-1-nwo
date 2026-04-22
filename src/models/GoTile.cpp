#include "models/GoTile.hpp"

GoTile::GoTile(int idx, std::string c, std::string n, int salary) 
    : Tile(idx, c, n, "SPECIAL"), salaryAmount(salary) {}

void GoTile::onLanded(Player& player, GameManager& gm) {
    std::cout << "Mendarat tepat di petak GO! Menerima gaji sebesar M" << salaryAmount << "." << std::endl;
    
    player += salaryAmount;
    
    // Mencatat ke Transaction Logger melalui GameManager
    gm.getLogger().logAction(gm.getCurrentTurn(), player.getUsername(), "GAJI", "Berhenti di GO");
}

void GoTile::passGo(Player& player, GameManager& gm) {
    std::cout << "Melewati petak GO! Menerima gaji sebesar M" << salaryAmount << "." << std::endl;
    
    player += salaryAmount;
    
    // Mencatat ke Transaction Logger
    gm.getLogger().logAction(gm.getCurrentTurn(), player.getUsername(), "GAJI", "Melewati GO");
    
}