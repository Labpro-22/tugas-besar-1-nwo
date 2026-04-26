#pragma once

#include "models/Player.hpp"
#include <string>
class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name, int startBalance,Color c);
    // void promptTurnAction(GameManager& gm) override;
    void takeTurn(GameManager& gm) override;
    
    friend std::ostream& operator<<(std::ostream& os, const HumanPlayer& p) {
        os << "[HUMAN] " << static_cast<const Player&>(p);
        return os;
    }
};
