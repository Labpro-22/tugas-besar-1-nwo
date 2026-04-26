#pragma once

#include "GameState.hpp"
#include <vector>
#include <map>
#include <string>

class StreetTile;

class StatePromptUpgrade : public GameState {
private:
    enum class Phase { SELECT_GROUP, SELECT_PROPERTY, CONFIRM_HOTEL };
    Phase phase = Phase::SELECT_GROUP;
    std::string selectedColorGroup;
    StreetTile* selectedProperty = nullptr;
    std::vector<std::string> groupNames;
    std::map<std::string, std::vector<StreetTile*>> monopolyGroups;
    std::string message;
    int messageTimer = 0;
    
    void buildMonopolyGroups(GameManager& gm);
    bool canBuildOn(StreetTile* target);
    void resetState();
    
public:
    void onEnter(GameManager& gm) override;
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};

