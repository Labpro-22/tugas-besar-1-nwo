#include "GameState.hpp"
class Player;
class GameManager;
class GameGUI;
// #include "StateDemolishSelect.hpp"
// #include "StateDropCard.hpp"
// #include "StateFestivalSelect.hpp"
// #include "StateJailed.hpp"
// #include "StateLassoSelect.hpp"
// #include "StatePromptBuy.hpp"
// #include "StateLiquidation.hpp"
// #include "StatePromptUpgrade.hpp"
// #include "StateTurnEnded.hpp"
// #include "StateTeleportSelect.hpp"
// #include "views/GameGUI.hpp"


class StateWaitingRoll : public GameState {
private:
    char cmdBuffer[100] = "\0";
    int cmdLetterCount = 0;
    void transitionAfterMove(GameManager& gm, Player& p);
public:
    void onEnter(GameManager& gm) override;
    void handleInput(GameManager& gm, GameGUI& gui) override;
    
    // FAKTA: WAJIB DIDEKLARASIKAN DI SINI JUGA!
    void updateUI(GameManager& gm, GameGUI& gui) override; 
};

