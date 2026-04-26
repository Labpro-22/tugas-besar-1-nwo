#include "core/GameManager.hpp"
#include "models/ConfigReader.hpp"
#include "models/Player.hpp"
#include <iostream>

using namespace std;

int main() {
    try {
        ConfigReader config("config");
        GameManager gm(config);
        vector<string> names;
        gm.initializeGame(0, names, true);
        cout << "Initialization successful!" << endl;
        
        auto& p = gm.getCurrentPlayer();
        p.takeTurn(gm);
        cout << "Turn taken successfully!" << endl;
    } catch (const exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
    return 0;
}
