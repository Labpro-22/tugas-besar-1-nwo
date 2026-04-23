#include "models/JailTile.hpp"
#include <iostream>

JailTile::JailTile(int idx, std::string c, std::string n, int bail) 
    : Tile(idx, c, n, "JAIL"), bailAmount(bail) {}

void JailTile::onLanded(Player& player, GameManager& gm) {
    if(player.getStatus() == "ACTIVE") {
        cout << "Pemain " << player.getUsername() << " hanya mampir di penjara.\n";
    }
    if(player.getStatus() == "JAILED") {
        processInmate(player, gm);
    }
}

void JailTile::processInmate(Player& player, GameManager& gm){
    int currentTurns = player.getJailTurns();
    if(currentTurns > 0){
        cout << "\n--- STATUS: DALAM PENJARA (" << player.getUsername() << ") ---\n";
        cout << "Opsi keluar:\n1. Bayar Denda (M" << bailAmount << ")\n2. Coba Kocok Dadu (Harus Double)\n";
        
        int choice;
        cout << "Pilihan Anda (1-2): ";
        cin >> choice;

        if (choice == 1) {
            try {
                player -= bailAmount;
                player.setStatus("ACTIVE");
                player.setJailTurns(0);
                cout << ">> Anda membayar denda dan sekarang BEBAS!\n";
                return;
            } catch (const InsufficientFundsException& e) {
                cout << ">> Uang tidak cukup untuk bayar denda!\n";
            }
        }

        cout << ">> Mencoba keberuntungan dengan dadu...\n";
        gm.getDice().rollRandom();
        cout << "Hasil dadu: " << gm.getDice().getTotal() << (gm.getDice().isDouble() ? " (DOUBLE!)" : "") << endl;

        if (gm.getDice().isDouble()) {
            player.setStatus("ACTIVE");
            player.setJailTurns(0);
            cout << ">> DOUBLE! Anda bebas dan diperbolehkan keluar.\n";
        } else {
            player.setJailTurns(currentTurns - 1);
            cout << ">> Gagal. Anda tetap di penjara untuk giliran ini.\n";
        }
    }
    else { // Sudah giliran terakhir, paksa bayar denda
        std::cout << "Sudah 3 kali gagal. Anda wajib membayar denda M" << bailAmount << "!\n";
        try {
            player -= bailAmount;
            player.setStatus("ACTIVE");
            player.setJailTurns(0);
            return;
        } catch (const InsufficientFundsException& e) {
            gm.handleBankruptcy(player, nullptr);
            return;
        }
    }
    

}