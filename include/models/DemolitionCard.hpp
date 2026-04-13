#pragma once

#include "models/SkillCard.hpp"

class DemolitionCard : public SkillCard {
public:
    DemolitionCard();
    void activate(Player& player, GameManager& gm) override;
};