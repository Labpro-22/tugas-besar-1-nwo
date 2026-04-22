#include "models/CommunityChestCard.hpp"
#include <iostream>

CommunityChestCard::CommunityChestCard(std::string desc, std::string effect, int amt) : Card(desc), effectType(effect), amount(amt){}

void CommunityChestCard::execute(Player& player, GameManager& gm){
    std::cout << "\n[KARTU DANA UMUM] " << description << "\n";

    if(effectType == "BIRTHDAY"){
        int collected = 0;
        for (Player* other : gm.getAllPlayers()){
            if(other->getUsername() == player.getUsername()) continue;
            if(other->getStatus() == "BANKRUPT") continue;

            if(other->getBalance() < amount){
                int taken = other->getBalance();
                *other -= taken;
                player += taken;
                collected += taken;
                std::cout << other->getUsername() << " hanya bisa membayar M" << taken << ".\n";
        
            }
        }
        std::cout << player.getUsername() << " menerima total M" << collected << " sebagai hadiah ulang tahun!\n";
        gm.getlogger().logAction(0, player.getUsername(), "KARTU", "Dana Umum: Ulang tahun, menerima M" + std::to_string(collected));

    } else if (effectType == "DOCTOR"){
        std::cout << "Biaya dokter M" << amount << " harus dibayar ke Bank.\n";

        if(player.getBalance() < amount) {
            std::cout << "Kamu tidak mampu membayar biaya dokter! (M" << amount << ")\n";
            std::cout << "Uang kamu saat ini: M" << player.getBalance() << "\n";
            gm.getLogger().logAction(0, player.getUsername(), "KARTU", "Dana Umum: Biaya dokter M" + std::to_string(amount) + " - tidak mampu bayar");

            // Alur kebangkrutan ditangani GameManager
            gm.handleBankruptcy(player, nullptr);

        } else {
            player -= amount;
            std::cout << "Kamu membayar M" << amount << " ke Bank. " << "Sisa Uang = M" << player.getBalance() << ".\n";
            gm.getLogger().logAction(0, player.getUsername(), "KARTU", "Dana Umum: Bayar biaya dokter M" + std::to_string(amount)); 
        }

    } else if (effectType == "CAMPAIGN") {
        int totalPaid = 0;
        int activePlayers = 0;
        for(Player* other : gm.getAllPlayers()) {
            if (other->getUsername() != player.getUsername() && other->getStatus() != "BANKRUPT"){
                activePlayers++;
            }
        }
        int totalOwed = amount * activePlayers;

        if(player.getBalance() < totalOwed) {
            std::cout << "Kamu tidak mampu membayar kampanye! Total: M" << totalOwed << "\n";
            std::cout << "Uang kamu saat ini: M" << player.getBalance() << "\n";
            gm.getLogger().logAction(0, player.getUsername(), "KARTU", "Dana Umum: Nyaleg M" + std::to_string(totalOwed) + " - tidak mampu bayar");
            gm.handleBankruptcy(player, nullptr);

        } else {
            for (Player* other : gm.getAllPlayers()) {
                if(other->getUsername() == player.getUsername()) continue;
                if(other->getStatus() == "BANKRUPT") continue;
                player -= amount;
                *other += amount;
                totalPaid += amount;
                std::cout << "Membayar M" << amount << " ke " << other->getUsername() << ".\n";
            }
            
            std::cout << player.getUsername() << " membayar total M" << totalPaid <<  " untuk kampanye.\n";
            gm.getLogger().getAction(0, player.getUsername(), "KARTU", "Dana umum: Nyaleg, bayar M" + std::to_string(totalPaid) + " ke semua pemain");
        }
    }
}