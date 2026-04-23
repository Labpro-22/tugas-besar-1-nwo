#include "core/StateTurnEnded.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "core/StateWaitingRoll.hpp"
#include "core/GameManager.hpp"

void StateTurnEnded::handleInput(GameManager& gm, GameGUI& gui) {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        gui.closeSkillMenu(); 
        gui.closeDeed();
        gui.closeAssetList(); 

        Player& p = gm.getCurrentPlayer();
        
        if (gm.getDice().isDouble() && p.getStatus() != "JAILED") {
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "DOUBLE", "Dadu Kembar! Jalan Lagi.");
            p.setStatus("WAITING_ROLL"); 
            gm.changeState(std::make_unique<StateWaitingRoll>()); 
        } else {
            gm.runGameLoop(); 
        }
    }
}

void StateTurnEnded::updateUI(GameManager& , GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    int baseY = 370; 
    DrawText("GILIRAN SELESAI", 920, baseY, 18, DARKGRAY);
    DrawText("[ENTER] LANJUT", 920, baseY + 30, 20, BLACK);
}