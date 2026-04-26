#include "core/StateDemolishSelect.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "models/StreetTile.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"

void StateDemolishSelect::resetState() {
    phase = Phase::SELECT_TARGET;
    targetProperties.clear();
    selectedProperty = nullptr;
    ownerName = "";
    message = "";
    messageTimer = 0;
}

void StateDemolishSelect::onEnter(GameManager& gm) {
    resetState();
    scanTargets(gm);
}

void StateDemolishSelect::scanTargets(GameManager& gm) {
    Player& p = gm.getCurrentPlayer();
    
    for (Tile* t : gm.getBoard().getAllTiles()) {
        if (t->isStreet()) {
            StreetTile* st = dynamic_cast<StreetTile*>(t);
            if (st && st->getOwner() != p.getUsername() && st->getOwner() != "BANK" && st->getOwner() != "" && st->getBuildingCount() > 0) {
                targetProperties.push_back(st);
            }
        }
    }
}

void StateDemolishSelect::handleInput(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    Player& p = gm.getCurrentPlayer();
    
    if (phase == Phase::SELECT_TARGET) {
        if (IsKeyPressed(KEY_ZERO)) {
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
            return;
        }
        
        for (int i = 0; i < 9; i++) {
            if (IsKeyPressed(KEY_ONE + i) && i < (int)targetProperties.size()) {
                selectedProperty = targetProperties[i];
                phase = Phase::CONFIRM_DEMOLISH;
                return;
            }
        }
    }
    else if (phase == Phase::CONFIRM_DEMOLISH) {
        if (IsKeyPressed(KEY_Y)) {
            if (selectedProperty) {
                std::string propName = selectedProperty->getName();
                int oldCount = selectedProperty->getBuildingCount();
                selectedProperty->demolishBuilding();
                
                message = propName + " dihancurkan 1 level! (" + std::to_string(oldCount) + " -> " + std::to_string(selectedProperty->getBuildingCount()) + ")";
                messageTimer = 120;
                
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "DEMOLISH", 
                    "Menghancurkan 1 level di " + propName);
            }
            // End turn immediately after 1 demolition (card effect used up)
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
        else if (IsKeyPressed(KEY_N)) {
            selectedProperty = nullptr;
            phase = Phase::SELECT_TARGET;
        }
    }
}

void StateDemolishSelect::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    int panelX = 150;
    int panelY = 150;
    int panelW = 900;
    int panelH = 700;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.92f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, RED);
    
    if (phase == Phase::SELECT_TARGET) {
        DrawText("=== HANCURKAN GEDUNG LAWAN ===", panelX + 30, panelY + 20, 24, RED);
        
        if (targetProperties.empty()) {
            DrawText("Tidak ada properti lawan yang bisa dihancurkan.", 
                     panelX + 30, panelY + 80, 20, GRAY);
            DrawText("Tekan [0] untuk batal", panelX + 30, panelY + 130, 18, WHITE);
            return;
        }
        
        int y = panelY + 70;
        for (int i = 0; i < (int)targetProperties.size() && i < 9; i++) {
            StreetTile* st = targetProperties[i];
            std::string bldgText;
            int count = st->getBuildingCount();
            if (count == 5) bldgText = "Hotel";
            else bldgText = std::to_string(count) + " rumah";
            
            std::string line = "[" + std::to_string(i + 1) + "] " + st->getName() + " (" + st->getCode() + ")";
            line += " - " + st->getOwner() + " - " + bldgText;
            DrawText(line.c_str(), panelX + 30, y, 18, RAYWHITE);
            y += 28;
        }
        
        DrawText("Pilih nomor properti (0 untuk batal)", 
                 panelX + 30, panelY + panelH - 50, 18, WHITE);
    }
    else if (phase == Phase::CONFIRM_DEMOLISH) {
        DrawText("KONFIRMASI PENGHANCURAN", panelX + 30, panelY + 30, 24, RED);
        
        std::string bldgText;
        int count = selectedProperty->getBuildingCount();
        if (count == 5) bldgText = "Hotel";
        else bldgText = std::to_string(count) + " rumah";
        
        std::string propLine = "Properti: " + selectedProperty->getName() + " (" + selectedProperty->getCode() + ")";
        DrawText(propLine.c_str(), panelX + 30, panelY + 80, 20, RAYWHITE);
        std::string ownerLine = "Pemilik: " + selectedProperty->getOwner();
        DrawText(ownerLine.c_str(), panelX + 30, panelY + 110, 20, ORANGE);
        std::string bldgLine = "Bangunan saat ini: " + bldgText;
        DrawText(bldgLine.c_str(), panelX + 30, panelY + 140, 20, YELLOW);
        DrawText("Yakin ingin menghancurkan 1 level?", 
                 panelX + 30, panelY + 190, 20, WHITE);
        
        DrawText("[Y] Ya    [N] Tidak", panelX + 30, panelY + 240, 20, WHITE);
    }
    
    if (messageTimer > 0) {
        messageTimer--;
        DrawRectangle(panelX + 50, panelY + panelH - 130, panelW - 100, 40, Fade(DARKBLUE, 0.9f));
        DrawText(message.c_str(), panelX + 60, panelY + panelH - 120, 16, YELLOW);
    }
}
