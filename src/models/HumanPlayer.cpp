#include "models/HumanPlayer.hpp"

using namespace std;
HumanPlayer::HumanPlayer(string name, int startBalance, Color c) : Player(name, startBalance, c) {}
void HumanPlayer::promptTurnAction(GameManager& gm) {
    
    if (this->status == "ACTIVE") {
        this->status = "WAITING_ROLL";
    }
}