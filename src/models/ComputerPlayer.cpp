#include "models/ComputerPlayer.hpp"
#include "core/GameManager.hpp"
#include "models/TaxTile.hpp"
#include "models/StreetTile.hpp"
using namespace std;
ComputerPlayer::ComputerPlayer(string name, int startBalance, Color c) : Player(name, startBalance, c) {}
void ComputerPlayer::promptTurnAction(GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "START_TURN", "Giliran COM (Saldo: M" + to_string(balance) + ")");

   
    if (status == "JAILED") {
        if (balance >= 50) {
            *this -= 50; 
            gm.getBank().collect(50, gm); 
            setStatus("ACTIVE");
            gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "FREE", "COM bayar denda M50.");
        } else {
            gm.getDice().rollRandom();
            if (gm.getDice().isDouble()) {
                setStatus("ACTIVE");
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "FREE", "COM dapat Double! Bebas.");
            } else {
                setStatus("TURN_ENDED");
                return;
            }
        }
    }

    // 2. KOCOK DADU & JALAN
    gm.getDice().rollRandom();
    int steps = gm.getDice().getTotal();
    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "DICE", "COM melempar dadu: " + to_string(steps));

    int oldPos = position;
    int newPos = (position + steps) % gm.getBoard().getTileCount();
    if (newPos < oldPos && steps > 0) {
        *this += 200; // Pass GO
        gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "GO", "COM lewat START, dapat M200.");
    }
    setPosition(newPos);

    // 3. MENDARAT (Ini bisa merubah status jadi PROMPT_BUY, TAX_CHOICE, dll)
    try {
        gm.getBoard().getTile(newPos).onLanded(*this, gm);
    } catch (const InsufficientFundsException& e) {
        setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
    }

    // 4. AI AUTO-RESOLVER (Meniru Manusia Mencet Tombol)
    bool resolving = true;
    while (resolving) {
        string s = getStatus();

        if (s.find("PROMPT_BUY_") == 0) {
            PropertyTile* prop = dynamic_cast<PropertyTile*>(&gm.getBoard().getTile(position));
            if (prop && balance >= prop->getBuyPrice()) {
                *this -= prop->getBuyPrice();
                // FAKTA: Uang masuk ke bank
                gm.getBank().collect(prop->getBuyPrice(), gm); 
                addProperty(prop);
                prop->setOwner(username);
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "BUY", "COM membeli " + prop->getName());
            } else if (prop) {
                gm.startAuction(*prop);
            }
            setStatus("TURN_ENDED");
        } 
        else if (s == "TAX_CHOICE") {
            TaxTile* tt = dynamic_cast<TaxTile*>(&gm.getBoard().getTile(position));
            int flat = tt->getFlatTax();
            int pct = getTotalWealth() * 0.1;
            int taxToPay = std::min(flat, pct); // AI Cerdas: Pilih pajak yang paling murah
            
            try {
                *this -= taxToPay;
                // FAKTA: Uang pajak masuk ke bank
                gm.getBank().collect(taxToPay, gm); 
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "TAX", "COM bayar pajak M" + to_string(taxToPay));
                setStatus("TURN_ENDED");
            } catch (const InsufficientFundsException& e) {
                setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
            }
        }
        else if (s == "FESTIVAL_SELECT") {
            if (!ownedProperties.empty()) {
                StreetTile* st = dynamic_cast<StreetTile*>(ownedProperties[0]); // Pilih properti pertama yang dia punya
                if (st) st->applyFestival(2, 1);
            }
            setStatus("TURN_ENDED");
        }
        else if (s == "DROP_CARD") {
            dropSkillCard(0); // Auto buang kartu paling kiri
            gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "DISCARD", "COM membuang 1 kartu.");
            setStatus("TURN_ENDED");
        }
        else if (s.find("LIQUIDATING_") == 0) {
            // AI Sederhana: Coba gadai properti pertama yang belum digadai
            bool saved = false;
            for (PropertyTile* prop : ownedProperties) {
                if (prop->getPropertyStatus() != "MORTGAGED") {
                    prop->mortgageProperty();
                    *this += prop->getMortgageValue();
                    // FAKTA: Uang gadai cair dari bank
                    gm.getBank().dispense(prop->getMortgageValue(), gm); 
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "MORTGAGE", "COM menggadai " + prop->getName());
                    saved = true;
                    break; // Cukup gadai satu dulu, loop lagi nanti kalau masih kurang
                }
            }
            if (!saved) { // Kalau udah nggak ada yang bisa digadai
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "BANKRUPT", "COM BANGKRUT!");
                gm.handleBankruptcy(*this, nullptr); 
                setStatus("BANKRUPT");
            } else {
                setStatus("TURN_ENDED"); 
            }
        }
        else {
            // Kalau statusnya udah "TURN_ENDED" atau "BANKRUPT", keluar dari loop
            resolving = false;
        }
    }
}