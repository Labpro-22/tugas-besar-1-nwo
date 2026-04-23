#include "models/MoveCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
#include "models/GoTile.hpp"
#include "core/StatePromptBuy.hpp"
#include "core/StateTaxChoice.hpp"
#include "core/StateJailed.hpp"
#include "core/StateFestivalSelect.hpp"
#include "core/StateTurnEnded.hpp"
#include <iostream>
using namespace std;
MoveCard::MoveCard(int steps) : SkillCard("Maju " + to_string(steps) + " Petak", steps, 0) {}

void MoveCard::activate(Player& player, GameManager& gm) {
    cout << "DEBUG: Tile Count = " << gm.getBoard().getTileCount() << endl;
    int oldPos = player.getPosition();
    int boardSize = gm.getBoard().getTileCount();
    
  
    int newPos = (oldPos + value) % boardSize;
  
    if (newPos < 0) {
        newPos += boardSize;
    }

    cout << player.getUsername() << " menggunakan kartu gerak! Pindah " << value << " langkah.\n";

    if (value > 0 && newPos < oldPos) {
        Tile& goTile = gm.getBoard().getTile(0);
        GoTile* go = dynamic_cast<GoTile*>(&goTile);
        if (go != nullptr) {
            go->passGo(player, gm);
        }
    }

    
    player.setPosition(newPos);
    player.setStatus("TURN_ENDED"); 

    Tile& targetTile = gm.getBoard().getTile(newPos);
    
    try {
        targetTile.onLanded(player, gm);
    } catch (const InsufficientFundsException& e) {
        player.setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
    }

    
    string s = player.getStatus();
    if (s.find("PROMPT_BUY") == 0) gm.changeState(std::make_unique<StatePromptBuy>());
    else if (s.find("TAX_CHOICE") == 0) gm.changeState(std::make_unique<StateTaxChoice>());
    else if (s.find("JAILED") == 0) gm.changeState(std::make_unique<StateJailed>());
    else if (s == "FESTIVAL_SELECT") gm.changeState(std::make_unique<StateFestivalSelect>());
    else gm.changeState(std::make_unique<StateTurnEnded>()); 
}