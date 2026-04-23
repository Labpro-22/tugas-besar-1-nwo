#include "models/ShieldCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateTurnEnded.hpp"
#include <iostream>

using namespace std;
ShieldCard::ShieldCard() : SkillCard("Kebal Tagihan", 0, 1) {}
void ShieldCard::activate(Player& player, GameManager& gm) {
    player.setStatus("SHIELDED");
    cout << ">> TING! Shield Aktif! Kamu kebal dari 1x tagihan sewa berikutnya.\n";
    gm.changeState(std::make_unique<StateTurnEnded>());

}