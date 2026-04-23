#pragma once

#include <bits/stdc++.h>
#include "core/Board.hpp"
#include "models/Bank.hpp"
#include "utils/Dice.hpp"
#include "utils/TransactionLogger.hpp"
#include "models/ConfigReader.hpp"
#include "utils/CardDeck.hpp" 
#include "core/GameState.hpp"
// class GameState;
class Player;
class PropertyTile;
class GameGUI;

// Forward declaration buat isi template kartu lu
class ChanceCard;
class CommunityChestCard;
class SkillCard;
class GameManager {
private:
    Board board;
    Bank bank;
    Dice dice;
    TransactionLogger logger;
    ConfigReader config;
    
    std::vector<Player*> players;
    int currentTurnIndex;
    int currentTurnCount;
    int maxTurn;
    bool isGameOver;

    CardDeck<ChanceCard> chanceDeck;
    CardDeck<CommunityChestCard> communityDeck;
    CardDeck<SkillCard> skillDeck;
    std::unique_ptr<GameState> currentState;

    bool auctionActive;
    PropertyTile* auctionProperty;
    std::vector<Player*> activeBidders;
    int currentBid;
    Player* highestBidder;
    int auctionTurnIndex;

public:
    GameManager(ConfigReader cfg);
    ~GameManager();

    void initializeGame(int numPlayers, const std::vector<std::string>& playerNames, bool includeCOM);
    void runGameLoop();
    void executeTurn(Player& currentPlayer);
    
    void handleBankruptcy(Player& debtor, Player* creditor);
    void startAuction(PropertyTile& property);
    void checkWinCondition();
    int getCurrentTurnCount(){return currentTurnCount;};

    Board& getBoard();
    const Board& getBoard() const;
    Bank& getBank();
    Dice& getDice();
    int getMaxTurn() const{ return maxTurn; }
    TransactionLogger& getLogger();
    Player& getCurrentPlayer();
    CardDeck<ChanceCard>& getChanceDeck();
    CardDeck<CommunityChestCard>& getCommunityDeck();
    CardDeck<SkillCard>& getSkillDeck();
    void setTurnData(int count, int index) { 
        currentTurnCount = count; 
        currentTurnIndex = index; 
    }
    void processBid(int amount);
    void passAuction();        
    void endAuction();    

    bool isAuctionOngoing() const { return auctionActive; }
    PropertyTile* getAuctionProperty() const { return auctionProperty; }
    int getCurrentBid() const { return currentBid; }
    Player* getHighestBidder() const { return highestBidder; }
    Player* getCurrentAuctionPlayer() const { 
        return activeBidders.empty() ? nullptr : activeBidders[auctionTurnIndex]; 
    }

    void changeState(std::unique_ptr<GameState> newState) {
        currentState = std::move(newState);
        if (currentState) {
            currentState->onEnter(*this);
        }
    }

    void updateStateInput(GameGUI& gui) {
        if (currentState) currentState->handleInput(*this, gui);
    }
    void updateStateUI(GameGUI& gui) {
        if (currentState) currentState->updateUI(*this, gui);
    }
    std::vector<Player*>& getAllPlayers() { return players; }

    // Versi 2: FAKTA mutlak harus ada kata 'const' di awal dan di akhir!
    const std::vector<Player*>& getAllPlayers() const { return players; }

    
};