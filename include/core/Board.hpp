#pragma once

#include <bits/stdc++.h>
// #include "models/Player.hpp"
// #include "models/Tile.hpp"
// #include "utils/Dice.hpp"
// #include "models/ConfigReader.hpp"
// #include "models/CardTile.hpp"
// #include "models/FestivalTile.hpp"
// #include "models/JailTile.hpp"
// #include "models/GoTile.hpp"
// #include "models/StreetTile.hpp"
// #include "models/RailroadTile.hpp"
// #include "models/GoToJailTile.hpp"
// #include "models/UtilityTile.hpp"
class ConfigReader;
class Tile;
class Board {
private:
    std::vector<Tile*> tiles;
public:
    Board();
    ~Board();
    void initDynamicBoard(ConfigReader& config);
    Tile& getTile(int index) const;
    int getTileCount() const;
    int getTileIndexByCode(std::string code) const;
    const std::vector<Tile*>& getAllTiles() const { return tiles; }
};
