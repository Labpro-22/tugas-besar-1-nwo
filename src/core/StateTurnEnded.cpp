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
            
        
            gm.executeTurn(p); 
            
        } else {
            gm.runGameLoop(); 
        }
    }
}


void StateTurnEnded::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    DrawText("GILIRAN SELESAI", 910, 420, 20, GRAY);
    DrawText("[ENTER] LANJUT", 910, 450, 24, BLACK);

    const auto& logs = gm.getLogger().getLogs(); 
    
    if (!logs.empty()) {
        const std::string& lastLog = logs.back(); 
        
        int midX = 250;
        int midY = 300;
        
        // Cek Bayar Sewa
        if (lastLog.find("RENT") != std::string::npos) {
            DrawRectangle(midX, midY, 400, 150, Fade(BLACK, 0.85f));
            DrawRectangleLines(midX, midY, 400, 150, RED);
            DrawText("BAYAR SEWA!", midX + 110, midY + 30, 26, RED);
            DrawText(lastLog.c_str(), midX + 20, midY + 80, 14, RAYWHITE);
        }
        // Cek Masuk Penjara
        else if (lastLog.find("JAILED") != std::string::npos) {
            DrawRectangle(midX, midY, 400, 150, Fade(BLACK, 0.85f));
            DrawRectangleLines(midX, midY, 400, 150, ORANGE);
            DrawText("MASUK PENJARA!", midX + 90, midY + 30, 26, ORANGE);
            DrawText(lastLog.c_str(), midX + 20, midY + 80, 14, RAYWHITE);
        }
        else if (lastLog.find("BUY") != std::string::npos) {
            DrawRectangle(midX, midY, 400, 150, Fade(BLACK, 0.85f));
            DrawRectangleLines(midX, midY, 400, 150, GREEN);
            DrawText("PROPERTI DIBELI!", midX + 80, midY + 30, 26, LIME);
            DrawText(lastLog.c_str(), midX + 20, midY + 80, 14, RAYWHITE);
        }
        else if (lastLog.find("UPGRADE") != std::string::npos) {
            DrawRectangle(midX, midY, 400, 150, Fade(BLACK, 0.85f));
            DrawRectangleLines(midX, midY, 400, 150, SKYBLUE);
            DrawText("BANGUN RUMAH!", midX + 90, midY + 30, 26, SKYBLUE);
            DrawText(lastLog.c_str(), midX + 20, midY + 80, 14, RAYWHITE);
        }
    }
}
