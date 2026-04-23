#include <bits/stdc++.h>

#include "core/core.hpp"
#include "models/models.hpp"
#include "utils/utils.hpp"
#include "views/views.hpp"
#include "raylib.h"
enum GameScreen { MAIN_MENU, SETUP_COUNT, SETUP_NAMES, SETUP_COM, GAMEPLAY };

int main() {
    // const int screenWidth = 1200;
    // const int screenHeight = 960;

    ConfigReader config("config");
    GameManager gm(config);
    GameGUI gui(gm);
    
    GameScreen currentScreen = MAIN_MENU;
    int numHumanPlayers = 0;
    std::vector<std::string> playerNames;
    char nameBuffer[16] = "\0"; 
    int letterCount = 0;
    int currentPlayerNaming = 1;
    bool closeWindowFromMenu = false;

    char cmdBuffer[100] = "\0";  
    int cmdLetterCount = 0;
    while (!WindowShouldClose() && !closeWindowFromMenu) {
        
        // ==========================================================
        // 1. LOGIKA UPDATE (Input & State)
        // ==========================================================
        switch (currentScreen) {
            case MAIN_MENU:
                if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_S)) currentScreen = SETUP_COUNT;
                if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_L)) {
                    try {
                        SaveManager saver("config"); 
                        
                        // FAKTA: Lempar "" biar Zenity terbuka buat milih file
                        if (saver.loadGameState("", gm)) {
                            // Opsional: Catat ke logger kalau berhasil load
                            gm.getLogger().logAction(gm.getCurrentTurnCount(), "SYSTEM", "LOAD", "Game berhasil dimuat!");
                            currentScreen = GAMEPLAY;
                        }
                    } catch (const FileException& e) {
                        std::cout << ">> GAGAL LOAD: " << e.what() << "\n";
                    } catch (const std::exception& e) {
                        std::cout << ">> ERROR SISTEM: " << e.what() << "\n";
                    }
                }
                if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_E)) closeWindowFromMenu = true;
                break;

            case SETUP_COUNT:
                if (IsKeyPressed(KEY_ONE))   { numHumanPlayers = 1; currentScreen = SETUP_NAMES; }
                else if (IsKeyPressed(KEY_TWO))   { numHumanPlayers = 2; currentScreen = SETUP_NAMES; }
                else if (IsKeyPressed(KEY_THREE)) { numHumanPlayers = 3; currentScreen = SETUP_NAMES; }
                else if (IsKeyPressed(KEY_FOUR))  { numHumanPlayers = 4; currentScreen = SETUP_NAMES; }
                break;

            case SETUP_NAMES: {
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (letterCount < 15)) {
                        nameBuffer[letterCount] = (char)key;
                        nameBuffer[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                    letterCount--;
                    nameBuffer[letterCount] = '\0';
                }
                if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                    playerNames.push_back(std::string(nameBuffer));
                    if (currentPlayerNaming < numHumanPlayers) {
                        currentPlayerNaming++;
                        nameBuffer[0] = '\0';
                        letterCount = 0;
                    } else currentScreen = SETUP_COM;
                }
                break;
            }

            case SETUP_COM:
                if (IsKeyPressed(KEY_Y)) { gm.initializeGame(numHumanPlayers, playerNames, true); currentScreen = GAMEPLAY; }
                else if (IsKeyPressed(KEY_N)) { gm.initializeGame(numHumanPlayers, playerNames, false); currentScreen = GAMEPLAY; }
                break;
            case GAMEPLAY: {
                Player& p = gm.getCurrentPlayer();

                // ==============================================================
                // 1. KONTROL MENU INVENTORY (Overlay Bebas State)
                // ==============================================================
                if (IsKeyPressed(KEY_K)) gui.toggleSkillMenu(); 
                if (IsKeyPressed(KEY_I)) gui.toggleAssetList(); 

                // --- [LOGIKA PAKAI KARTU SKILL] ---
                if (gui.isSkillMenuOpen()) {
                    int selected = -1;
                    if (IsKeyPressed(KEY_ONE))   selected = 0;
                    if (IsKeyPressed(KEY_TWO))   selected = 1;
                    if (IsKeyPressed(KEY_THREE)) selected = 2;
                    if (IsKeyPressed(KEY_FOUR))  selected = 3;

                    if (selected != -1 && selected < (int)p.getHand().size()) {
                        p.getHand()[selected]->execute(p, gm); 
                        p.dropSkillCard(selected); 
                        gui.closeSkillMenu(); 
                    }
                }

                // --- [LOGIKA PILIH ASET PROPERTI] ---
                if (gui.isAssetListOpen() && !gui.isDeedOpen()) {
                    int selectedIdx = -1;
                    for (int i = 0; i < 9; i++) {
                        if (IsKeyPressed(KEY_ONE + i)) selectedIdx = i;
                    }
                    if (selectedIdx != -1 && selectedIdx < (int)p.getOwnedProperties().size()) {
                        gui.openDeed(p.getOwnedProperties()[selectedIdx]); 
                    }
                }

                // ==============================================================
                // 2. THE REAL OOP ENGINE (Sapu Bersih Semua if-else Lama!)
                // ==============================================================
                // FAKTA: Semua logika SPACE, B, L, 1, 2, ENTER sekarang MURNI
                // dikendalikan oleh objek State yang lagi aktif!
                
                if (!gui.isAnyMenuOpen()) {
                    gm.updateStateInput(gui); 
                }

                break;
            }
        }

        // ==========================================================
        // 2. RENDERING (Visual Only)
        // ==========================================================
        BeginDrawing();
        ClearBackground({ 240, 240, 245, 255 }); // Off-white clean background
        int centerX = GetScreenWidth() / 2;

        switch (currentScreen) {
            case MAIN_MENU:
                DrawText("NIMONSPOLI", centerX - (MeasureText("NIMONSPOLI", 60) / 2), 200, 60, DARKBLUE);
                DrawText("The Ultimate Monopoly Adventure", centerX - (MeasureText("The Ultimate Monopoly Adventure", 20) / 2), 270, 20, GRAY);
                
                
                gui.DrawStyledBox(centerX - 150, 400, 300, 60, SKYBLUE, "[1] START GAME", 20);
                gui.DrawStyledBox(centerX - 150, 480, 300, 60, DARKGREEN, "[2] LOAD GAME", 20);
                gui.DrawStyledBox(centerX - 150, 560, 300, 60, MAROON, "[3] EXIT", 20);
                break;

            case SETUP_COUNT:
                DrawText("PERSIAPAN", centerX - (MeasureText("PERSIAPAN", 40) / 2), 250, 40, DARKGRAY);
                DrawText("Berapa pemain manusia?", centerX - (MeasureText("Berapa pemain manusia?", 25) / 2), 320, 25, BLACK);
                
                for(int i = 1; i <= 4; i++) {
                    gui.DrawStyledBox(centerX - 100, 380 + (i-1)*70, 200, 50, DARKBLUE, TextFormat("Pemain: %d", i), 20);
                }
                break;

            case SETUP_NAMES:
                DrawText(TextFormat("NAMA PEMAIN #%i", currentPlayerNaming), centerX - 150, 250, 35, DARKBLUE);
                DrawRectangle(centerX - 200, 350, 400, 60, WHITE);
                DrawRectangleLinesEx({ (float)centerX - 200, 350, 400, 60 }, 3, DARKBLUE);
                DrawText(nameBuffer, centerX - 180, 365, 30, BLACK);
                DrawText("Tekan ENTER untuk lanjut", centerX - 120, 430, 20, DARKGRAY);
                break;

            case SETUP_COM:
                DrawText("TAMBAHKAN BOT (COM)?", centerX - 200, 350, 30, DARKBLUE);
                gui.DrawStyledBox(centerX - 220, 450, 200, 60, GREEN, "[Y] YA", 20);
                gui.DrawStyledBox(centerX + 20, 450, 200, 60, RED, "[N] TIDAK", 20);
                break;

            case GAMEPLAY:
                gui.render(); 
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}