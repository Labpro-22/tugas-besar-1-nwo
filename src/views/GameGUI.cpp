#include "views/GameGUI.hpp"
#include "core/GameManager.hpp"
#include "models/Tile.hpp"
#include "models/GoTile.hpp"
#include "models/Player.hpp"
#include "models/PropertyTile.hpp"
#include "models/StreetTile.hpp"
#include "models/SkillCard.hpp"
using namespace std;
static GameGUI* globalCLI = nullptr;
enum MenuCmd { START, LOAD, EXIT, UNKNOWN_MENU };
GameGUI::GameGUI(GameManager& manager) : gm(manager) {
    InitWindow(screenWidth, screenHeight, "NIMONSPOLI");
    SetTargetFPS(60);
    initTileMap();
}

void GameGUI::initTileMap() {
    int playAreaWidth = 900;
    int playAreaHeight = 1000;
    
    int boardTotalSize = 11 * tileSize; 
    
    int offsetX = (playAreaWidth - boardTotalSize) / 2;
    int offsetY = (playAreaHeight - boardTotalSize) / 2;

    for (int i = 0; i <= 10; i++) 
        tileMap[i] = { offsetX + (10 - i) * tileSize, offsetY + 10 * tileSize };
    
    for (int i = 11; i <= 19; i++)
        tileMap[i] = { offsetX, offsetY + (10 - (i - 10)) * tileSize };

    for (int i = 20; i <= 30; i++)
        tileMap[i] = { offsetX + (i - 20) * tileSize, offsetY };

    for (int i = 31; i <= 39; i++)
        tileMap[i] = { offsetX + 10 * tileSize, offsetY + (i - 30) * tileSize };
}

void GameGUI::drawTile(int index, Tile& tile) {
    TilePos pos = tileMap[index];
    
    DrawRectangle(pos.x, pos.y, tileSize, tileSize, RAYWHITE);
    DrawRectangleLines(pos.x, pos.y, tileSize, tileSize, BLACK);

    bool hasHeader = tile.hasHeaderColor();
    Color headerColor = tile.getHeaderColor();

    if (hasHeader) {
        DrawRectangle(pos.x + 2, pos.y + 2, tileSize - 4, 18, headerColor);
    }

    Color tileCodeColor = BLACK;
    if (hasHeader) {
        int brightness = headerColor.r + headerColor.g + headerColor.b;
        if (brightness < 450) {
            tileCodeColor = WHITE;
        }
    }

    DrawText(tile.getCode().c_str(), pos.x + 5, pos.y + (hasHeader ? 4 : 20), 13, tileCodeColor);

    if (tile.isProperty()) {
        std::string owner = tile.getOwner();
        
        std::string mText = "M" + std::to_string(tile.getBuyPrice());
        
        if (owner == "BANK") {
            DrawText(mText.c_str(), pos.x + 5, pos.y + tileSize - 15, 11, BLUE);
        } else {
            DrawText(mText.c_str(), pos.x + 5, pos.y + tileSize - 18, 10, BLUE);

            int currentRent = tile.getCurrentRent(gm.getDice(), gm);
            std::string rText = "R: M" + std::to_string(currentRent);
            Color rColor = tile.isFestivalActive() ? GOLD : MAROON;
            DrawText(rText.c_str(), pos.x + 5, pos.y + tileSize - 30, 10, rColor);


            if (tile.isFestivalActive()) {
                DrawCircle(pos.x + tileSize - 10, pos.y + tileSize - 24, 6, GOLD);
                DrawText("F", pos.x + tileSize - 13, pos.y + tileSize - 28, 10, BLACK);
            }

            Color ownerColor = GRAY;
            for (Player* p : gm.getAllPlayers()) {
                if (p->getUsername() == owner) {
                    ownerColor = p->getColor();
                    break;
                }
            }

            DrawRectangle(pos.x, pos.y + tileSize - 8, tileSize, 8, ownerColor);
            DrawRectangleLines(pos.x, pos.y + tileSize - 8, tileSize, 8, BLACK);

            int bCount = tile.getBuildingCount(); 
            if (bCount > 0) {
                int buildY = pos.y + (hasHeader ? 22 : 35); 
                
                if (bCount >= 5) {
                    DrawRectangle(pos.x + tileSize/2 - 10, buildY, 20, 12, ownerColor);
                    DrawRectangleLines(pos.x + tileSize/2 - 10, buildY, 20, 12, WHITE);
                    DrawText("H", pos.x + tileSize/2 - 4, buildY + 2, 10, WHITE);
                } else {
                    for (int b = 0; b < bCount; b++) {
                        int hx = pos.x + 4 + (b * 16); 
                        DrawRectangle(hx, buildY, 12, 10, ownerColor);
                        DrawRectangleLines(hx, buildY, 12, 10, WHITE);
                    }
                }
            }
        }
    }
}


void GameGUI::drawPropertyDeed() {
    if (!showDeedPopup || activeDeed == nullptr) return;

    int cardW = 300;
    int cardH = 400;
    int posX = (screenWidth - cardW) / 2;
    int posY = (screenHeight - cardH) / 2;

    DrawRectangle(posX + 5, posY + 5, cardW, cardH, Fade(BLACK, 0.3f)); 
    DrawRectangle(posX, posY, cardW, cardH, RAYWHITE);
    DrawRectangleLines(posX, posY, cardW, cardH, BLACK);

    Color headerColor = activeDeed->getHeaderColor();
    DrawRectangle(posX + 10, posY + 10, cardW - 20, 60, headerColor);
    DrawRectangleLines(posX + 10, posY + 10, cardW - 20, 60, BLACK);
    
    int brightness = headerColor.r + headerColor.g + headerColor.b;
    Color titleColor = (brightness < 450) ? WHITE : BLACK;
    DrawText(activeDeed->getName().c_str(), posX + 20, posY + 25, 20, titleColor);

    int textY = posY + 85;
    auto drawLineInfo = [&](std::string label, std::string value) {
        DrawText(label.c_str(), posX + 20, textY, 16, DARKGRAY);
        DrawText(value.c_str(), posX + cardW - MeasureText(value.c_str(), 16) - 20, textY, 16, BLACK);
        textY += 25;
    };

    drawLineInfo("Harga Beli:", "M" + std::to_string(activeDeed->getBuyPrice()));
    drawLineInfo("Nilai Gadai:", "M" + std::to_string(activeDeed->getMortgageValue()));
    drawLineInfo("Status:", activeDeed->getPropertyStatus());

    std::string extra = activeDeed->getExtraDetails();
    if (!extra.empty()) {
        DrawLine(posX + 20, textY + 5, posX + cardW - 20, textY + 5, LIGHTGRAY);
        textY += 15;
        drawLineInfo("Detail:", extra);
        drawLineInfo("Sewa Saat Ini:", "M" + std::to_string(activeDeed->calculateRent(gm.getDice(), 0))); 
    }

    DrawText("Tekan [,] untuk Tutup", posX + 60, posY + cardH - 30, 15, GRAY);
    if (IsKeyPressed(KEY_COMMA)) {
        showDeedPopup = false;
        activeDeed = nullptr;
    }
}

void GameGUI::drawPlayers() {
    std::map<int, int> playerCountAtTile;

    for (Player* p : gm.getAllPlayers()) {
        if (p->getStatus() == "BANKRUPT") continue;

        int pPos = p->getPosition();
        int offsetIdx = playerCountAtTile[pPos]++; 

        TilePos pos = tileMap[pPos];

        int dx = (offsetIdx % 2 == 0) ? -12 : 12;
        int dy = (offsetIdx / 2 == 0) ? -12 : 12;

        Color pColor = p->getColor(); 
        
        DrawCircle(pos.x + tileSize/2 + dx, pos.y + tileSize/2 + dy, 11, pColor);
        DrawCircleLines(pos.x + tileSize/2 + dx, pos.y + tileSize/2 + dy, 11, WHITE); // Outline putih biar rapi
        DrawText(p->getUsername().substr(0,1).data(), pos.x + tileSize/2 + dx - 4, pos.y + tileSize/2 + dy - 6, 12, WHITE);
    }
}
void GameGUI::render() {
    for (int i = 0; i < gm.getBoard().getTileCount(); i++) {
        drawTile(i, gm.getBoard().getTile(i));
    }
    drawPlayers();
    drawPlayerHUD();
    drawLogBox(); 
    drawCenterLog(); 

    DrawText("[K] KARTU SKILL  |  [I] DAFTAR ASET", 910, 580, 16, DARKBLUE);

    Player& p = gm.getCurrentPlayer();
    if (p.getStatus() != "WAITING_ROLL" && gm.getDice().getTotal() > 0) {
        int baseY = 320; 
        int die1 = gm.getDice().getDie1();
        int die2 = gm.getDice().getDie2();
        int total = gm.getDice().getTotal();

        DrawText("Hasil Dadu:", 920, baseY + 8, 16, BLACK);
        DrawRectangle(1020, baseY, 30, 30, RAYWHITE);
        DrawRectangleLines(1020, baseY, 30, 30, BLACK);
        DrawText(TextFormat("%d", die1), 1030, baseY + 6, 20, DARKGRAY);

        DrawText("+", 1058, baseY + 8, 16, BLACK);
        DrawRectangle(1075, baseY, 30, 30, RAYWHITE);
        DrawRectangleLines(1075, baseY, 30, 30, BLACK);
        DrawText(TextFormat("%d", die2), 1085, baseY + 6, 20, DARKGRAY);

        DrawText("=", 1113, baseY + 8, 16, BLACK);

        // --- KOTAK TOTAL ---
        DrawRectangle(1130, baseY, 30, 30, GOLD); 
        DrawRectangleLines(1130, baseY, 30, 30, BLACK);
        DrawText(TextFormat("%d", total), (total >= 10 ? 1134 : 1140), baseY + 6, 20, MAROON);
    }
    gm.updateStateUI(*this);
    if (showSkillMenu) {
        drawSkillMenu();
    } else if (showAssetList) {
        drawAssetList(); 
    } else if (showDeedPopup) {
        drawPropertyDeed();
    }

    if (isCommandMode) {
        int baseY = 370;
        DrawRectangle(910, baseY + 115, 280, 40, LIGHTGRAY);
        DrawRectangleLines(910, baseY + 115, 280, 40, BLACK);
        
        std::string cursor = (int(GetTime() * 2) % 2 == 0) ? "_" : "";
        DrawText(("> " + commandText + cursor).c_str(), 915, baseY + 125, 16, MAROON);
        
        DrawText("[F1] Eksekusi | [F2] Batal", 915, baseY + 160, 14, GRAY);
    }

    int activePlayers = 0;
    Player* winner = nullptr;
    
    for (Player* p : gm.getAllPlayers()) {
        if (p->getStatus() != "BANKRUPT") {
            activePlayers++;
            if (winner == nullptr || *p > *winner) {
                winner = p;
            }
        }
    }

    bool isTimeUp = gm.getCurrentTurnCount() > gm.getMaxTurn(); 
    bool isLastManStanding = (activePlayers <= 1);

    if ((isTimeUp || isLastManStanding) && winner != nullptr) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.85f));
        
        const char* title = isTimeUp ? "PERMAINAN SELESAI (WAKTU HABIS)!" : "SEMUA LAWAN BANGKRUT!";
        DrawText(title, (screenWidth - MeasureText(title, 40))/2, 250, 40, RED);

        const char* winText = "PEMENANG MUTLAK:";
        DrawText(winText, (screenWidth - MeasureText(winText, 30))/2, 340, 30, RAYWHITE);
        
        DrawText(winner->getUsername().c_str(), (screenWidth - MeasureText(winner->getUsername().c_str(), 70))/2, 400, 70, GOLD);
        
        const char* wealthText = TextFormat("Total Kekayaan: M%d", winner->getTotalWealth());
        DrawText(wealthText, (screenWidth - MeasureText(wealthText, 25))/2, 510, 25, GREEN);

        const char* exitText = "Tekan [ESC] untuk Keluar";
        DrawText(exitText, (screenWidth - MeasureText(exitText, 20))/2, 850, 20, GRAY);
    }
}

void GameGUI::drawPlayerHUD() {
    Player& p = gm.getCurrentPlayer();
    int panelX = 920;
    int panelY = 40;
    
    DrawRectangle(panelX, panelY, 260, 100, Fade(DARKGRAY, 0.8f));
    DrawRectangleLines(panelX, panelY, 260, 100, SKYBLUE);


    DrawText(TextFormat("GILIRAN: %s", p.getUsername().c_str()), panelX + 15, panelY + 15, 20, RAYWHITE);
    Color statusColor = (p.getStatus() == "ACTIVE" || p.getStatus() == "WAITING_ROLL") ? GREEN : YELLOW;
    DrawText(TextFormat("STATUS: %s", p.getStatus().c_str()), panelX + 15, panelY + 45, 16, statusColor);
    DrawText(TextFormat("SALDO : M%d", p.getBalance()), panelX + 15, panelY + 70, 18, LIME);
}

void GameGUI::drawAssetList() {
    Player& p = gm.getCurrentPlayer();
    const auto& props = p.getOwnedProperties();

    int menuW = 400;
    int menuH = 500;
    int posX = (screenWidth - menuW) / 2;
    int posY = (screenHeight - menuH) / 2;

    DrawRectangle(posX + 8, posY + 8, menuW, menuH, Fade(BLACK, 0.5f)); // Shadow
    DrawRectangle(posX, posY, menuW, menuH, { 30, 40, 55, 255 });       // Dark Navy Theme
    DrawRectangleLinesEx({ (float)posX, (float)posY, (float)menuW, (float)menuH }, 2, SKYBLUE);
    DrawRectangle(posX + 10, posY + 10, menuW - 20, 50, SKYBLUE);
    const char* title = "DAFTAR ASET PROPERTI";
    DrawText(title, posX + (menuW/2) - (MeasureText(title, 22)/2), posY + 25, 22, BLACK);
    if (props.empty()) {
        const char* emptyMsg = "Belum punya tanah bos!";
        DrawText(emptyMsg, posX + (menuW/2) - (MeasureText(emptyMsg, 18)/2), posY + 200, 18, GRAY);
    } else {
        int maxItems = std::min((int)props.size(), 9); 
        
        for (int i = 0; i < maxItems; i++) {
            int itemY = posY + 80 + (i * 40);
            
            DrawRectangle(posX + 15, itemY, menuW - 30, 35, { 50, 60, 75, 255 });
            DrawRectangleLines(posX + 15, itemY, menuW - 30, 35, { 100, 110, 125, 255 });
            DrawRectangle(posX + 15, itemY, 35, 35, SKYBLUE);
            DrawText(TextFormat("%d", i + 1), posX + 25, itemY + 10, 18, BLACK);
            DrawText(props[i]->getName().c_str(), posX + 60, itemY + 10, 18, RAYWHITE);
            StreetTile* street = dynamic_cast<StreetTile*>(props[i]);
            if (street) {
                DrawText(TextFormat("Lvl: %d", street->getBuildingCount()), posX + menuW - 75, itemY + 10, 16, LIGHTGRAY);
            }
        }
        
        if (props.size() > 9) {
            DrawText("... dan aset lainnya (Max tampil 9)", posX + 20, posY + 80 + (9 * 40) + 5, 14, GRAY);
        }
    }

    const char* footerText = "Tekan [1-9] lihat detail | [I] Tutup";
    DrawText(footerText, posX + (menuW/2) - (MeasureText(footerText, 15)/2), posY + menuH - 30, 15, GRAY);
}
void GameGUI::drawSkillMenu() {
    Player& p = gm.getCurrentPlayer();
    const auto& hand = p.getHand();

    int menuW = 350;
    int menuH = 450;
    int posX = (GetScreenWidth() - menuW) / 2;
    int posY = (GetScreenHeight() - menuH) / 2;

    DrawRectangle(posX + 6, posY + 6, menuW, menuH, Fade(BLACK, 0.4f));
    DrawRectangle(posX, posY, menuW, menuH, { 45, 45, 45, 255 }); // Dark theme
    DrawRectangleLinesEx({ (float)posX, (float)posY, (float)menuW, (float)menuH }, 2, GOLD);

    DrawRectangle(posX + 10, posY + 10, menuW - 20, 50, GOLD);
    DrawText("SKILL INVENTORY", posX + (menuW/2) - (MeasureText("SKILL INVENTORY", 22)/2), posY + 25, 22, BLACK);

    if (hand.empty()) {
        DrawText("Tidak ada kartu di tangan.", posX + 50, posY + 200, 18, GRAY);
    } else {
        for (int i = 0; i < (int)hand.size(); i++) {
            int itemY = posY + 80 + (i * 70);
            DrawRectangle(posX + 15, itemY, menuW - 30, 60, { 60, 60, 60, 255 });
            DrawRectangleLines(posX + 15, itemY, menuW - 30, 60, GRAY);
            
            DrawCircle(posX + 40, itemY + 30, 15, GOLD);
            DrawText(TextFormat("%d", i + 1), posX + 36, itemY + 22, 18, BLACK);
            DrawText(hand[i]->getDescription().c_str(), posX + 70, itemY + 20, 16, RAYWHITE);
        }
    }

    DrawText("Tekan [K] untuk Tutup", posX + (menuW/2) - (MeasureText("Tekan [K] untuk Tutup", 15)/2), posY + menuH - 30, 15, GRAY);
}

void GameGUI::drawLogBox() {
    int logX = 910;
    int logY = 620; 
    int logW = 280;
    int logH = 350;

    DrawRectangle(logX, logY, logW, logH, Color{20, 20, 20, 255}); 
    DrawRectangleLines(logX, logY, logW, logH, LIME);
    DrawText("[LOG SYSTEM]", logX + 10, logY + 10, 15, LIME);

    const auto& logs = gm.getLogger().getLogs();
    int start = (logs.size() > 17) ? logs.size() - 17 : 0; 
    
    for (int i = start; i < logs.size(); i++) {
        string logText = logs[i];
        if (logText.length() > 40) logText = logText.substr(0, 37) + "...";
        
        DrawText(logText.c_str(), logX + 10, logY + 35 + (i - start) * 18, 12, LIGHTGRAY);
    }
}

void GameGUI::drawCenterLog() {
    const auto& logs = gm.getLogger().getLogs();
    if (logs.empty()) return;

    string lastLog = logs.back();
    
    Color bgColor = Fade(BLACK, 0.85f);
    Color textColor = RAYWHITE;
    Color borderColor = LIGHTGRAY;
    
    if (lastLog.find("| BUY |") != string::npos) {
        borderColor = GREEN;
        textColor = GREEN;
    } else if (lastLog.find("| RENT |") != string::npos) {
        borderColor = RED;
        textColor = RED;
    } else if (lastLog.find("| FESTIVAL |") != string::npos) {
        borderColor = MAGENTA;
        textColor = MAGENTA;
    } else if (lastLog.find("| TAX |") != string::npos) {
        borderColor = ORANGE;
        textColor = ORANGE;
    } else if (lastLog.find("| AUCTION") != string::npos) {
        borderColor = GOLD;
        textColor = GOLD;
    } else if (lastLog.find("| FREE |") != string::npos || lastLog.find("| GO |") != string::npos || lastLog.find("| CASH |") != string::npos) {
        borderColor = SKYBLUE;
        textColor = SKYBLUE;
    } else if (lastLog.find("| BANKRUPT |") != string::npos) {
        borderColor = MAROON;
        textColor = MAROON;
        bgColor = Fade(RED, 0.3f);
    } else if (lastLog.find("| MORTGAGE |") != string::npos) {
        borderColor = YELLOW;
        textColor = YELLOW;
    } else if (lastLog.find("| UPGRADE |") != string::npos) {
        borderColor = LIME;
        textColor = LIME;
    } else if (lastLog.find("| SKILL |") != string::npos || lastLog.find("| TELEPORT |") != string::npos || lastLog.find("| LASSO |") != string::npos) {
        borderColor = PURPLE;
        textColor = PURPLE;
    } else if (lastLog.find("| ERROR |") != string::npos || lastLog.find("| WARNING |") != string::npos) {
        borderColor = RED;
        textColor = RED;
    }

    string displayLog = lastLog;
    if (displayLog.length() > 65) displayLog = displayLog.substr(0, 62) + "...";

    int fontSize = 20;
    int textWidth = MeasureText(displayLog.c_str(), fontSize);
    
    int padding = 20;
    int boxW = textWidth + padding * 2;
    int boxH = fontSize + padding * 2;
    
    int centerX = 450;
    int centerY = 500;
    
    int boxX = centerX - boxW / 2;
    int boxY = centerY - boxH / 2;

    DrawRectangle(boxX + 5, boxY + 5, boxW, boxH, Fade(BLACK, 0.5f));
    DrawRectangle(boxX, boxY, boxW, boxH, bgColor);
    DrawRectangleLinesEx({ (float)boxX, (float)boxY, (float)boxW, (float)boxH }, 3, borderColor);
    DrawText("LATEST EVENT", boxX + 10, boxY - 20, 16, borderColor);
    DrawText(displayLog.c_str(), boxX + padding, boxY + padding, fontSize, textColor);
}

// enum GameScreen { MAIN_MENU, SETUP_COUNT, SETUP_NAMES, SETUP_COM, GAMEPLAY };


void GameGUI::DrawStyledBox(int x, int y, int w, int h, Color color, const char* text, int fontSize) {
    DrawRectangle(x + 4, y + 4, w, h, Fade(BLACK, 0.3f)); // Shadow
    DrawRectangle(x, y, w, h, color);
    DrawRectangleLinesEx({ (float)x, (float)y, (float)w, (float)h }, 2, WHITE);
    int textW = MeasureText(text, fontSize);
    DrawText(text, x + (w / 2) - (textW / 2), y + (h / 2) - (fontSize / 2), fontSize, WHITE);
}
