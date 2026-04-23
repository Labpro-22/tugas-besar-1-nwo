#include "core/StatePromptUpgrade.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "models/Tile.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"

void StatePromptUpgrade::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    Player& p = gm.getCurrentPlayer();
    Tile& t = gm.getBoard().getTile(p.getPosition());
    
    // FAKTA: baseY diturunkan sedikit biar gak tabrakan sama Dadu
    int baseY = 410; 
    int boxH = 140;

    DrawRectangle(910, baseY, 280, boxH, Fade(GREEN, 0.1f)); // Background soft
    DrawRectangleLines(910, baseY, 280, boxH, DARKGREEN);   // Border tegas
    
    DrawText("UPGRADE PROPERTI", 925, baseY + 15, 18, DARKGREEN);
    
    // Gunakan polymorphism t.getName()
    DrawText(t.getName().c_str(), 925, baseY + 45, 16, BLACK);
    
    // Gunakan polymorphism t.getUpgradePrice()
    std::string priceText = "Biaya: M" + std::to_string(t.getUpgradePrice());
    DrawText(priceText.c_str(), 925, baseY + 70, 16, MAROON);
    
    DrawText("[U] UPGRADE", 925, baseY + 105, 16, DARKBLUE);
    DrawText("[L] LEWAT", 1040, baseY + 105, 16, GRAY);
}
void StatePromptUpgrade::handleInput(GameManager& gm, GameGUI&) {
    Player& p = gm.getCurrentPlayer();
    Tile& t = gm.getBoard().getTile(p.getPosition());

    if (IsKeyPressed(KEY_U)) {
        if (p.getBalance() >= t.getUpgradePrice()) {
            p -= t.getUpgradePrice();
            t.build(); // FAKTA: Polymorphism murni tanpa casting!
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "BUILD", "Membangun properti.");
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
    } else if (IsKeyPressed(KEY_L)) {
        p.setStatus("TURN_ENDED");
        gm.changeState(std::make_unique<StateTurnEnded>());
    }
}
