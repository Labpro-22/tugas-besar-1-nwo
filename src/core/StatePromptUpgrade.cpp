#include "core/StatePromptUpgrade.hpp"
#include "core/StateTurnEnded.hpp"
#include "models/Player.hpp"
#include "models/StreetTile.hpp"
#include "core/GameManager.hpp"
#include "views/GameGUI.hpp"

void StatePromptUpgrade::resetState() {
    phase = Phase::SELECT_GROUP;
    selectedColorGroup = "";
    selectedProperty = nullptr;
    groupNames.clear();
    monopolyGroups.clear();
    message = "";
    messageTimer = 0;
}

void StatePromptUpgrade::onEnter(GameManager& gm) {
    resetState();
    buildMonopolyGroups(gm);
}

void StatePromptUpgrade::buildMonopolyGroups(GameManager& gm) {
    Player& p = gm.getCurrentPlayer();
    
    std::map<std::string, int> groupTotals;
    std::map<std::string, int> groupOwned;
    std::map<std::string, std::vector<StreetTile*>> groupProperties;
    
    for (Tile* t : gm.getBoard().getAllTiles()) {
        if (t->isStreet()) {
            StreetTile* st = dynamic_cast<StreetTile*>(t);
            if (st) {
                std::string cg = st->getColorGroup();
                groupTotals[cg]++;
                if (st->getOwner() == p.getUsername()) {
                    groupOwned[cg]++;
                    groupProperties[cg].push_back(st);
                }
            }
        }
    }
    
    for (auto& [cg, total] : groupTotals) {
        if (groupOwned[cg] == total && total > 0) {
            monopolyGroups[cg] = groupProperties[cg];
            groupNames.push_back(cg);
        }
    }
}

bool StatePromptUpgrade::canBuildOn(StreetTile* target) {
    if (!target || target->getBuildingCount() >= 5) return false;
    auto it = monopolyGroups.find(selectedColorGroup);
    if (it == monopolyGroups.end()) return false;
    
    int minCount = 5;
    for (StreetTile* st : it->second) {
        if (st->getBuildingCount() < minCount) {
            minCount = st->getBuildingCount();
        }
    }
    return target->getBuildingCount() == minCount;
}

void StatePromptUpgrade::handleInput(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    Player& p = gm.getCurrentPlayer();
    
    if (phase == Phase::SELECT_GROUP) {
        if (IsKeyPressed(KEY_ZERO)) {
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
            return;
        }
        
        for (int i = 0; i < 9; i++) {
            if (IsKeyPressed(KEY_ONE + i) && i < (int)groupNames.size()) {
                selectedColorGroup = groupNames[i];
                phase = Phase::SELECT_PROPERTY;
                return;
            }
        }
    }
    else if (phase == Phase::SELECT_PROPERTY) {
        if (IsKeyPressed(KEY_ZERO)) {
            phase = Phase::SELECT_GROUP;
            selectedColorGroup = "";
            return;
        }
        
        auto it = monopolyGroups.find(selectedColorGroup);
        if (it == monopolyGroups.end()) {
            phase = Phase::SELECT_GROUP;
            return;
        }
        
        auto& properties = it->second;
        for (int i = 0; i < 9; i++) {
            if (IsKeyPressed(KEY_ONE + i) && i < (int)properties.size()) {
                StreetTile* target = properties[i];
                if (!canBuildOn(target)) {
                    message = "Tidak dapat dibangun! Pemerataan tidak terpenuhi.";
                    messageTimer = 120;
                    return;
                }
                
                int price = (target->getBuildingCount() == 4) ? target->getHotelPrice() : target->getHousePrice();
                if (p.getBalance() < price) {
                    message = "Dana tidak mencukupi!";
                    messageTimer = 120;
                    return;
                }
                
                if (target->getBuildingCount() < 4) {
                    p -= price;
                    gm.getBank().collect(price, gm);
                    target->build();
                    message = "Kamu membangun 1 rumah di " + target->getName() + ". Biaya: M" + std::to_string(price);
                    messageTimer = 120;
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "UPGRADE", 
                        "Membangun rumah di " + target->getName());
                }
                else if (target->getBuildingCount() == 4) {
                    selectedProperty = target;
                    phase = Phase::CONFIRM_HOTEL;
                }
                return;
            }
        }
    }
    else if (phase == Phase::CONFIRM_HOTEL) {
        if (IsKeyPressed(KEY_Y)) {
            int price = selectedProperty->getHotelPrice();
            if (p.getBalance() >= price) {
                p -= price;
                gm.getBank().collect(price, gm);
                selectedProperty->build();
                message = selectedProperty->getName() + " di-upgrade ke Hotel! Biaya: M" + std::to_string(price);
                messageTimer = 120;
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "UPGRADE", 
                    "Upgrade ke hotel di " + selectedProperty->getName());
            } else {
                message = "Dana tidak mencukupi untuk upgrade hotel!";
                messageTimer = 120;
            }
            selectedProperty = nullptr;
            phase = Phase::SELECT_PROPERTY;
        }
        else if (IsKeyPressed(KEY_N)) {
            selectedProperty = nullptr;
            phase = Phase::SELECT_PROPERTY;
        }
    }
}

void StatePromptUpgrade::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    Player& p = gm.getCurrentPlayer();
    
    int panelX = 150;
    int panelY = 150;
    int panelW = 900;
    int panelH = 700;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.92f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, GREEN);
    
    if (phase == Phase::SELECT_GROUP) {
        if (groupNames.empty()) {
            DrawText("Tidak ada color group yang memenuhi syarat untuk dibangun.", 
                     panelX + 30, panelY + 30, 20, RED);
            DrawText("Kamu harus memiliki seluruh petak dalam satu color group terlebih dahulu.", 
                     panelX + 30, panelY + 60, 18, RAYWHITE);
            DrawText("Tekan [0] untuk batal", panelX + 30, panelY + 120, 18, GRAY);
            return;
        }
        
        DrawText("=== Color Group yang Memenuhi Syarat ===", panelX + 30, panelY + 20, 22, GREEN);
        
        int y = panelY + 60;
        for (int i = 0; i < (int)groupNames.size() && i < 9; i++) {
            std::string cg = groupNames[i];
            std::string groupLine = "[" + std::to_string(i + 1) + "] [" + cg + "]";
            DrawText(groupLine.c_str(), panelX + 30, y, 18, YELLOW);
            y += 25;
            
            auto it = monopolyGroups.find(cg);
            if (it != monopolyGroups.end()) {
                for (StreetTile* st : it->second) {
                    std::string bldgText;
                    int count = st->getBuildingCount();
                    if (count == 5) bldgText = "Hotel";
                    else bldgText = std::to_string(count) + " rumah";
                    
                    int displayPrice = (count == 4) ? st->getHotelPrice() : st->getHousePrice();
                    std::string detailLine = "    - " + st->getName() + " (" + st->getCode() + ") : " + bldgText + " (Harga: M" + std::to_string(displayPrice) + ")";
                    DrawText(detailLine.c_str(), panelX + 50, y, 16, RAYWHITE);
                    y += 22;
                }
                y += 10;
            }
        }
        
        std::string balanceLine = "Uang kamu saat ini : M" + std::to_string(p.getBalance());
        DrawText(balanceLine.c_str(), panelX + 30, panelY + panelH - 80, 18, GOLD);
        DrawText("Pilih nomor color group (0 untuk batal)", 
                 panelX + 30, panelY + panelH - 50, 18, WHITE);
    }
    else if (phase == Phase::SELECT_PROPERTY) {
        std::string titleLine = "Color group [" + selectedColorGroup + "]:";
        DrawText(titleLine.c_str(), panelX + 30, panelY + 20, 22, GREEN);
        
        auto it = monopolyGroups.find(selectedColorGroup);
        if (it != monopolyGroups.end()) {
            int y = panelY + 60;
            auto& properties = it->second;
            
            bool allFour = true;
            for (StreetTile* st : properties) {
                if (st->getBuildingCount() != 4) {
                    allFour = false;
                    break;
                }
            }
            if (allFour && !properties.empty()) {
                std::string hotelMsg = "Seluruh color group [" + selectedColorGroup + "] sudah memiliki 4 rumah. Siap di-upgrade ke hotel!";
                DrawText(hotelMsg.c_str(), panelX + 30, y, 16, LIME);
                y += 30;
            }
            
            for (int i = 0; i < (int)properties.size() && i < 9; i++) {
                StreetTile* st = properties[i];
                std::string bldgText;
                int count = st->getBuildingCount();
                if (count == 5) bldgText = "Hotel";
                else bldgText = std::to_string(count) + " rumah";
                
                std::string statusText;
                Color statusColor = RAYWHITE;
                if (count == 5) {
                    statusText = " <- sudah maksimal, tidak dapat dibangun";
                    statusColor = GRAY;
                } else if (canBuildOn(st)) {
                    if (count == 4) {
                        statusText = " <- siap upgrade ke hotel";
                        statusColor = LIME;
                    } else {
                        statusText = " <- dapat dibangun";
                        statusColor = SKYBLUE;
                    }
                } else {
                    statusText = " <- tidak dapat dibangun";
                    statusColor = RED;
                }
                
                std::string line = "[" + std::to_string(i + 1) + "] " + st->getName() + " (" + st->getCode() + ") : " + bldgText + statusText;
                DrawText(line.c_str(), panelX + 30, y, 16, statusColor);
                y += 25;
            }
            
            std::string balanceLine2 = "Uang kamu saat ini : M" + std::to_string(p.getBalance());
            DrawText(balanceLine2.c_str(), panelX + 30, panelY + panelH - 80, 18, GOLD);
            DrawText("Pilih petak (0 untuk kembali)", 
                     panelX + 30, panelY + panelH - 50, 18, WHITE);
        }
    }
    else if (phase == Phase::CONFIRM_HOTEL) {
        DrawText("UPGRADE KE HOTEL?", panelX + 30, panelY + 30, 24, GOLD);
        std::string propLine = "Properti: " + selectedProperty->getName();
        DrawText(propLine.c_str(), panelX + 30, panelY + 80, 20, RAYWHITE);
        std::string costLine = "Biaya: M" + std::to_string(selectedProperty->getHotelPrice());
        DrawText(costLine.c_str(), panelX + 30, panelY + 110, 20, MAROON);
        DrawText("[Y] Ya    [N] Tidak", panelX + 30, panelY + 160, 20, WHITE);
    }
    
    if (messageTimer > 0) {
        messageTimer--;
        DrawRectangle(panelX + 50, panelY + panelH - 130, panelW - 100, 40, Fade(DARKBLUE, 0.9f));
        DrawText(message.c_str(), panelX + 60, panelY + panelH - 120, 16, YELLOW);
    }
}
