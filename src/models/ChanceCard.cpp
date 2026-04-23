#include "models/ChanceCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "utils/InsufficientFundsException.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
ChanceCard::ChanceCard(string desc, string effect) : Card(desc), effectType(effect) {}
void ChanceCard::execute(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "CHANCE", description);

    if (effectType == "BONUS") {
        player += 500;
        player.setStatus("TURN_ENDED");
    } else if (effectType == "PENALTY") {
        try {
            player -= 250;
            player.setStatus("TURN_ENDED");
        } catch (const InsufficientFundsException& e) {
            player.setStatus("LIQUIDATING_250");
        }
    } else if (effectType == "GOTO_JAIL") {
        int jailIdx = gm.getBoard().getTileIndexByCode("JAIL");
        player.setPosition(jailIdx != -1 ? jailIdx : 10);
        player.setStatus("JAILED");
    }
}