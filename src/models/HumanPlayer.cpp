#include "models/HumansPlayer.hpp"

using namespace std;
HumanPlayer::HumanPlayer(string name, int startBalance) : Player(name, startBalance) {}
void HumanPlayer::promptTurnAction(GameManager& gm) {
    // Hanya mengubah status sebagai sinyal untuk GameGUI, bukan menjalankan loop terminal
    if (this->status == "ACTIVE") {
        this->status = "WAITING_ROLL";
    }
}