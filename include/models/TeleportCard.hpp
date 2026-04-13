#pragma once

#include "models/SkillCard.hpp"

class TeleportCard : public SkillCard {
public:
    TeleportCard();
    void activate(Player& player, GameManager& gm) override;
};
