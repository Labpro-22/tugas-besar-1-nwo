#include "include/models/DiscountCard.hpp"
#include <iostream>

DiscountCard::DiscountCard(int percent)
    : SkillCard("DiscountCard: Diskon " + std::to_string(percent) + "%", percent, 1){}

void DiscountCard::activate(Player& player, GameManager& gm){
    std::cout << "[DISCOUNT CARD] Diskon " << value << "% aktif selama giliran ini.\n";
    std::cout << "Diskon berlaku untuk pembelian properti pada giliran ini.\n";
    gm.getLogger().logAction(0, player.getUsername(), "KARTU", "DisountCard: Diskon " + std::to_string(value) + "% diaktifkan");

    // duration = 1 menandakan kartu masih aktif untuk giliran ini.
    // GameManager harus memanggil decrementDuration() di akhir giliran
    // dan menghapus kartu dari hand jika duration == 0.
}