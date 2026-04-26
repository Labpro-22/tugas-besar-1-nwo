#pragma once

#include "core/GameState.hpp"
#include "models/PropertyTile.hpp"
#include <vector>
#include <string>

class StateRedeem : public GameState {
public:
    enum class Phase {
        SELECT_PROPERTY,
        CONFIRM_REDEEM
    };

    StateRedeem() = default;
    void onEnter(GameManager& gm) override;
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;

private:
    void resetState();
    void scanMortgagedProperties(GameManager& gm);
    
    Phase phase;
    std::vector<PropertyTile*> mortgagedProperties;
    PropertyTile* selectedProperty;
    std::string message;
    int messageTimer;
};
