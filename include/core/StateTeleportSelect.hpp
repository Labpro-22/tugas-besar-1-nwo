
#pragma once

#include <bits/stdc++.h>
#include "GameState.hpp"
#include "StateTaxChoice.hpp"
#include "models/Tile.hpp"


class StateTeleportSelect : public GameState {
private:
    std::string inputBuffer = ""; // FAKTA: Tambahan memori buat nyimpen ketikan
public:
    void handleInput(GameManager& gm, GameGUI& gui) override;
    void updateUI(GameManager& gm, GameGUI& gui) override;
};