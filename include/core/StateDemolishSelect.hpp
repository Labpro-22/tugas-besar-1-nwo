

#pragma once

#include "GameState.hpp"
#include <vector>
#include <string>

class StreetTile;

class StateDemolishSelect : public GameState {
private:
    enum class Phase { SELECT_TARGET, CONFIRM_DEMOLISH };
    Phase phase = Phase::SELECT_TARGET;
    
    std::vector<StreetTile*> targetProperties;
    StreetTile* selectedProperty = nullptr;
    std::string ownerName;
    
    std::string message;
    int messageTimer = 0;
    
    void scanTargets(GameManager& gm);
    void resetState();
    
public:
    void onEnter(GameManager& gm) override;
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};
