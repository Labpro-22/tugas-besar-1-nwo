#include "core/StateAuction.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/GameManager.hpp"
#include "models/Player.hpp"
#include "models/PropertyTile.hpp"
#include "views/GameGUI.hpp"
StateAuction::StateAuction(PropertyTile* prop, std::vector<Player*> allPlayers)
    : property(prop), participants(allPlayers), currentBid(10), highestBidder(nullptr), currentPlayerIndex(0) {
}

void StateAuction::nextPlayer() {
    currentPlayerIndex++;
    if (currentPlayerIndex >= participants.size()) {
        currentPlayerIndex = 0; 
    }
}

void StateAuction::endAuction(GameManager& gm) {
    if (participants.size() == 1) {
        Player* winner = participants[0];
        *winner -= currentBid;
        property->setOwner(winner->getUsername());
        
        gm.getLogger().logAction(gm.getCurrentTurnCount(), "SYSTEM", "LELANG", 
            winner->getUsername() + " memenangkan " + property->getName() + " seharga M" + std::to_string(currentBid));
    } else {
        gm.getLogger().logAction(gm.getCurrentTurnCount(), "SYSTEM", "LELANG", "Semua nyerah, " + property->getName() + " batal terjual.");
    }
    
    // Balik ke giliran selesai
    gm.changeState(std::make_unique<StateTurnEnded>());
}

void StateAuction::handleInput(GameManager& gm, GameGUI& gui) {
    if (participants.empty() || participants.size() == 1) {
        endAuction(gm);
        return;
    }

    Player* p = participants[currentPlayerIndex];
    if (p->getBalance() <= currentBid) {
        gm.getLogger().logAction(gm.getCurrentTurnCount(), p->getUsername(), "AUCTION", p->getUsername() + " otomatis nyerah (Miskin).");
        participants.erase(participants.begin() + currentPlayerIndex);
        if (currentPlayerIndex >= participants.size()) currentPlayerIndex = 0;
        return;
    }
    if (p->getUsername().substr(0, 3) == "COM") {
        
        int maxWillingToPay = property->getBuyPrice() + (p->getBalance() * 0.3);
        
        if (currentBid < maxWillingToPay) {
            currentBid += 10;
            highestBidder = p;
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p->getUsername(), "AUCTION", "COM berani bayar M" + std::to_string(currentBid));
            nextPlayer();
        } else {
            // Harga udah ga ngotak, COM Nyerah!
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p->getUsername(), "AUCTION", "COM mundur dari lelang.");
            participants.erase(participants.begin() + currentPlayerIndex);
            if (currentPlayerIndex >= participants.size()) currentPlayerIndex = 0;
        }
        
        return;
    }

    if (IsKeyPressed(KEY_ONE)) {
        currentBid += 10;
        highestBidder = p;
        nextPlayer();
    } 
    else if (IsKeyPressed(KEY_TWO)) {
        currentBid += 50;
        highestBidder = p;
        nextPlayer();
    } 
    else if (IsKeyPressed(KEY_THREE)) { 
        gm.getLogger().logAction(gm.getCurrentTurnCount(), p->getUsername(), "AUCTION", p->getUsername() + " menyerah.");
        participants.erase(participants.begin() + currentPlayerIndex);
        if (currentPlayerIndex >= participants.size()) currentPlayerIndex = 0;
    }
}
void StateAuction::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    if (participants.empty()) return;
    
    // UI ARENA LELANG (Tengah Layar, Gede)
    DrawRectangle(200, 200, 450, 300, Fade(BLACK, 0.95f));
    DrawRectangleLines(200, 200, 450, 300, GOLD);
    
    DrawText("RUANG LELANG", 330, 220, 24, GOLD);
    DrawText(property->getName().c_str(), 350, 260, 20, RAYWHITE);
    
    // Info Bid Tertinggi
    std::string highestName = (highestBidder) ? highestBidder->getUsername() : "Belum ada";
    DrawText(TextFormat("Bid Tertinggi: M%d", currentBid), 240, 310, 20, LIME);
    DrawText(("Pemegang: " + highestName).c_str(), 240, 340, 18, RAYWHITE);
    
    // Giliran Siapa Sekarang
    Player* p = participants[currentPlayerIndex];
    DrawRectangle(220, 370, 410, 110, Fade(DARKBLUE, 0.6f));
    DrawText(("GILIRAN BID: " + p->getUsername()).c_str(), 240, 385, 20, YELLOW);
    
    DrawText("[1] Bid +M10", 240, 420, 18, LIME);
    DrawText("[2] Bid +M50", 380, 420, 18, LIME);
    DrawText("[3] Nyerah (Fold)", 240, 450, 18, RED);
}