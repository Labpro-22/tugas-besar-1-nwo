#pragma once

#include "models/Player.hpp"
#include "models/Tile.hpp"
#include "utils/Dice.hpp"
#include "models/ConfigReader.hpp"
#include "models/CardTile.hpp"
#include "models/FestivalTile.hpp"
#include "models/JailTile.hpp"
#include "models/GoTile.hpp"
#include "models/StreetTile.hpp"
#include "models/RailroadTile.hpp"

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
};