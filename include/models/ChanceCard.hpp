#pragma once

#include "models/Card.hpp"

class ChanceCard : public Card {
private:
    std::string effectType;
public:
    ChanceCard(std::string desc, std::string effect);
    void execute(Player& player, GameManager& gm) override;
};
