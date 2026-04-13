#pragma once

#include "models/Card.hpp"

class CommunityChestCard : public Card {
private:
    std::string effectType;
    int amount;
public:
    CommunityChestCard(std::string desc, std::string effect, int amt);
    void execute(Player& player, GameManager& gm) override;
};