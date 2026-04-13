#pragma once

#include "models/SkillCard.hpp"

class LassoCard : public SkillCard {
public:
    LassoCard();
    void activate(Player& player, GameManager& gm) override;
};