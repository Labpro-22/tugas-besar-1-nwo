#include "include/models/SkillCard.hpp"
#include <iostream>

SkillCard::SkillCard(std::string desc, int val, int dur)
    : Card(desc), value(val), duration(dur){}

void SkillCard::execute(Player& player, GameManager& gm){
    activate(player, gm);
}

int SkillCard::getValue() const {
    return value;
}

int SkillCard::getDuration() const {
    return duration;
}

void SkillCard::decrementDuration(){
    if (duration > 0) duration--;
}