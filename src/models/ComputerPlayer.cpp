#include "models/ComputerPlayer.hpp"
#include "core/GameManager.hpp"
#include "models/TaxTile.hpp"
#include "models/StreetTile.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/StateLiquidation.hpp"

using namespace std;

ComputerPlayer::ComputerPlayer(string name, int startBalance, Color c) : Player(name, startBalance, c) {}

void ComputerPlayer::takeTurn(GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "START_TURN", "Giliran COM (Saldo: M" + to_string(balance) + ")");

    // 1. CEK PENJARA
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
                gm.changeState(std::make_unique<StateTurnEnded>()); // Lempar ke UI nunggu ENTER
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

    // 3. MENDARAT
    try {
        gm.getBoard().getTile(newPos).onLanded(*this, gm);
    } catch (const InsufficientFundsException& e) {
        setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
    }

    // 4. AI AUTO-RESOLVER
    bool resolving = true;
    while (resolving) {
        string s = getStatus();
        if (s.find("PROMPT_BUY_") == 0) {
            // FAKTA: 100% PURE POLYMORPHISM. No Casting!
            PropertyTile* prop = gm.getBoard().getTile(position).getAsProperty();
            
            if (prop && balance >= prop->getBuyPrice()) {
                *this -= prop->getBuyPrice();
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
            // FAKTA: 100% PURE POLYMORPHISM. No Casting!
            TaxTile* tt = gm.getBoard().getTile(position).getAsTax();
            
            if (tt) {
                int flat = tt->getFlatTax();
                int pct = getTotalWealth() * 0.1;
                int taxToPay = std::min(flat, pct); 
                
                try {
                    *this -= taxToPay;
                    gm.getBank().collect(taxToPay, gm); 
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "TAX", "COM bayar pajak M" + to_string(taxToPay));
                    setStatus("TURN_ENDED");
                } catch (const InsufficientFundsException& e) {
                    setStatus("LIQUIDATING_" + to_string(e.getRequired() - e.getAvailable()));
                }
            } else {
                setStatus("TURN_ENDED");
            }
        }
        // else if (s == "FESTIVAL_SELECT") {
        //     if (!ownedProperties.empty()) {
        //         StreetTile* st = dynamic_cast<StreetTile*>(ownedProperties[0]);
        //         if (st) st->applyFestival(2, 1);
        //     }
        //     setStatus("TURN_ENDED");
        // }
        else if (s == "DROP_CARD") {
            dropSkillCard(0);
            gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "DISCARD", "COM membuang 1 kartu.");
            setStatus("TURN_ENDED");
        }
        else if (s.find("PROMPT_UPGRADE_") == 0) {
            PropertyTile* prop = gm.getBoard().getTile(position).getAsProperty();
            
            if (prop && prop->canBuild() && prop->getHouseCount() < 4) {
                if (balance >= (prop->getHousePrice() + 200)) {
                    *this -= prop->getHousePrice();
                    gm.getBank().collect(prop->getHousePrice(), gm);
                    prop->build();
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "UPGRADE", "COM membangun properti di " + prop->getName());
                }
            }
            setStatus("TURN_ENDED");
        }
        else if (s == "FESTIVAL") { // Atau apapun nama status lu pas AI nginjek petak Festival
            const auto& props = getOwnedProperties();
            
            if (!props.empty()) {
                // Cari properti dengan jumlah rumah terbanyak (Paling untung!)
                PropertyTile* bestProp = props[0];
                int maxHouses = -1;
                
                for (PropertyTile* prop : props) {
                    if (prop->getHouseCount() > maxHouses) {
                        maxHouses = prop->getHouseCount();
                        bestProp = prop;
                    }
                }
                
                // FAKTA: NOL CASTING! Langsung eksekusi buff-nya
                bestProp->applyFestival(2, 1);
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "FESTIVAL", "COM mengadakan pesta di " + bestProp->getName());
            } else {
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "FESTIVAL", "COM gak punya aset buat di-festival-in.");
            }
            
            setStatus("TURN_ENDED");
        }
        else if (s.find("LIQUIDATING_") == 0) {
            bool saved = false;
            for (PropertyTile* prop : ownedProperties) {
                if (prop->getPropertyStatus() != "MORTGAGED") {
                    prop->mortgageProperty();
                    *this += prop->getMortgageValue();
                    gm.getBank().dispense(prop->getMortgageValue(), gm); 
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "MORTGAGE", "COM menggadai " + prop->getName());
                    saved = true;
                    break; 
                }
            }
            if (!saved) { 
                gm.getLogger().logAction(gm.getCurrentTurnCount(), username, "BANKRUPT", "COM BANGKRUT!");
                gm.handleBankruptcy(*this, nullptr); 
                setStatus("BANKRUPT");
            } else {
                setStatus("TURN_ENDED"); 
            }
        }
        else {
            resolving = false;
        }
    }

    gm.changeState(std::make_unique<StateTurnEnded>());
}