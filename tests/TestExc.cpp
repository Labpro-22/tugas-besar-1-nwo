#include "models/Player.hpp"
#include "models/HumanPlayer.hpp"
#include <iostream>

using namespace std;

int main() {
    HumanPlayer p("Test", 100, RED);
    p -= p.getBalance();
    cout << "Balance 100: OK" << endl;
    
    HumanPlayer p2("Test", 0, RED);
    p2 -= p2.getBalance();
    cout << "Balance 0: OK" << endl;
    
    HumanPlayer p3("Test", -100, RED);
    p3 -= p3.getBalance();
    cout << "Balance -100: OK" << endl;

    return 0;
}
