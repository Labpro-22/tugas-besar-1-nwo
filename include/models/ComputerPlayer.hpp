#pragma once

#include "models/Player.hpp"


class ComputerPlayer : public Player {
public:
    ComputerPlayer(std::string name, int startBalance,Color c);
    // void promptTurnAction(GameManager& gm) override;
    void takeTurn(GameManager& gm)override;
    bool isBot() const override { return true; }
    
    friend std::ostream& operator<<(std::ostream& os, const ComputerPlayer& p) {
        os << "[COM] " << static_cast<const Player&>(p);
        return os;
    }
};
