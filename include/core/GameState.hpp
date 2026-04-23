#pragma once
#include "raylib.h"
class GameManager; 
class GameGUI;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void onEnter(GameManager&) {} 
    virtual void handleInput(GameManager& gm, GameGUI& gui) = 0;
    virtual void updateUI(GameManager& gm, GameGUI& gui) = 0;
};