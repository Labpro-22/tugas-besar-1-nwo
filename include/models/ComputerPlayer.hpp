#pragma once

#include "models/Player.hpp"

class ComputerPlayer : public Player {
public:
    ComputerPlayer(std::string name, int startBalance);
    void promptTurnAction(GameManager& gm) override;
};