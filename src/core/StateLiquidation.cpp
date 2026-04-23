#include "core/StateLiquidation.hpp"
#include "models/Player.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"
#include "models/PropertyTile.hpp"
void StateLiquidation::onEnter(GameManager& gm) {
    Player& p = gm.getCurrentPlayer();
    std::string status = p.getStatus();
    
    // Potong string "LIQUIDATING_" (12 karakter) buat ngambil angkanya
    if (status.length() > 12) {
        try {
            requiredAmount = std::stoi(status.substr(12));
        } catch (...) {
            requiredAmount = 0;
        }
    }
}

void StateLiquidation::updateUI(GameManager& gm, GameGUI& ) {
    Player& p = gm.getCurrentPlayer();
    
    // Gambar background semi-transparan biar dramatis
    DrawRectangle(0, 0, 1200, 1000, Fade(BLACK, 0.7f));
    
    // Kotak Panel Utama
    DrawRectangle(300, 200, 600, 500, RAYWHITE);
    DrawRectangleLines(300, 200, 600, 500, RED);
    
    DrawText("!!! PANEL LIKUIDASI !!!", 470, 220, 20, RED);
    
    std::string txtUtang = "Kewajiban: M" + std::to_string(requiredAmount);
    std::string txtSaldo = "Uang Anda: M" + std::to_string(p.getBalance());
    
    DrawText(txtUtang.c_str(), 320, 260, 18, BLACK);
    DrawText(txtSaldo.c_str(), 320, 290, 18, (p.getBalance() >= requiredAmount) ? DARKGREEN : MAROON);
    
    // List Aset yang bisa digadai / dijual
    DrawText("Aset Anda:", 320, 330, 18, DARKBLUE);
    
    int yPos = 360;
    int idx = 1;
    for (PropertyTile* prop : p.getOwnedProperties()) {
        if (prop->getPropertyStatus() != "MORTGAGED") {
            std::string item = "[" + std::to_string(idx) + "] Gadai " + prop->getName() + " (M" + std::to_string(prop->getMortgageValue()) + ")";
            DrawText(item.c_str(), 340, yPos, 16, BLACK);
            yPos += 25;
            idx++;
        }
    }
    
    // Pesan di bawah
    if (p.getBalance() >= requiredAmount) {
        DrawText("[ENTER] Selesai & Bayar Utang", 450, 640, 18, DARKGREEN);
    } else {
        if (idx == 1) { // Gak punya aset lagi
            DrawText("[B] Nyatakan BANGKRUT!", 480, 640, 18, RED);
        } else {
            DrawText("Tekan Nomor [1..9] untuk Menggadai", 420, 640, 16, GRAY);
        }
    }
}

void StateLiquidation::handleInput(GameManager& gm, GameGUI& ) {
    Player& p = gm.getCurrentPlayer();
    
    // Kumpulin aset yang masih bisa digadai (belum di-mortgage)
    std::vector<PropertyTile*> liquidatable;
    for (PropertyTile* prop : p.getOwnedProperties()) {
        if (prop->getPropertyStatus() != "MORTGAGED") {
            liquidatable.push_back(prop);
        }
    }

    // 1. Cek kalau duitnya udah cukup buat bayar utang
    if (p.getBalance() >= requiredAmount) {
        if (IsKeyPressed(KEY_ENTER)) {
            // Potong duitnya untuk bayar utang (asumsi bayar ke Bank/Sistem dulu)
            p -= requiredAmount;
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "PAY_DEBT", "Berhasil melunasi utang M" + std::to_string(requiredAmount));
            
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
        return; // Jangan kasih dia pencet tombol lain kalau udah cukup
    }

    // 2. Cek kalau dia bener-bener miskin dan gak punya aset lagi (BANGKRUT)
    if (liquidatable.empty() && p.getBalance() < requiredAmount) {
        if (IsKeyPressed(KEY_B)) {
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "BANKRUPT", "Dinyatakan BANGKRUT!");
            
            // Logika sederhana: balikin semua aset ke BANK (Nanti bisa lu kembangin buat ditransfer ke kreditor)
            for (PropertyTile* prop : p.getOwnedProperties()) {
                prop->setOwner("BANK");
            }
            p.setStatus("BANKRUPT"); 
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
        return;
    }

    // 3. Logika mencet tombol angka [1-9] buat gadai properti
    // 3. Logika mencet tombol angka [1-9] buat gadai properti
    for (long unsigned int i = 0; i < liquidatable.size(); i++) {
        if (i > 8) break; // Maksimal mapping ke tombol 1-9
        
        if (IsKeyPressed(KEY_ONE + i)) {
            PropertyTile* prop = liquidatable[i];
            
            // FAKTA: Pasang sabuk pengaman TRY-CATCH!
            try {
                prop->mortgageProperty(); // Coba gadai...
                
                // Kalau sukses, gak ada error yang dilempar, lanjut tambah duit
                p += prop->getMortgageValue(); 
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "MORTGAGE", "Gadai " + prop->getName() + " (+M" + std::to_string(prop->getMortgageValue()) + ")");
                
            } catch (const IllegalMortgageException& e) {
                // FAKTA: Kalau gagal karena ada rumah, tangkep pesannya dan masukin ke LOG!
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "ERROR", e.what());
                
            } catch (const std::exception& e) {
                // Jaga-jaga kalau ada error aneh lainnya
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "ERROR", "Gagal gadai: System Error");
            }
        }
    }
}