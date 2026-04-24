#include "models/HumanPlayer.hpp"
#include "core/GameManager.hpp"
#include "core/StateWaitingRoll.hpp"
#include "core/StateJailed.hpp"
using namespace std;
HumanPlayer::HumanPlayer(string name, int startBalance, Color c) : Player(name, startBalance, c) {}
// void HumanPlayer::promptTurnAction(GameManager& gm) {
    
//     if (this->status == "ACTIVE") {
//         this->status = "WAITING_ROLL";
//     }
// }
void HumanPlayer::takeTurn(GameManager& gm) {
    if (getStatus() == "JAILED") {
        gm.changeState(std::make_unique<StateJailed>());
    } else {
        gm.changeState(std::make_unique<StateWaitingRoll>());
    }
}