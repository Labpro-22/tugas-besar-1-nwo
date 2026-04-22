#include "models/Bank.hpp"
#include "core/GameManager.hpp"
using namespace std;

Bank::Bank() {}
void Bank::collect(int amount, GameManager& gm) {
    gm.getLogger().logAction(
        gm.getCurrentTurnCount(),
        "BANK", 
        "COLLECT", 
        "Menerima dana sebesar M" + std::to_string(amount)
    );
}

void Bank::dispense(int amount, GameManager& gm) {
    gm.getLogger().logAction(
        gm.getCurrentTurnCount(), 
        "BANK", 
        "DISPENSE", 
        "Mengeluarkan dana sebesar M" + std::to_string(amount)
    );
}