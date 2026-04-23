#include "core/StateDemolishSelect.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"
void StateDemolishSelect::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    int baseY = 370;
    DrawRectangle(910, baseY, 280, 100, Fade(RED, 0.8f));
    DrawText("HANCURKAN GEDUNG!", 920, baseY + 15, 18, WHITE);
    DrawText("Tekan [1-9] pilih properti", 920, baseY + 50, 14, RAYWHITE);
}
void StateDemolishSelect::handleInput(GameManager& gm, GameGUI& gui) {
    // Implementasi demolish sesuai logika lu...
    Player& p = gm.getCurrentPlayer();
    if (IsKeyPressed(KEY_ONE)) { // Dummy contoh
        p.setStatus("TURN_ENDED");
        gm.changeState(std::make_unique<StateTurnEnded>());
    }
}
