#include "core/GameManager.hpp"
#include "models/models.hpp"
#include "utils/utils.hpp"
#include "models/HumanPlayer.hpp"
#include "models/ComputerPlayer.hpp"
#include "models/Bank.hpp"
#include "core/GameState.hpp"
#include "core/Board.hpp"
#include "core/StateWaitingRoll.hpp"
#include "models/Bank.hpp"
class HumanPlayer;
class ComputerPlayer;
using namespace std;

GameManager::GameManager(ConfigReader cfg) 
    : config(cfg), board(), bank(), dice(), 
      currentTurnIndex(0), currentTurnCount(1), maxTurn(0), isGameOver(false),
      auctionActive(false), auctionProperty(nullptr), currentBid(0), highestBidder(nullptr), auctionTurnIndex(0) {}
GameManager::~GameManager() {
    for (Player* p : players) delete p;
}

void GameManager::initializeGame(int numPlayers, const vector<string>& playerNames, bool includeCOM) {
    config.loadAllConfigs();
    board.initDynamicBoard(config);
    
    maxTurn = config.getMaxTurn();
    if (maxTurn <= 0) {
        maxTurn = 30; 
    }

    int startBal = config.getInitialBalance();
    if (startBal <= 0) {
        startBal = 150000;
    }
    Color pColors[] = { RED, GREEN, BLUE, ORANGE, PURPLE };
    int colorIdx = 0;
    
    for (int i = 0; i < numPlayers; i++) {
        players.push_back(new HumanPlayer(playerNames[i], startBal, pColors[colorIdx++]));
    }
    
    if (includeCOM) {
        players.push_back(new ComputerPlayer("COM", startBal, pColors[colorIdx++]));
    }
    getSkillDeck().addCard(new MoveCard(4));
    getSkillDeck().addCard(new DiscountCard(50));
    getSkillDeck().addCard(new ShieldCard());
    getSkillDeck().addCard(new TeleportCard());
    getSkillDeck().addCard(new LassoCard());
    getSkillDeck().addCard(new DemolitionCard());

    getSkillDeck().shuffle();
    setTurnData(1,0);
    changeState(make_unique<StateWaitingRoll>());

}
void GameManager::runGameLoop() {
    
    if (!isGameOver) {
        checkWinCondition();
    
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
        
        
        if (currentTurnIndex == 0) {
            currentTurnCount++;
        }

       
        logger.logAction(currentTurnCount, players[currentTurnIndex]->getUsername(), "SYSTEM", "Mulai giliran.");
        players[currentTurnIndex]->setStatus("WAITING_ROLL");
        changeState(std::make_unique<StateWaitingRoll>());
    }
}
void GameManager::executeTurn(Player& currentPlayer) {
    if (currentPlayer.getStatus() == "BANKRUPT") return;
    currentPlayer.promptTurnAction(*this);
}
void GameManager::handleBankruptcy(Player& debtor, Player* creditor) {
    debtor.setStatus("BANKRUPT");
    cout << debtor.getUsername() << " telah BANGKRUT!\n";

    vector<PropertyTile*> assets = debtor.getOwnedProperties();

    for (int i = 0; i < assets.size(); i++) {
        PropertyTile* prop = assets[i];
        
        if (creditor != nullptr) {
            prop->setOwner(creditor->getUsername());
            creditor->addProperty(prop);
        } else {
            prop->setOwner("BANK");
            startAuction(*prop);
        }
    }
}

const Board& GameManager::getBoard() const {
    return board;
}

void GameManager::startAuction(PropertyTile& property) {
    
    auctionProperty = &property;
    currentBid = 0; 
    highestBidder = nullptr;
    auctionTurnIndex = 0;

    activeBidders.clear();
    for (Player* p : players) {
        if (p->getStatus() != "BANKRUPT") {
            activeBidders.push_back(p);
        }
    }

    logger.logAction(currentTurnCount, "SYSTEM", "AUCTION_START", 
        "Lelang " + property.getName() + " dimulai!");
}

void GameManager::processBid(int bid) {
    Player* p = activeBidders[auctionTurnIndex];
    if (bid <= currentBid) {
        throw NimonspoliException("Bid harus lebih besar dari M" + std::to_string(currentBid));
    }
    if (bid > p->getBalance()) {
        throw InsufficientFundsException(bid, p->getBalance());
    }

    currentBid = bid;
    highestBidder = p;
    logger.logAction(currentTurnCount, p->getUsername(), "BID", "Menawar M" + std::to_string(bid));

    auctionTurnIndex++;
    if (auctionTurnIndex >= activeBidders.size()) auctionTurnIndex = 0;
}

void GameManager::passAuction() {
    Player* p = activeBidders[auctionTurnIndex];
    logger.logAction(currentTurnCount, p->getUsername(), "PASS", "Mundur dari lelang.");

    activeBidders.erase(activeBidders.begin() + auctionTurnIndex);
    if (auctionTurnIndex >= activeBidders.size()) auctionTurnIndex = 0;

    if (activeBidders.size() == 1 && highestBidder != nullptr) {
        endAuction();
    } else if (activeBidders.empty()) {
        logger.logAction(currentTurnCount, "SYSTEM", "AUCTION_END", "Tidak ada yang menawar.");
        auctionActive = false; 
    }
}

void GameManager::endAuction() {
    if (highestBidder != nullptr && auctionProperty != nullptr) {

        *highestBidder -= currentBid;
        auctionProperty->setOwner(highestBidder->getUsername());
        highestBidder->addProperty(auctionProperty);

        logger.logAction(currentTurnCount, "SYSTEM", "AUCTION_WIN", 
            highestBidder->getUsername() + " memenangkan " + auctionProperty->getName() + 
            " seharga M" + std::to_string(currentBid));
    }
    auctionActive = false; 
}


void GameManager::checkWinCondition() {
    if (isGameOver) return;
    if (currentTurnCount > maxTurn) {
        isGameOver = true;
        
        Player* winner = nullptr;
        for (int i = 0; i < players.size(); i++) {
            if (players[i]->getStatus() != "BANKRUPT") {
                winner = players[i];
                break;
            }
        }
        if (winner != nullptr) {
            for (int i = 0; i < players.size(); i++) {
                if (players[i]->getStatus() != "BANKRUPT") {
                    if (*players[i] > *winner) {
                        winner = players[i];
                    }
                }
            }
            cout << "Pemenang: " << winner->getUsername() << " dengan kekayaan M" << winner->getTotalWealth() << "\n";
        }
    }
}
// Accessors GameManager
Board& GameManager::getBoard() { return board; }
Bank& GameManager::getBank() { return bank; }
Dice& GameManager::getDice() { return dice; }
TransactionLogger& GameManager::getLogger() { return logger; }
Player& GameManager::getCurrentPlayer() { return *players[currentTurnIndex]; }
CardDeck<ChanceCard>& GameManager::getChanceDeck() { return chanceDeck; }
CardDeck<CommunityChestCard>& GameManager::getCommunityDeck() { return communityDeck; }
CardDeck<SkillCard>& GameManager::getSkillDeck() { return skillDeck; }

