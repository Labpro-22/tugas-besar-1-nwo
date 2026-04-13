#pragma once

#include <bits/stdc++.h>
#include "core/GameManager.hpp"
class SaveManager {
private:
    std::string saveDirectory;
public:
    SaveManager(std::string dir);
    bool saveGameState(const std::string& filename, const GameManager& gm);
    bool loadGameState(const std::string& filename, GameManager& gm);
};
