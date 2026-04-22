#include "models/ComputerPlayer.hpp"
#include "core/GameManager.hpp"
using namespace std;

ComputerPlayer::ComputerPlayer(string name, int startBalance) : Player(name, startBalance) {}
void ComputerPlayer::promptTurnAction(GameManager& gm) {
    cout << "\n--- GILIRAN COM: " << username << " (M" << balance << ") ---\n";
    
    // 1. Roll Dadu otomatis
    gm.getDice().rollRandom();
    int steps = gm.getDice().getTotal();
    cout << "COM melempar dadu: " << steps << "\n";
    
    int newPos = (position + steps) % gm.getBoard().getTileCount();
    
    // Cek lewat GO
    if (newPos < position && steps > 0) {
        gm.getBoard().getTile(0).onLanded(*this, gm);
    }
    setPosition(newPos);
    
    // 2. Mendarat
    Tile& t = gm.getBoard().getTile(position);
    PropertyTile* prop = dynamic_cast<PropertyTile*>(&t);

    if (prop != nullptr && prop->getOwner() == "BANK") {
        // Logika AI: Kalau duit lebih besar dari harga beli, selalu beli.
        if (balance >= prop->getBuyPrice()) {
            cout << "COM memutuskan untuk MEMBELI " << prop->getName() << ".\n";
            *this -= prop->getBuyPrice();
            addProperty(prop);
            prop->setOwner(username);
        } else {
            cout << "COM tidak punya uang untuk beli, dilempar ke lelang.\n";
            gm.startAuction(*prop);
        }
    } else {
        // Kalau bukan tanah kosong, jalankan logika biasa (bayar sewa, dll)
        t.onLanded(*this, gm);
    }
}

