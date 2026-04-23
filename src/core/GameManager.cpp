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
    
    // 1. Tarik dan lindungi nilai maxTurn
    maxTurn = config.getMaxTurn();
    if (maxTurn <= 0) {
        maxTurn = 30; 
    }

    // 2. Tarik dan lindungi nilai saldo awal
    int startBal = config.getInitialBalance();
    if (startBal <= 0) {
        startBal = 150000;
    }
    Color pColors[] = { RED, GREEN, BLUE, ORANGE, PURPLE };
    int colorIdx = 0;
    
    // 3. Inisialisasi pemain manusia
    for (int i = 0; i < numPlayers; i++) {
        players.push_back(new HumanPlayer(playerNames[i], startBal, pColors[colorIdx++]));
    }
    
    // 4. Inisialisasi bot komputer jika diminta
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
    // FAKTA: Fungsi ini dieksekusi 1x setiap kali pemain menekan [ENTER] di akhir gilirannya.
    if (!isGameOver) {
        checkWinCondition();
        
        // 1. Pindah ke index pemain berikutnya
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
        
        // 2. FAKTA KRUSIAL: Set status pemain baru jadi ACTIVE
        // Tanpa ini, main loop di main.cpp nggak akan manggil executeTurn() buat pemain baru!
        // players[currentTurnIndex]->setStatus("ACTIVE");

        // 3. Update counter putaran
        if (currentTurnIndex == 0) {
            currentTurnCount++;
        }

        // 4. Log buat mastiin giliran beneran ganti (Opsional buat debug)
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
// FAKTA: Implementasi buat versi const yang baru lu janjiin di atas
const Board& GameManager::getBoard() const {
    return board;
}
// Tambahkan ini di GameManager.cpp atau kelas tempat lu menaruh logika Lelang
void GameManager::startAuction(PropertyTile& property) {
    // FAKTA: Inisialisasi state lelang, BUKAN loop. UI yang bakal ngulang prosesnya.
    auctionActive = true;
    auctionProperty = &property;
    currentBid = 0; // Harga dasar
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

    // Validasi OOP
    if (bid <= currentBid) {
        throw NimonspoliException("Bid harus lebih besar dari M" + std::to_string(currentBid));
    }
    if (bid > p->getBalance()) {
        throw InsufficientFundsException(bid, p->getBalance());
    }

    // Sah!
    currentBid = bid;
    highestBidder = p;
    logger.logAction(currentTurnCount, p->getUsername(), "BID", "Menawar M" + std::to_string(bid));

    // Pindah giliran nawar
    auctionTurnIndex++;
    if (auctionTurnIndex >= activeBidders.size()) auctionTurnIndex = 0;
}

void GameManager::passAuction() {
    Player* p = activeBidders[auctionTurnIndex];
    logger.logAction(currentTurnCount, p->getUsername(), "PASS", "Mundur dari lelang.");

    // Hapus dari daftar
    activeBidders.erase(activeBidders.begin() + auctionTurnIndex);
    if (auctionTurnIndex >= activeBidders.size()) auctionTurnIndex = 0;

    // FAKTA: Cek kondisi menang atau batal
    if (activeBidders.size() == 1 && highestBidder != nullptr) {
        endAuction();
    } else if (activeBidders.empty()) {
        logger.logAction(currentTurnCount, "SYSTEM", "AUCTION_END", "Tidak ada yang menawar.");
        auctionActive = false; // Batal
    }
}

void GameManager::endAuction() {
    if (highestBidder != nullptr && auctionProperty != nullptr) {
        // Eksekusi data sesuai kode asli lu
        *highestBidder -= currentBid;
        auctionProperty->setOwner(highestBidder->getUsername());
        highestBidder->addProperty(auctionProperty);

        logger.logAction(currentTurnCount, "SYSTEM", "AUCTION_WIN", 
            highestBidder->getUsername() + " memenangkan " + auctionProperty->getName() + 
            " seharga M" + std::to_string(currentBid));
    }
    auctionActive = false; // Matikan status lelang
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

