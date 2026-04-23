#include "models/DiscountCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateTurnEnded.hpp"
#include <iostream>

using namespace std;

DiscountCard::DiscountCard(int percent) : SkillCard("Diskon " + to_string(percent) + "%", percent, 1) {}
// void DiscountCard::activate(Player& player, GameManager& gm) {
//     player.setStatus("DISCOUNT");
//     cout << ">> BING! Kartu Diskon diaktifkan. Kamu mendapat potongan sewa 50% untuk 1x transaksi ke depan!\n";
// }
void DiscountCard::activate(Player& player, GameManager& gm) {
    // player.setBuff("DISCOUNT", 1); // Asumsi lu punya sistem Buff
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "BUFF", "Potongan sewa aktif!");
    player.setStatus("TURN_ENDED");
    gm.changeState(std::make_unique<StateTurnEnded>());
}