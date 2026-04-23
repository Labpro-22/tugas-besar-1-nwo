#include "core/StateFestivalSelect.hpp"
#include "views/GameGUI.hpp"
#include "core/GameManager.hpp"
#include "models/Player.hpp"
#include "models/StreetTile.hpp"
#include "core/StateTurnEnded.hpp"

void StateFestivalSelect::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    int baseY = 370;
    DrawRectangle(910, baseY, 280, 220, Fade(PURPLE, 0.8f));
    DrawRectangleLines(910, baseY, 280, 220, WHITE);
    DrawText("PILIH LOKASI FESTIVAL:", 920, baseY + 10, 18, GOLD);
    
    const auto& props = gm.getCurrentPlayer().getOwnedProperties();
    for (int i = 0; i < (int)props.size() && i < 7; i++) { 
        Color textColor = (props[i]->getType() == "STREET") ? RAYWHITE : GRAY;
        DrawText(TextFormat("%d. %s", i + 1, props[i]->getName().c_str()), 925, baseY + 40 + (i * 22), 16, textColor);
    }
    DrawText("Tekan angka [1-7]", 920, baseY + 195, 15, YELLOW);
}


void StateFestivalSelect::handleInput(GameManager& gm, GameGUI& gui) {
    Player& p = gm.getCurrentPlayer();
    int favIdx = -1;
    for (int i = 0; i < 7; i++) {
        if (IsKeyPressed(KEY_ONE + i)) favIdx = i;
    }
    if (favIdx != -1 && favIdx < (int)p.getOwnedProperties().size()) {
        StreetTile* st = dynamic_cast<StreetTile*>(p.getOwnedProperties()[favIdx]);
        if (st) {
            st->applyFestival(2, 1);
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "FESTIVAL", "Pesta di " + st->getName());
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>()); // GANTI GIGI!
        }
    }
}