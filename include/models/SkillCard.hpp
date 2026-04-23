#pragma once

#include "models/Card.hpp"
// #include "models/Player.hpp"
// #include "core/GameManager.hpp"

class Player;
class GameManager;

class SkillCard : public Card {
protected:
    int value;
    int duration;
public:
    SkillCard(std::string desc, int val, int dur);
    virtual void activate(Player& player, GameManager& gm) = 0;
    void execute(Player& player, GameManager& gm) override;
    int getValue() const;
    int getDuration() const;
    void decrementDuration();
};
