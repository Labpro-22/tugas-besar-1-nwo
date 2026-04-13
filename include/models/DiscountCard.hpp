#pragma once

#include "models/SkillCard.hpp"

class DiscountCard : public SkillCard {
public:
    DiscountCard(int percent);
    void activate(Player& player, GameManager& gm) override;
};