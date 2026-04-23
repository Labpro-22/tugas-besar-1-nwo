#include "core/StateTaxChoice.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "models/TaxTile.hpp"
#include "core/GameManager.hpp"
#include "core/StateTurnEnded.hpp"
void StateTaxChoice::handleInput(GameManager& gm, GameGUI& gui) {
    Player& p = gm.getCurrentPlayer();
    TaxTile* tt = dynamic_cast<TaxTile*>(&gm.getBoard().getTile(p.getPosition()));
    
    if (tt) {
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO)) {
            int totalTax = IsKeyPressed(KEY_ONE) ? tt->getFlatTax() : (p.getTotalWealth() * 0.1);
            try {
                p -= totalTax;
                gm.getBank().collect(totalTax, gm);
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "TAX", "Bayar pajak M" + std::to_string(totalTax));
                p.setStatus("TURN_ENDED");
                gm.changeState(std::make_unique<StateTurnEnded>());
            } catch (const InsufficientFundsException& e) {
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "WARNING", "Dana tidak cukup untuk pajak!");
                p.setStatus("LIQUIDATING_" + std::to_string(e.getRequired() - e.getAvailable()));
            }
        }
    }
}
void StateTaxChoice::updateUI(GameManager&, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    DrawRectangle(350, 850, 500, 40, Fade(BLACK, 0.8f));
    DrawRectangleLines(350, 850, 500, 40, RED);
    DrawText("[1] Bayar Flat | [2] Bayar 10% Aset", 380, 862, 20, RED);
}