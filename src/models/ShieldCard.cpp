#include "models/ShieldCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include <iostream>

ShieldCard::ShieldCard() : SkillCard("ShieldCard: Kebal tagihan/sanksi selama 1 giliran", 0, 1){}

void ShieldCard::activate(Player& player, GameManager& gm){
    std::cout << "[SHIELDCARD] ShieldCard diaktifkan!\n";
    std::cout << "Anda kebal terhadap tagihan atau sanksi selama giliran ini.\n";
    gm.getLogger().logAction(0, player.getUsername(), "KARTU", "ShieldCard: Perlindungan aktif selama 1 giliran");

    // GameManager bertanggung jawab mengecek apakah pemain
    // memegang ShieldCard aktif (duration > 0) sebelum memproses
    // pembayaran sewa, pajak, atau efek kartu lain yang merugikan.
}