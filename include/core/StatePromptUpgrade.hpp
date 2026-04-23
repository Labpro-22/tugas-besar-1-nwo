#pragma once


#include "GameState.hpp"


class StatePromptUpgrade : public GameState {
public:
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};