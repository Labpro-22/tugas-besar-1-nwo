#include "core/StateDropCard.hpp"
#include "core/GameManager.hpp"
#include "views/GameGUI.hpp"
#include "core/StateTurnEnded.hpp"
#include "models/Player.hpp"
#include "models/SkillCard.hpp"

void StateDropCard::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    int baseY = 370;
    DrawRectangle(910, baseY, 280, 220, Fade(ORANGE, 0.8f));
    DrawRectangleLines(910, baseY, 280, 220, BLACK);
    DrawText("TANGAN PENUH! BUANG 1:", 920, baseY + 10, 16, BLACK);

    const auto& hand = gm.getCurrentPlayer().getHand();
    for (int i = 0; i < (int)hand.size(); i++) {
        std::string desc = std::to_string(i + 1) + ". " + hand[i]->getDescription();
        if (desc.length() > 30) desc = desc.substr(0, 27) + "...";
        DrawText(desc.c_str(), 920, baseY + 40 + (i * 25), 14, DARKGRAY);
    }
    DrawText("Tekan angka [1-4]", 920, baseY + 190, 15, MAROON);
}

void StateDropCard::handleInput(GameManager& gm, GameGUI& ) {
    Player& p = gm.getCurrentPlayer();
    int dropIdx = -1;
    for (int i = 0; i < 4; i++) {
        if (IsKeyPressed(KEY_ONE + i)) dropIdx = i;
    }
    if (dropIdx != -1 && dropIdx < (int)p.getHand().size()) {
        p.dropSkillCard(dropIdx);
        gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "DISCARD", "Membuang kartu.");
        p.setStatus("TURN_ENDED");
        gm.changeState(std::make_unique<StateTurnEnded>());
    }
}
