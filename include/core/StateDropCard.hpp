#pragma once

#include "GameState.hpp"
// #include "GameManager.hpp"

class StateDropCard : public GameState {
public:
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};