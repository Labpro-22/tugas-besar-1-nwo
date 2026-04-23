#include "core/StateLassoSelect.hpp"
#include "core/StateTurnEnded.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "models/Tile.hpp"
void StateLassoSelect::updateUI(GameManager& , GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    int baseY = 370;
    DrawRectangle(910, baseY, 280, 100, Fade(ORANGE, 0.8f));
    DrawText("MODE LASSO (TARIK)", 920, baseY + 15, 20, WHITE);
    DrawText("Tekan [1-4] pilih pemain", 920, baseY + 50, 15, RAYWHITE);
}
void StateLassoSelect::handleInput(GameManager& gm, GameGUI&) {
    Player& p = gm.getCurrentPlayer();
    int victimIdx = -1;
    for (int i=0; i<4; i++) if (IsKeyPressed(KEY_ONE + i)) victimIdx = i;
    auto allPlayers = gm.getAllPlayers();
    if (victimIdx != -1 && victimIdx < (int)allPlayers.size() && allPlayers[victimIdx] != &p) {
        allPlayers[victimIdx]->setPosition(p.getPosition());
        gm.getBoard().getTile(p.getPosition()).onLanded(*allPlayers[victimIdx], gm);
        gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "LASSO", "Menarik " + allPlayers[victimIdx]->getUsername());
        p.setStatus("TURN_ENDED");
        gm.changeState(std::make_unique<StateTurnEnded>());
    }
}