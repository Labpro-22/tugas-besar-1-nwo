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

void StateJailed::updateUI(GameManager&, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    DrawRectangle(400, 850, 400, 40, Fade(BLACK, 0.8f));
    DrawRectangleLines(400, 850, 400, 40, ORANGE);
    DrawText("[1] Bayar M50 | [2] Kocok Double", 420, 862, 20, ORANGE);
}