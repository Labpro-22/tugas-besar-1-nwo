#pragma once

#include "models/SkillCard.hpp"

class ShieldCard : public SkillCard {
public:
    ShieldCard();
    void activate(Player& player, GameManager& gm) override;
};
