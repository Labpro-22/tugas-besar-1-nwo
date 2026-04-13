#pragma once

#include "models/SkillCard.hpp"

class MoveCard : public SkillCard {
public:
    MoveCard(int steps);
    void activate(Player& player, GameManager& gm) override;
};
