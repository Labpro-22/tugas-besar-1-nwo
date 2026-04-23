#pragma once


#include "GameState.hpp"
#include "utils/IllegalMortgageException.hpp"

class StateLiquidation : public GameState {
private:
    int requiredAmount; // Buat nyimpen dia ngutang berapa
public:
    void onEnter(GameManager& gm) override;
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};