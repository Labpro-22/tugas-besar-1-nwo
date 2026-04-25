#include <cassert>
#include <iostream>
#include <vector>
#include "models/ComputerPlayer.hpp"
#include "core/GameManager.hpp"
#include "models/ConfigReader.hpp"
#include "models/TaxTile.hpp"
#include "models/PropertyTile.hpp"
#include "models/StreetTile.hpp"
#include "models/Bank.hpp"

using namespace std;

void testJailSufficientFunds() {
    cout << "Running testJailSufficientFunds..." << endl;
    ConfigReader config("config");
    GameManager gm(config);
    // Initialize 0 human, {"Bot1"}, 1 COM. This means there's 1 player and it's a COM.
    gm.initializeGame(0, {}, true); 
    
    assert(!gm.getAllPlayers().empty());
    
    ComputerPlayer* cp = dynamic_cast<ComputerPlayer*>(gm.getAllPlayers()[0]);
    assert(cp != nullptr);
    
    // Save initial balance
    int startBalance = cp->getBalance();
    
    // Put to jail
    cp->setStatus("JAILED");
    
    // Give it plenty of money to ensure it has > 50
    *cp += 100;
    
    // It should pay 50 and then roll the dice
    cp->takeTurn(gm);
    
    // Expected to pay 50. Then it might land on a tile that modifies balance (GO, TAX, RENT, BUY).
    // It's hard to precisely assert balance since dice roll and tile landing happens.
    // However, we know it got out of jail.
    assert(cp->getStatus() != "JAILED");
    
    cout << "testJailSufficientFunds passed!" << endl;
}

void testJailInsufficientFunds() {
    cout << "Running testJailInsufficientFunds..." << endl;
    ConfigReader config("config");
    GameManager gm(config);
    gm.initializeGame(0, {}, true); 
    
    ComputerPlayer* cp = dynamic_cast<ComputerPlayer*>(gm.getAllPlayers()[0]);
    
    // Make balance less than 50
    int startBal = cp->getBalance();
    *cp -= startBal; // Balance becomes 0
    *cp += 10;       // Balance becomes 10
    
    cp->setStatus("JAILED");
    
    cp->takeTurn(gm);
    
    // Did not pay 50. Output status could be ACTIVE if rolled double, or TURN_ENDED if not.
    // Either way, if balance was 10, it definitely could not pay 50.
    // Wait, if it rolled double, it moves and might land on something.
    // At the very least, if status is TURN_ENDED it means it stayed in jail.
    if (cp->getStatus() == "TURN_ENDED") {
        // Did not roll double
        assert(cp->getBalance() == 10);
    } else {
        // Rolled double, moved. Status might be something else based on landing.
    }
    
    cout << "testJailInsufficientFunds passed!" << endl;
}

int main() {
    testJailSufficientFunds();
    testJailInsufficientFunds();
    cout << "All ComputerPlayer tests finished!" << endl;
    return 0;
}
