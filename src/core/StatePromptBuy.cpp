#include "core/StatePromptBuy.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateTurnEnded.hpp"
#include "views/GameGUI.hpp"
#include "models/PropertyTile.hpp"
#include "core/StateAuction.hpp"

void StatePromptBuy::handleInput(GameManager& gm, GameGUI&) {
    Player& p = gm.getCurrentPlayer();
    PropertyTile* tile = dynamic_cast<PropertyTile*>(&gm.getBoard().getTile(p.getPosition()));
    if (IsKeyPressed(KEY_B)) {
        
        if (tile && p.getBalance() >= tile->getBuyPrice()) {
            p -= tile->getBuyPrice(); 
            p.addProperty(tile); 
            tile->setOwner(p.getUsername());
            gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "BUY", "Membeli " + tile->getName());
            
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
    } else if (IsKeyPressed(KEY_L)) {
        // gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "PASS", "Melewati pembelian.");
        // p.setStatus("TURN_ENDED");
        // gm.changeState(std::make_unique<StateTurnEnded>());
        p.setStatus("AUCTION"); // Bebas ini buat log aja
            
        // Masukin SEMUA player ke dalem array lelang
        std::vector<Player*> allPlayers;
        for (Player* playerPtr : gm.getAllPlayers()) {
            // Pastikan player belum bangkrut kalau lu ada sistem bangkrut
            allPlayers.push_back(playerPtr);
        }
        
        gm.changeState(std::make_unique<StateAuction>(tile, allPlayers));
    }
}


void StatePromptBuy::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    Player& p = gm.getCurrentPlayer();
    
    PropertyTile* prop = gm.getBoard().getTile(p.getPosition()).getAsProperty();
    
    if (prop) {
        int baseY = 370;
        DrawRectangle(910, baseY, 280, 180, Fade(SKYBLUE, 0.9f));
        DrawRectangleLines(910, baseY, 280, 180, BLUE);
        
        DrawText("KONFIRMASI BELI", 925, baseY + 15, 18, DARKBLUE);
        DrawText(prop->getName().c_str(), 925, baseY + 45, 16, BLACK);
        DrawText(TextFormat("M%d", prop->getBuyPrice()), 925, baseY + 70, 16, MAROON);
        
        DrawText("[B] BELI  [L] LELANG", 925, baseY + 135, 18, DARKGREEN);
    }
}