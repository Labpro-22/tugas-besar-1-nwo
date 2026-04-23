#include "models/HumanPlayer.hpp"

using namespace std;
HumanPlayer::HumanPlayer(string name, int startBalance, Color c) : Player(name, startBalance, c) {}
void HumanPlayer::promptTurnAction(GameManager& gm) {
    // Hanya mengubah status sebagai sinyal untuk GameGUI, bukan menjalankan loop terminal
    if (this->status == "ACTIVE") {
        this->status = "WAITING_ROLL";
    }
}