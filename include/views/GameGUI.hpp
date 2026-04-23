#include <raylib.h>
#include <bits/stdc++.h>
// #include "core/GameManager.hpp"
// #include "models/Tile.hpp"
// #include "models/GoTile.hpp"

class Tile;
class PropertyTile;
class GameManager;
class GameGUI {
private:
    GameManager& gm;
    const int screenWidth = 1200;
    const int screenHeight = 1000;
    const int tileSize = 80;
    const int boardOffset = 50;

    PropertyTile* activeDeed = nullptr; 
    bool showDeedPopup = false;
    bool showSkillMenu = false;
    bool showAssetList = false;

    bool isCommandMode = false;
    std::string commandText = "";

    struct TilePos { int x, y; };
    std::map<int, TilePos> tileMap;

    void initTileMap();
    void drawTile(int index, Tile& tile);
    void drawPlayers();
    // void drawUI(); 
    void drawLogBox();
    void drawPlayerHUD();
    void drawAssetList();
    void drawSkillMenu();
    void drawPropertyDeed(); 

public:
    GameGUI(GameManager& manager);
    
    void render(); 

    void openDeed(PropertyTile* prop) { 
        if (prop != nullptr) {
            activeDeed = prop; showDeedPopup = true; showAssetList = false; 
        }
    }
    void closeDeed() { activeDeed = nullptr; showDeedPopup = false; }
    bool isDeedOpen() const { return showDeedPopup; }

    void toggleSkillMenu() { 
        showSkillMenu = !showSkillMenu; 
        if(showSkillMenu) { showAssetList = false; showDeedPopup = false; }
    }
    void closeSkillMenu() { showSkillMenu = false; }
    bool isSkillMenuOpen() const { return showSkillMenu; }

    void toggleAssetList() { 
        showAssetList = !showAssetList; 
        if(showAssetList) { showSkillMenu = false; showDeedPopup = false; }
    }
    void closeAssetList() { showAssetList = false; }
    bool isAssetListOpen() const { return showAssetList; }
    
    bool isAnyMenuOpen() const { return showSkillMenu || showDeedPopup || showAssetList; }
    void toggleCommandMode() { isCommandMode = !isCommandMode; commandText = ""; }
    bool getCommandMode() const { return isCommandMode; }
    void setCommandText(std::string text) { commandText = text; }
    void DrawStyledBox(int x, int y, int w, int h, Color color, const char* text, int fontSize) ;
};