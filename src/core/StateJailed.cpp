#include "core/StateJailed.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateWaitingRoll.hpp"
#include "core/StateTurnEnded.hpp"
#include "views/GameGUI.hpp"

void StateJailed::handleInput(GameManager& gm, GameGUI&) {
    Player& p = gm.getCurrentPlayer();
    if (IsKeyPressed(KEY_ONE) && p.getBalance() >= 50) { 
        p -= 50; 
        p.setStatus("ACTIVE"); 
        gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "FREE", "Bayar denda M50.");
        gm.changeState(std::make_unique<StateWaitingRoll>()); 
    } else if (IsKeyPressed(KEY_TWO)) {
        gm.getDice().rollRandom();
        if (gm.getDice().isDouble()) {
            p.setStatus("ACTIVE");
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "LUCKY", "Double! Bebas.");
            gm.changeState(std::make_unique<StateTurnEnded>());
        } else {
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
    }
}

void StateJailed::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    DrawRectangle(200, 300, 400, 200, Fade(BLACK, 0.9f));
    DrawRectangleLines(200, 300, 400, 200, RED);
    

    DrawText("ANDA DI PENJARA!", 270, 330, 24, RED);
    

    DrawText("[1] Bayar Uang Jaminan (M50)", 230, 380, 20, RAYWHITE);
    DrawText("[2] Kocok Dadu (Harus Double)", 230, 420, 20, RAYWHITE);
    
    std::string bal = "Saldo Anda: M" + std::to_string(gm.getCurrentPlayer().getBalance());
    DrawText(bal.c_str(), 230, 460, 18, GOLD);
}