#include "core/StateRedeem.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "models/PropertyTile.hpp"
#include "models/StreetTile.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"

void StateRedeem::resetState() {
    phase = Phase::SELECT_PROPERTY;
    mortgagedProperties.clear();
    selectedProperty = nullptr;
    message = "";
    messageTimer = 0;
}

void StateRedeem::onEnter(GameManager& gm) {
    resetState();
    scanMortgagedProperties(gm);
}

void StateRedeem::scanMortgagedProperties(GameManager& gm) {
    Player& p = gm.getCurrentPlayer();
    
    for (Tile* t : gm.getBoard().getAllTiles()) {
        if (t->isProperty()) {
            PropertyTile* prop = dynamic_cast<PropertyTile*>(t);
            if (prop && prop->getOwner() == p.getUsername() && prop->getPropertyStatus() == "MORTGAGED") {
                mortgagedProperties.push_back(prop);
            }
        }
    }
}

void StateRedeem::handleInput(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    Player& p = gm.getCurrentPlayer();
    
    if (phase == Phase::SELECT_PROPERTY) {
        if (IsKeyPressed(KEY_ZERO)) {
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
            return;
        }
        
        for (int i = 0; i < 9; i++) {
            if (IsKeyPressed(KEY_ONE + i) && i < (int)mortgagedProperties.size()) {
                selectedProperty = mortgagedProperties[i];
                phase = Phase::CONFIRM_REDEEM;
                return;
            }
        }
    }
    else if (phase == Phase::CONFIRM_REDEEM) {
        if (IsKeyPressed(KEY_Y)) {
            if (selectedProperty) {
                int redeemPrice = selectedProperty->getMortgageValue();
                
                if (p.getBalance() >= redeemPrice) {
                    std::string propName = selectedProperty->getName();
                    p -= redeemPrice;
                    gm.getBank().dispense(redeemPrice, gm);
                    selectedProperty->redeemProperty();
                    
                    message = propName + " berhasil ditebus! Kamu membayar M" + std::to_string(redeemPrice) + " ke Bank.";
                    messageTimer = 120;
                    
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "REDEEM", 
                        "Menebus " + propName + " dengan M" + std::to_string(redeemPrice));
                } else {
                    message = "Uang kamu tidak cukup untuk menebus " + selectedProperty->getName() + 
                              ". Harga tebus: M" + std::to_string(redeemPrice) + " | Uang kamu: M" + std::to_string(p.getBalance());
                    messageTimer = 120;
                }
            }
            selectedProperty = nullptr;
            phase = Phase::SELECT_PROPERTY;
            
            // End turn after one redeem attempt (success or fail)
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
        else if (IsKeyPressed(KEY_N)) {
            selectedProperty = nullptr;
            phase = Phase::SELECT_PROPERTY;
        }
    }
}

void StateRedeem::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    int panelX = 150;
    int panelY = 150;
    int panelW = 900;
    int panelH = 700;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.92f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, GOLD);
    
    if (phase == Phase::SELECT_PROPERTY) {
        DrawText("=== TEBUS GADAI PROPERTI ===", panelX + 30, panelY + 20, 24, GOLD);
        
        if (mortgagedProperties.empty()) {
            DrawText("Tidak ada properti yang sedang digadaikan.", 
                     panelX + 30, panelY + 80, 20, GRAY);
            DrawText("Tekan [0] untuk batal", panelX + 30, panelY + 130, 18, WHITE);
            return;
        }
        
        int y = panelY + 70;
        for (int i = 0; i < (int)mortgagedProperties.size() && i < 9; i++) {
            PropertyTile* prop = mortgagedProperties[i];
            std::string colorTag = "";
            if (prop->isStreet()) {
                StreetTile* st = dynamic_cast<StreetTile*>(prop);
                if (st) colorTag = "   [" + st->getColorGroup() + "]";
            }
            
            std::string line = "[" + std::to_string(i + 1) + "] " + prop->getName() + " (" + prop->getCode() + ")";
            line += colorTag + "  [M]  Harga Tebus: M" + std::to_string(prop->getMortgageValue());
            DrawText(line.c_str(), panelX + 30, y, 18, RAYWHITE);
            y += 28;
        }
        
        std::string balanceLine = "Uang kamu saat ini : M" + std::to_string(gm.getCurrentPlayer().getBalance());
        DrawText(balanceLine.c_str(), panelX + 30, panelY + panelH - 80, 18, GOLD);
        DrawText("Pilih nomor properti (0 untuk batal)", 
                 panelX + 30, panelY + panelH - 50, 18, WHITE);
    }
    else if (phase == Phase::CONFIRM_REDEEM) {
        DrawText("KONFIRMASI TEBUS GADAI", panelX + 30, panelY + 30, 24, GOLD);
        
        std::string propLine = "Properti: " + selectedProperty->getName() + " (" + selectedProperty->getCode() + ")";
        DrawText(propLine.c_str(), panelX + 30, panelY + 80, 20, RAYWHITE);
        std::string costLine = "Harga Tebus: M" + std::to_string(selectedProperty->getMortgageValue());
        DrawText(costLine.c_str(), panelX + 30, panelY + 110, 20, MAROON);
        
        int redeemPrice = selectedProperty->getMortgageValue();
        if (gm.getCurrentPlayer().getBalance() >= redeemPrice) {
            DrawText("Yakin ingin menebus properti ini?", panelX + 30, panelY + 160, 20, WHITE);
        } else {
            std::string warnLine = "Uang tidak cukup! Kamu punya: M" + std::to_string(gm.getCurrentPlayer().getBalance());
            DrawText(warnLine.c_str(), panelX + 30, panelY + 160, 20, RED);
        }
        
        DrawText("[Y] Ya    [N] Tidak", panelX + 30, panelY + 210, 20, WHITE);
    }
    
    if (messageTimer > 0) {
        messageTimer--;
        DrawRectangle(panelX + 50, panelY + panelH - 130, panelW - 100, 40, Fade(DARKBLUE, 0.9f));
        DrawText(message.c_str(), panelX + 60, panelY + panelH - 120, 16, YELLOW);
    }
}
