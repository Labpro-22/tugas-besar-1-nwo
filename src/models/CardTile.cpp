#include "models/CardTile.hpp"
#include "core/GameManager.hpp"
#include "models/SkillCard.hpp"
#include "models/Player.hpp"
#include "utils/HandFullException.hpp"
using namespace std;

CardTile::CardTile(int idx, string c, string n, string type) 
    : Tile(idx, c, n, type), deckType(type) {}
void CardTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "DRAW", "Mengambil kartu...");
    
    SkillCard* drawnCard = gm.getSkillDeck().drawCard(); 
    if (!drawnCard) return;

    
    try {
        player.addSkillCard(drawnCard); 
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "KEEP", "Simpan: " + drawnCard->getDescription());
        player.setStatus("TURN_ENDED");
    } catch (const HandFullException& e) {
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "HAND_FULL", "Tas penuh!");
        gm.getSkillDeck().discardCard(drawnCard);
        player.setStatus("DROP_CARD"); 
    }
}
