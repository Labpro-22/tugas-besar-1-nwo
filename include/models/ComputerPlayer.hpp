#pragma once

#include "models/Player.hpp"


class ComputerPlayer : public Player {
public:
    ComputerPlayer(std::string name, int startBalance,Color c);
    // void promptTurnAction(GameManager& gm) override;
    void takeTurn(GameManager& gm)override;
    bool isBot() const override { return true; }
};