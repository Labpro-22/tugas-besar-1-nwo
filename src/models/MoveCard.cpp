#include "models/MoveCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include <iostream>

MoveCard::MoveCard(int steps)
    :SkillCard("MoveCard: maju " + std::to_string(steps) + " petak", steps, 0){}

void MoveCard::activate(Player& player, GameManager& gm){
    int boardSize = gm.getBoard().getTileCount();
    int currentPos = player.getPosition();
    int newPos = (currentPos + value) % boardSize;

    std::cout << "[MOVE CARD] maju " << value << " petak.\n";

    if (newPos < currentPos) {
        int salary = 200;
        player += salary;
        std::cout << "Melewati GO! Menerima gaji M" << salary << ".\n";
        gm.getLogger().logAction(0, player.getUsername(), "GAJI", "Melewati GO via MoveCard, menerima M" + std::to_string(salary)); 
    }

    player.setPosition(newPos);
    std::cout << "Bidak berpindah ke: " << gm.getBoard().getTile(newPos).getName() << "\n";
    gm.getLogger().logAction(0, player.getUsername(), "KARTU", "MoveCard: Maju " + std::to_string(value) + " petak ke " + gm.getBoard().getTile(newPos).getName());
    gm.getBoard().getTile(newPos).onLanded(player, gm);
}