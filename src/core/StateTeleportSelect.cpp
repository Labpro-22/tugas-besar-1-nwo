#include "core/StateTeleportSelect.hpp"
#include "core/StatePromptBuy.hpp"
#include "core/StateJailed.hpp"
#include "views/GameGUI.hpp"
#include "core/StateTurnEnded.hpp"
#include "models/Player.hpp"
#include "core/StateFestivalSelect.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"

void StateTeleportSelect::updateUI(GameManager& , GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    int baseY = 370;
    DrawRectangle(910, baseY, 280, 120, Fade(BLUE, 0.8f));
    DrawRectangleLines(910, baseY, 280, 120, WHITE); 
    DrawText("MODE TELEPORT", 920, baseY + 15, 20, WHITE);
    DrawText("Ketik ID Petak (0-39):", 920, baseY + 45, 15, RAYWHITE);
    
    // 1. Gambar Kotak Input
    DrawRectangle(920, baseY + 70, 60, 30, LIGHTGRAY);
    DrawRectangleLines(920, baseY + 70, 60, 30, BLACK);
    
    // 2. Efek Kursor Berkedip
    std::string cursor = (int(GetTime() * 2) % 2 == 0) ? "_" : "";
    DrawText((inputBuffer + cursor).c_str(), 925, baseY + 75, 18, BLACK);
    
    DrawText("[ENTER] Eksekusi", 990, baseY + 75, 15, YELLOW);
}

void StateTeleportSelect::handleInput(GameManager& gm, GameGUI&) {
    Player& p = gm.getCurrentPlayer();
    
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= '0') && (key <= '9') && (inputBuffer.length() < 2)) {
            inputBuffer += (char)key;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && inputBuffer.length() > 0) {
        inputBuffer.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER) && inputBuffer.length() > 0) {
        int targetID = std::stoi(inputBuffer);
        
        if (targetID >= 0 && targetID < gm.getBoard().getTileCount()) {
            p.setPosition(targetID);
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "TELEPORT", "Pindah ke petak " + std::to_string(targetID));
            
            try { 
                gm.getBoard().getTile(targetID).onLanded(p, gm); 
            } catch (const InsufficientFundsException& e) { 
                p.setStatus("LIQUIDATING_" + std::to_string(e.getRequired() - e.getAvailable())); 
            }
            
           
            std::string s = p.getStatus();
            if (s.find("PROMPT_BUY") == 0) gm.changeState(std::make_unique<StatePromptBuy>());
            else if (s.find("TAX_CHOICE") == 0) gm.changeState(std::make_unique<StateTaxChoice>());
            else if (s.find("JAILED") == 0) gm.changeState(std::make_unique<StateJailed>());
            else if (s == "FESTIVAL_SELECT") gm.changeState(std::make_unique<StateFestivalSelect>());
            else {
                p.setStatus("TURN_ENDED");
                gm.changeState(std::make_unique<StateTurnEnded>());
            }
        } else {
           
            inputBuffer = ""; 
        }
    }
}