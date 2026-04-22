#include "core/GameManager.hpp"

class HumanPlayer;
class ComputerPlayer;
using namespace std;
GameManager::GameManager(ConfigReader cfg) 
    : config(cfg), board(), bank(), dice(), currentTurnIndex(0), currentTurnCount(1), maxTurn(0), isGameOver(false) {}

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
    
    for (int i = 0; i < numPlayers; i++) {
        players.push_back(new HumanPlayer(playerNames[i], startBal));
    }
    
    if (includeCOM) {
        players.push_back(new ComputerPlayer("COM", startBal));
    }
    getSkillDeck().addCard(new MoveCard(4));
    getSkillDeck().addCard(new DiscountCard(50));
    getSkillDeck().addCard(new ShieldCard());
    getSkillDeck().addCard(new TeleportCard());
    getSkillDeck().addCard(new LassoCard());
    getSkillDeck().addCard(new DemolitionCard());

    getSkillDeck().shuffle();
}
void GameManager::runGameLoop() {
    if (!isGameOver) {
        checkWinCondition();
        
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
        players[currentTurnIndex]->setStatus("ACTIVE");

        if (currentTurnIndex == 0) {
            currentTurnCount++;
        }

        logger.logAction(currentTurnCount, players[currentTurnIndex]->getUsername(), "SYSTEM", "Mulai giliran.");
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
void GameManager::startAuction(PropertyTile& property) {
    cout << "\n--- LELANG DIMULAI UNTUK " << property.getName() << " ---\n";
    
    vector<Player*> activeBidders;
    for (Player* p : players) {
        if (p->getStatus() != "BANKRUPT") {
            activeBidders.push_back(p);
        }
    }

    int highestBid = 0;
    Player* highestBidder = nullptr;
    int currentIndex = 0;

    while (activeBidders.size() > 1) {
        Player* p = activeBidders[currentIndex];
        
        cout << "\nGiliran: " << p->getUsername() << " (Saldo: M" << p->getBalance() << ")\n";
        cout << "Bid Tertinggi: M" << highestBid << "\n";
        cout << "Masukkan bid (Ketik 0 untuk mundur): ";
        
        int bid;
        
        if (!(cin >> bid)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << ">> ERROR: Input tidak valid. Harus berupa angka!\n";
            continue; 
        }

        if (bid == 0) {
            cout << ">> " << p->getUsername() << " mundur dari lelang.\n";
            activeBidders.erase(activeBidders.begin() + currentIndex);
            
            if (currentIndex >= activeBidders.size()) currentIndex = 0;
            continue;
        }

        if (bid <= highestBid) {
            cout << ">> ERROR: Bid harus lebih besar dari M" << highestBid << "!\n";
            continue;
        }

        if (bid > p->getBalance()) {
            cout << ">> ERROR: Uang tidak cukup! Saldo kamu hanya M" << p->getBalance() << ".\n";
            continue;
        }

        highestBid = bid;
        highestBidder = p;
        cout << ">> SAH! " << p->getUsername() << " memimpin lelang dengan bid M" << highestBid << ".\n";

        currentIndex++;
        if (currentIndex >= activeBidders.size()) currentIndex = 0;
    }

    if (highestBidder != nullptr) {
        cout << "\n*** LELANG SELESAI ***\n";
        cout << ">> " << highestBidder->getUsername() << " memenangkan " << property.getName() 
             << " seharga M" << highestBid << "!\n";
             
        *highestBidder -= highestBid;
        property.setOwner(highestBidder->getUsername()); 
    } else {
        cout << "\n>> Tidak ada yang menawar. " << property.getName() << " tetap tidak dimiliki.\n";
    }
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

    // Accessors
Board& GameManager::getBoard() { return board; }
Bank& GameManager::getBank() { return bank; }
Dice& GameManager::getDice() { return dice; }
TransactionLogger& GameManager::getLogger() { return logger; }
Player& GameManager::getCurrentPlayer() { return *players[currentTurnIndex]; }
vector<Player*>& GameManager::getAllPlayers() { return players; }
CardDeck<ChanceCard>& GameManager::getChanceDeck() { return chanceDeck; }
CardDeck<CommunityChestCard>& GameManager::getCommunityDeck() { return communityDeck; }
CardDeck<SkillCard>& GameManager::getSkillDeck() { return skillDeck; }
int GameManager::getCurrentTurnCount() const { return currentTurnCount; }
