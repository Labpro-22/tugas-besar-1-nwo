#pragma once


#include "GameState.hpp"

class StateTurnEnded : public GameState {
public:
    void handleInput(GameManager& gm, GameGUI& gui) override;
    
    // FAKTA: WAJIB DIDEKLARASIKAN DI SINI!
    void updateUI(GameManager& gm, GameGUI& gui) override; 
};
