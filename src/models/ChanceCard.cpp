#include "models/ChanceCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

ChanceCard::ChanceCard(std::string desc, std::string effect) : Card(desc), effectType(effect){}


void ChanceCard::execute(Player& player, GameManager& gm){
    std::cout << "\n[KARTU KESEMPATAN] " << description << "\n";

    if (effectType == "NEAREST_STATION"){
        const std::vector<int> stationIndices = {6, 16, 26, 36};
        int currentPos = player.getPosition();
        int targetIndex = -1;
        int boardSize = gm.getBoard().getTileCount();

        int minDistance = boardSize + 1;
        for (int stIdx : stationIndices){
            int distance = (stIdx - currentPos + boardSize) % boardSize;
            if (distance > 0 && distance < minDistance){
                minDistance = distance;
                targetIndex = stIdx;
            }
        }

        if (targetIndex == -1) targetIndex = stationIndices[0]; // fallback
        std::cout << "Bidak dipindahkan ke stasiun terdekat : " << gm.getBoard().getTile(targetIndex).getName() << "\n";

        // Cek apakah melewati GO
        if (targetIndex < currentPos){
            int salary = gm.getBoard().getTile(0).getName() == "GO" ? 200 : 200;
            player += salary;
            std::cout << "Melewati GO! Menerima gaji M" << salary << ".\n";
            gm.getLogger().logAction(0, player.getUsername(), "GAJI", "MELEWATI GO, menerima M" + std::to_string(salary));
        }
        
        player.setPosition(targetIndex);
        gm.getLogger().logAction(0, player.getUsername(), "KARTU", "KESEMPATAN: Pindah ke stasiun terdekat (" + gm.getBoard().getTile(targetIndex).getName() + ")");

        //Trigger efek petak yang dituju
        gm.getBoard().getTile(targetIndex).onLanded(player, gm);

    } else if (effectType == "MOVE_BACK_3"){
        int boardSize = gm.getBoard().getTileCount();
        int newPos = (player.getPosition() - 3 + boardSize) % boardSize;
        std::cout << "Bidak mundur 3 petak ke: " << gm.getBoard().getTile(newPos).getName() << "\n";

        player.setPosition(newPos);
        gm.getLogger().logAction(0, player.getUsername(), "KARTU", "Kesempatan: Mundur 3 petak ke " + gm.getBoard().getTile(newPos).getName());
        gm.getBoard().getTile(newPos).onLanded(player, gm);

    } else if (effectType == "GO_TO_JAIL") {
        std::cout << "Anda harus masuk penjara\n";

        //Cari index petak penjara
        int jailIndex = gm.getBoard().getTileIndexByCode("PEN");
        if (jailIndex == -1) jailIndex == 11;

        player.setPosition(jailIndex);
        player.setStatus("JAILED");
        player.setJailTurns(0);

        gm.getLogger().logAction(0, player.getUsername(), "KARTU", "Kesempatan: Masuk penjara");
    }
}