#include "models/MoveCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
#include "models/GoTile.hpp"
#include "core/StatePromptBuy.hpp"
#include "core/StateTaxChoice.hpp"
#include "core/StateJailed.hpp"
#include "core/StateFestivalSelect.hpp"
#include "core/StateTurnEnded.hpp"
#include <iostream>
using namespace std;
MoveCard::MoveCard(int steps) : SkillCard("Maju " + to_string(steps) + " Petak", steps, 0) {}

void MoveCard::activate(Player& player, GameManager& gm) {
    cout << "DEBUG: Tile Count = " << gm.getBoard().getTileCount() << endl;
    int oldPos = player.getPosition();
    int boardSize = gm.getBoard().getTileCount();
    
    // 1. Hitung posisi baru (Handling Circular Board)
    // Gunakan modulo agar posisi tetap dalam rentang indeks board

    int newPos = (oldPos + value) % boardSize;
    
    // Jika value negatif (mundur), pastikan hasil modulo tetap positif
    if (newPos < 0) {
        newPos += boardSize;
    }

    cout << player.getUsername() << " menggunakan kartu gerak! Pindah " << value << " langkah.\n";

    // 2. Cek apakah melewati petak GO (index 0)
    // Logika: Jika posisi baru lebih kecil dari posisi lama (dan langkah positif), berarti lewat GO
    if (value > 0 && newPos < oldPos) {
        // Cari petak GO (biasanya indeks 0)
        Tile& goTile = gm.getBoard().getTile(0);
        GoTile* go = dynamic_cast<GoTile*>(&goTile);
        if (go != nullptr) {
            go->passGo(player, gm);
        }
    }

    // 3. Update posisi pemain
    player.setPosition(newPos);

    // 4. Trigger efek petak tujuan (onLanded)
    // Tile& targetTile = gm.getBoard().getTile(newPos);
    // cout << "Mendarat di: " << targetTile.getName() << " (" << targetTile.getCode() << ")\n";
    // targetTile.onLanded(player, gm);
    player.setStatus("TURN_ENDED"); 

    Tile& targetTile = gm.getBoard().getTile(newPos);
    
    try {
        targetTile.onLanded(player, gm);
    } catch (const InsufficientFundsException& e) {
        player.setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
    }

    // FAKTA KRUSIAL: Evaluasi nasib player setelah mendarat biar State-nya nyambung!
    string s = player.getStatus();
    if (s.find("PROMPT_BUY") == 0) gm.changeState(std::make_unique<StatePromptBuy>());
    else if (s.find("TAX_CHOICE") == 0) gm.changeState(std::make_unique<StateTaxChoice>());
    else if (s.find("JAILED") == 0) gm.changeState(std::make_unique<StateJailed>());
    else if (s == "FESTIVAL_SELECT") gm.changeState(std::make_unique<StateFestivalSelect>());
    else gm.changeState(std::make_unique<StateTurnEnded>()); // Kalau mendarat di tanah sendiri/aman
}