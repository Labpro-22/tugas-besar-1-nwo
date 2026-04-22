#include "include/models/LassoCard.hpp"
#include <iostream>


LassoCard::LassoCard()
    : SkillCard("LassoCard: Tarik lawan di depan ke posisi Anda", 0, 0){}

void LassoCard::activate(Player& player, GameManager& gm){
    int currentPos = player.getPosition();
    int boardSize = gm.getBoard().getTileCount();

    std::vector<Player*> targets;
    for(Player* other: gm.getAllPlayers()){
        if(other->getUsername() == player.getUsername()) continue;
        if(other->getStatus() == "BANKRUPT") continue;

        int otherPos = other->getPosition();
        int distance = (otherPos - currentPos + boardSize) % boardSize;
        if (distance > 0) {
            targets.push_back(other);
        }
    }
    
    if(targets.empty()) {
        std::cout << "[LASSO CARD] Tidak ada pemain lawan yang berada di depan Anda.\n";
        return;
    }

    std::cout << "[LASSO CARD] Pilih pemain yang ingin ditarik:\n";
    for(int i = 0; i < (int)targets.size(); i++){
        int dist = (targets[i]->getPosition() - currentPos + boardSize) % boardSize;
        std::cout << " " << (i + 1) << ". " << targets[i]->getUsername() << " (di " << gm.getBoard().getTile(targets[i]->getPosition()).getName() << ", " << dist << " petak di depan)\n";
    }

    std::cout << "Pilihan (1-" << targets.size() << "): ";

    int choice;
    std::cin >> choice;
    if(choice < 1 || choice > (int)targets.size()) {
        std::cout << "Pilihan tidak valid. LassoCard dibatalkan.\n";
        return;
    }

    Player* target = targets[choice - 1];
    std::cout << "[LASSO CARD] " << target->getUsername() << " ditarik dari " << gm.getBoard().getTile(target->getPosition()).getName() << " ke " << gm.getBoard().getTile(currentPos).getName() << "!\n";

    target->setPosition(currentPos);

    gm.getLogger().logAction(0, player.getUsername(), "KARTU", "LassoCard: Menarik " + target->getUsername() + " ke " + gm.getBoard().getTile(currentPos).getName());
    // Trigger efek petak bagi pemain yang ditarik
    gm.getBoard().getTile(currentPos).onLanded(*target, gm);
}