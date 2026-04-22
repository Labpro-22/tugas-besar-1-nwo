#pragma once

#include <bits/stdc++.h>
// #include "models/Tile.hpp"
// #include "models/PropertyTile.hpp"
#include "utils/HandFullException.hpp"
#include "utils/InsufficientFundsException.hpp"
// #include "models/SkillCard.hpp"
#include "models/PropertyTile.hpp"

// class PropertyTile;
class SkillCard;
class GameManager;
class Tile;

class Player {
protected:
    std::string username;
    int balance;
    int position;
    std::string status; 
    int jailTurns;
    bool hasUsedSkillCardThisTurn;
    std::vector<SkillCard*> hand;
    std::vector<PropertyTile*> ownedProperties;

public:
    Player(std::string name, int startBalance);
    virtual ~Player();

    // Getters
    std::string getUsername() const;
    int getBalance() const;
    int getPosition() const;
    std::string getStatus() const;
    int getTotalWealth() const;
    const std::vector<SkillCard*>& getHand() const;
    const std::vector<PropertyTile*>& getOwnedProperties() const;

    // Setters
    void setPosition(int pos);
    void setStatus(std::string stat);
    void setJailTurns(int turns);
    void setUsedSkillCardThisTurn(bool used);

    // Operator Overloading
    Player& operator+=(int amount);
    Player& operator-=(int amount);
    bool operator>(const Player& other) const;
    bool operator<(const Player& other) const;

    // Logic
    void addSkillCard(SkillCard* card);
    void dropSkillCard(int index);
    void addProperty(PropertyTile* prop);
    void removeProperty(PropertyTile* prop);
    
    // Abstract Method untuk COM Bonus
    virtual void promptTurnAction(GameManager& gm) = 0;
};