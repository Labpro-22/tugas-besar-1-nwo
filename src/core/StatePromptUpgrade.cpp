#include "core/StatePromptUpgrade.hpp"
#include "core/StateTurnEnded.hpp"
#include "models/Player.hpp"
#include "models/PropertyTile.hpp" 
#include "core/GameManager.hpp"
#include "views/GameGUI.hpp"

void StatePromptUpgrade::handleInput(GameManager& gm, GameGUI& gui) {
    Player& p = gm.getCurrentPlayer();
    
    PropertyTile* prop = gm.getBoard().getTile(p.getPosition()).getAsProperty();
    
    if (prop && prop->canBuild()) {
        if (IsKeyPressed(KEY_ONE)) {
            if (p.getBalance() >= prop->getHousePrice() && prop->getHouseCount() < 4) {
                p -= prop->getHousePrice();
                gm.getBank().collect(prop->getHousePrice(), gm);
                prop->build(); 
                
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "UPGRADE", "Membangun properti di " + prop->getName());
            } else {
                gm.getLogger().logAction(gm.getCurrentTurnCount(), p.getUsername(), "WARNING", "Dana kurang atau properti maksimal!");
            }
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
            
        } else if (IsKeyPressed(KEY_TWO)) {
            p.setStatus("TURN_ENDED");
            gm.changeState(std::make_unique<StateTurnEnded>());
        }
    } else {
        p.setStatus("TURN_ENDED");
        gm.changeState(std::make_unique<StateTurnEnded>());
    }
}

void StatePromptUpgrade::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    std::cout << "[DEBUG] 1. Masuk updateUI Upgrade..." << std::endl;
    Player& p = gm.getCurrentPlayer();
    
    std::cout << "[DEBUG] 2. Ngambil posisi player: " << p.getPosition() << std::endl;
    
    PropertyTile* prop = gm.getBoard().getTile(p.getPosition()).getAsProperty();
    std::cout << "[DEBUG] 3. Pointer PropertyTile aman..." << std::endl;
    
    if (prop) {
        std::cout << "[DEBUG] 4. Cek canBuild()..." << std::endl;
        if (prop->canBuild()) {
            
            std::cout << "[DEBUG] 5. Ambil Nama Properti: " << prop->getName() << std::endl;
            std::cout << "[DEBUG] 6. Ambil Harga Rumah: " << prop->getHousePrice() << std::endl;
            
            DrawRectangle(200, 300, 400, 200, Fade(BLACK, 0.9f));
            DrawRectangleLines(200, 300, 400, 200, GREEN);
            
            std::string title = "UPGRADE " + prop->getName() + "?";
            DrawText(title.c_str(), 220, 330, 22, GREEN);
            
            DrawText(TextFormat("Harga 1 Rumah: M%d", prop->getHousePrice()), 230, 370, 20, RAYWHITE);
            DrawText("[1] Bangun Rumah", 230, 410, 20, LIME);
            DrawText("[2] Lewati", 230, 440, 20, RED);
            
            std::cout << "[DEBUG] 7. Selesai ngegambar UI!" << std::endl;
        }
    }
}