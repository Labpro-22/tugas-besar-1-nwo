#include "models/TeleportCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include <iostream>

TeleportCard::TeleportCard()
    : SkillCard("TeleportCard: Pindah ke petak manapun", 0, 0){}

void TeleportCard::activate(Player& player, GameManager& gm){
    std::cout << "[TELEPORT CARD] Masukkan kode petak tujuan: ";
    std::string targetCode;
    std::cin >> targetCode;

    int targetIndex = gm.getBoard().getTileIndexByCode(targetCode);
    if(targetIndex == -1){
        std::cout << "Kode petak tidak valid! TeleportCard gagal digunakan.\n";
        return;
    }

    int currentPos = player.getPosition();
    int boardSize = gm.getBoard().getTileCount();

    std::cout << "[TELEPORT CARD] berpindah ke: " << gm.getBoard().getTile(targetIndex).getName() << "\n";

    // Cek apakah melewati GO (posisi tujuan < posisi asal secara clockwise)
    // Hanya berlaku jika targetIndex < currentPos (melewati petak 0)
    if (targetIndex < currentPos) {
        int salary = 200;
        player += salary;
        std::cout << "Melewati GO! Menerima gaji M" << salary << ".\n";
        gm.getLogger().logAction(0, player.getUsername(), "GAJI",
            "Melewati GO via TeleportCard, menerima M" + std::to_string(salary));
    }

    player.setPosition(targetIndex);
    gm.getLogger().logAction(0, player.getUsername(), "KARTU", "TeleportCard: Pindah ke " + gm.getBoard().getTile(targetIndex).getName() + " (" + targetCode + ")");
    gm.getBoard().getTile(targetIndex).onLanded(player, gm);
}