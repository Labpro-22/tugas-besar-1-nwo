#include "models/StreetTile.hpp"
#include "utils/Dice.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
using namespace std;
StreetTile::StreetTile(int idx, string c, string n, int price, int mortgage, string color, int hPrice, vector<int> rents)
    : PropertyTile(idx, c, n, "STREET", price, mortgage), colorGroup(color), housePrice(hPrice), rentTable(rents), buildingCount(0), festivalMultiplier(1), festivalDuration(0) {}


void StreetTile::buildHouse() { if(buildingCount < 5) buildingCount++; }
void StreetTile::demolishBuilding() { if(buildingCount > 0) buildingCount--; }
void StreetTile::applyFestival(int multiplier, int duration) { festivalMultiplier = multiplier; festivalDuration = duration; }
void StreetTile::decrementFestival() {
    if (festivalDuration > 0) {
        festivalDuration--;
        if(festivalDuration == 0) festivalMultiplier = 1;
    }
}
Color StreetTile::getHeaderColor() const {
        if (colorGroup == "COKLAT") return BROWN;
        if (colorGroup == "BIRU_MUDA") return SKYBLUE;
        if (colorGroup == "MERAH_MUDA") return  PINK;
        if (colorGroup == "ORANGE") return ORANGE;
        if (colorGroup == "MERAH") return RED;
        if (colorGroup == "KUNING") return YELLOW;
        if (colorGroup == "HIJAU") return GREEN;
        if (colorGroup == "BIRU_TUA") return DARKBLUE;
        return BLACK;
    }
string StreetTile::getColorGroup() const { return colorGroup; }
int StreetTile::getBuildingCount() const { return buildingCount; }
int StreetTile::getTotalBuildingValue() const { return buildingCount * housePrice; }
void StreetTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", "Mendarat di " + name);

    if (ownerName == "BANK") {
        // 1. Belum ada yang punya -> Tawarkan Beli
        player.setStatus("PROMPT_BUY_" + this->code);
    } 
    else if (ownerName == player.getUsername()) {
        // 2. Tanah sendiri -> Tawarkan Upgrade jika belum L5 (Hotel)
        if (this->canUpgrade(gm)) player.setStatus("PROMPT_UPGRADE");
        else player.setStatus("TURN_ENDED");
    } 
    else if (propertyStatus == "MORTGAGED") {
        // 3. Sedang digadai -> Gratis
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", name + " digadai. Bebas sewa!");
        player.setStatus("TURN_ENDED");
    } 
    else {
        // 4. FAKTA: LOGIKA BAYAR SEWA YANG SEMPET ILANG!
        Player* owner = nullptr;
        for (Player* p : gm.getAllPlayers()) {
            if (p->getUsername() == ownerName) { owner = p; break; }
        }

        if (owner != nullptr) {
            int rent = calculateRent(gm.getDice(), 0); 
            
            // Cek saldo, kalau kurang lempar Exception
            if (player.getBalance() < rent) throw InsufficientFundsException(rent, player.getBalance());

            // Transaksi mutlak
            player -= rent;
            *owner += rent;
            
            gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "RENT", 
                "Bayar sewa M" + std::to_string(rent) + " ke " + ownerName);
        }
        player.setStatus("TURN_ENDED");
    }
}

bool StreetTile::canUpgrade(const GameManager& gm) const {
    if (buildingCount >= 5) return false; 

    int totalInGroup = 0;
    int ownedInGroup = 0;
    
    // Sekarang gm.getBoard() bakal manggil versi const!
    for (Tile* t : gm.getBoard().getAllTiles()) {
        if (t->isProperty() && t->getType() == "STREET") {
            // Karena t itu pointer ke Tile, dan lu butuh getColorGroup dari StreetTile,
            // kita harus cast dulu biar bisa panggil fungsi itu.
            const StreetTile* st = dynamic_cast<const StreetTile*>(t);
            
            // Cek apakah warnanya sama dengan petak ini (this->colorGroup)
            if (st && st->getColorGroup() == this->getColorGroup()) { 
                totalInGroup++;
                // Cek apakah pemiliknya sama
                if (st->getOwner() == this->ownerName) {
                    ownedInGroup++;
                }
            }
        }
    }
    
    return (totalInGroup > 0 && ownedInGroup == totalInGroup);
}
