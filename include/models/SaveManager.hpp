#pragma once

#include <bits/stdc++.h>
class GameManager;
class SaveManager {
private:
    std::string saveDirectory;
    std::string openLoadFileDialog();
    std::string openSaveFileDialog();
public:
    SaveManager(std::string dir);
    bool saveGameState(const std::string& filename, const GameManager& gm);
    bool loadGameState(const std::string& filename, GameManager& gm);
};