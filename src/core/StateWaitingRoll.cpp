#include "core/StateWaitingRoll.hpp"
#include "core/StateDemolishSelect.hpp"
#include "core/StateDropCard.hpp"
#include "core/StateFestivalSelect.hpp"
#include "core/StateJailed.hpp"
#include "core/StateLassoSelect.hpp"
#include "core/StatePromptBuy.hpp"
#include "core/StateLiquidation.hpp"
#include "core/StatePromptUpgrade.hpp"
#include "core/StateTurnEnded.hpp"
#include "core/StateTeleportSelect.hpp"
#include "views/GameGUI.hpp"
#include "models/Player.hpp"
#include "models/SaveManager.hpp"
#include "models/PropertyTile.hpp"
#include "utils/FileException.hpp"
#include "utils/InsufficientFundsException.hpp"
#include "core/GameManager.hpp"


void StateWaitingRoll::updateUI(GameManager& gm, GameGUI& gui) {
    if (gui.isAnyMenuOpen()) return;
    
    int baseY = 370; 
    DrawText("[SPACE] KOCOK DADU", 920, baseY + 30, 22, DARKGREEN);
    DrawText("[S] SAVE GAME", 920, baseY + 65, 16, DARKBLUE);
    DrawText("[C] COMMAND MODE", 920, baseY + 85, 16, PURPLE);
}

void StateWaitingRoll::transitionAfterMove(GameManager& gm, Player& p) {
    std::string s = p.getStatus();
    
    if (s.find("PROMPT_BUY") == 0) {
        gm.changeState(std::make_unique<StatePromptBuy>());
    } else if (s.find("TAX_CHOICE") == 0) {
        gm.changeState(std::make_unique<StateTaxChoice>());
    } else if (s.find("JAILED") == 0) {
        gm.changeState(std::make_unique<StateJailed>());
    } else if (s.find("TURN_ENDED") == 0) {
        gm.changeState(std::make_unique<StateTurnEnded>());
    } else if (s.find("LIQUIDATING") == 0) {
        gm.changeState(std::make_unique<StateLiquidation>());
    }
    else if (s == "FESTIVAL_SELECT") {
        gm.changeState(std::make_unique<StateFestivalSelect>());
    } else if (s == "DROP_CARD") {
        gm.changeState(std::make_unique<StateDropCard>());
    } else if (s == "TELEPORT_SELECT") {
        gm.changeState(std::make_unique<StateTeleportSelect>());
    } else if (s == "LASSO_SELECT") {
        gm.changeState(std::make_unique<StateLassoSelect>());
    } else if (s == "DEMOLISH_SELECT") {
        gm.changeState(std::make_unique<StateDemolishSelect>());
    }
    else if (s == "PROMPT_UPGRADE") gm.changeState(std::make_unique<StatePromptUpgrade>()); // FAKTA: Tambah ini!

}

void StateWaitingRoll::onEnter(GameManager& gm) {
    cmdBuffer[0] = '\0';
    cmdLetterCount = 0;
    gm.getCurrentPlayer().setStatus("WAITING_ROLL");
}

void StateWaitingRoll::handleInput(GameManager& gm, GameGUI& gui) {
    Player& p = gm.getCurrentPlayer();

    if (gui.isAnyMenuOpen()) return; 

    if (!gui.getCommandMode()) {
        if (IsKeyPressed(KEY_C)) {
            gui.toggleCommandMode();
            while (GetCharPressed() > 0) {} 
        }
        if (IsKeyPressed(KEY_S)) {
            try {
                SaveManager saver("config"); 
                if (saver.saveGameState("", gm)) {
                    gm.getLogger().logAction(gm.getCurrentTurnCount(), "SYSTEM", "SAVE", "Game disimpan!");
                }
            } catch (const FileException& e) {
                gm.getLogger().logAction(gm.getCurrentTurnCount(), "SYSTEM", "ERROR", e.what());
            }
        }
        if (IsKeyPressed(KEY_SPACE)) {
            gm.getDice().rollRandom();
            int steps = gm.getDice().getTotal();
            
            int oldPos = p.getPosition();
            int nPos = (oldPos + steps) % gm.getBoard().getTileCount();
            if (nPos < oldPos) p += 200; 
            p.setPosition(nPos);
            p.setStatus("TURN_ENDED"); 
            
            try { 
                gm.getBoard().getTile(nPos).onLanded(p, gm); 
            } catch (const InsufficientFundsException& e) {
                p.setStatus("LIQUIDATING_" + std::to_string(e.getRequired() - e.getAvailable()));
            }
            
            
            transitionAfterMove(gm, p);
            
            
            return; 
        }
    } else {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (cmdLetterCount < 90)) {
                cmdBuffer[cmdLetterCount] = (char)toupper(key);
                cmdBuffer[cmdLetterCount + 1] = '\0';
                cmdLetterCount++;
                gui.setCommandText(std::string(cmdBuffer));
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && cmdLetterCount > 0) {
            cmdLetterCount--;
            cmdBuffer[cmdLetterCount] = '\0';
            gui.setCommandText(std::string(cmdBuffer));
        }
        if (IsKeyPressed(KEY_F2)) {
            gui.toggleCommandMode(); 
            cmdBuffer[0] = '\0';
            cmdLetterCount = 0;
            gui.setCommandText("");
        }
        if (IsKeyPressed(KEY_F1) && cmdLetterCount > 0) {
            std::string cmd = std::string(cmdBuffer);
            if (cmd.find("ATUR_DADU ") == 0) {
                int d1 = 0, d2 = 0;
                if (sscanf(cmd.c_str(), "ATUR_DADU %d %d", &d1, &d2) == 2) {
                    gm.getDice().setManual(d1, d2);
                    int steps = gm.getDice().getTotal();
                    int oldPos = p.getPosition();
                    int nPos = (oldPos + steps) % gm.getBoard().getTileCount();
                    if (nPos < oldPos) p += 200;
                    p.setPosition(nPos);
                    p.setStatus("TURN_ENDED");
                    try { 
                        gm.getBoard().getTile(nPos).onLanded(p, gm); 
                    } catch (const InsufficientFundsException& e) {
                        p.setStatus("LIQUIDATING_" + std::to_string(e.getRequired() - e.getAvailable()));
                    }
                    transitionAfterMove(gm, p);
                }
            } else if (cmd.find("CETAK_AKTA ") == 0) {
                std::string code = cmd.substr(11);
                int idx = gm.getBoard().getTileIndexByCode(code);
                if (idx != -1) {
                    PropertyTile* prop = dynamic_cast<PropertyTile*>(&gm.getBoard().getTile(idx));
                    if (prop) gui.openDeed(prop); 
                }
            } else if (cmd == "CETAK_PROPERTI") {
                gui.toggleAssetList(); 
            }
            gui.toggleCommandMode();
            cmdBuffer[0] = '\0';
            cmdLetterCount = 0;
            gui.setCommandText("");
        }
    }
}