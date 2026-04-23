#pragma once

#include "models/Player.hpp"
#include <string>
class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name, int startBalance);
    void promptTurnAction(GameManager& gm) override;
};

