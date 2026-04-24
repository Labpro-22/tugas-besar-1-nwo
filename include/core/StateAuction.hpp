#pragma once
#include "core/GameState.hpp"
#include <vector>
#include <string>

class PropertyTile;
class Player;

class StateAuction : public GameState {
private:
    PropertyTile* property;          
    std::vector<Player*> participants; 
    int currentBid;                  
    Player* highestBidder;            
    int currentPlayerIndex;            

    void nextPlayer();
    void endAuction(GameManager& gm);

public:
    StateAuction(PropertyTile* prop, std::vector<Player*> allPlayers);
    
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};