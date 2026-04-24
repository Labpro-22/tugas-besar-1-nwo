#pragma once

#include <bits/stdc++.h>
#include <raylib.h>
// #include "models/Tile.hpp"
// #include "models/PropertyTile.hpp"
// #include "utils/HandFullException.hpp"
// #include "utils/InsufficientFundsException.hpp"
// #include "models/SkillCard.hpp"
// #include "models/PropertyTile.hpp"

// class PropertyTile;
class SkillCard;
class GameManager;
class Tile;
class PropertyTile;

class Player {
protected:
    std::string username;
    int balance;
    int position;
    std::string status; 
    int jailTurns;
    bool hasUsedSkillCardThisTurn;
    Color playerColor;
    std::vector<SkillCard*> hand;
    std::vector<PropertyTile*> ownedProperties;
public:
    Player(std::string name, int startBalance, Color c);
    virtual ~Player();

    std::string getUsername() const;
    int getBalance() const;
    int getPosition() const;
    std::string getStatus() const;
    int getTotalWealth() const;
    Color getColor() const;

    const std::vector<SkillCard*>& getHand() const;
    const std::vector<PropertyTile*>& getOwnedProperties() const;

    void setPosition(int pos);
    void setStatus(std::string stat);
    void setJailTurns(int turns);
    void setUsedSkillCardThisTurn(bool used);

    Player& operator+=(int amount);
    Player& operator-=(int amount);
    bool operator>(const Player& other) const;
    bool operator<(const Player& other) const;

    void addSkillCard(SkillCard* card);
    void dropSkillCard(int index);
    void addProperty(PropertyTile* prop);
    void removeProperty(PropertyTile* prop);
    virtual bool isBot() const { return false; }
    virtual void takeTurn(GameManager& gm) = 0;
    // virtual void promptTurnAction(GameManager& gm) = 0;
};
