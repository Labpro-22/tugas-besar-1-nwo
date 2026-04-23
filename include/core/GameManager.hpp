#pragma once

#include <bits/stdc++.h>
#include "models/models.hpp"
#include "utils/utils.hpp"
#include "models/HumansPlayer.hpp"
#include "models/ComputerPlayer.hpp"
#include "models/Bank.hpp"

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

public:
    GameManager(ConfigReader cfg);
    ~GameManager();

    void initializeGame(int numPlayers, const std::vector<std::string>& playerNames, bool includeCOM);
    void runGameLoop();
    void executeTurn(Player& currentPlayer);
    
    // Core Mechanics
    void handleBankruptcy(Player& debtor, Player* creditor);
    void startAuction(PropertyTile& property);
    void checkWinCondition();

    // Accessors
    Board& getBoard();
    Bank& getBank();
    Dice& getDice();
    TransactionLogger& getLogger();
    Player& getCurrentPlayer();
    std::vector<Player*>& getAllPlayers();
    CardDeck<ChanceCard>& getChanceDeck();
    CardDeck<CommunityChestCard>& getCommunityDeck();
    CardDeck<SkillCard>& getSkillDeck();
    int getCurrentTurnCount() const;
    int getMaxTurn() const;

    //untuk savemanager
    const Board& getBoardConst() const;
    const Player& getCurrentPlayerConst() const;
    const std::vector<Player*>& getAllPlayersConst() const;
    const CardDeck<SkillCard>& getSkillDeckConst() const;
    const TransactionLogger& getLoggerConst() const;

    //Data-layer
    void setCurrentTurnCount(int t);
    void setMaxTurn(int t);
    void addPlayer(Player* p);
    void setCurrentPlayerByUsername(const std::string& username);
};
