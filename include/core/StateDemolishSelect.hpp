#pragma once


#include "GameState.hpp"

class StateDemolishSelect : public GameState {
public:
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};
