#include "models/SaveManager.hpp"
#include "core/GameManager.hpp"
#include "models/Player.hpp"
#include "utils/FileException.hpp"
#include "models/MoveCard.hpp"
#include "models/DiscountCard.hpp"
#include "models/ShieldCard.hpp"
#include "models/TeleportCard.hpp"
#include "models/LassoCard.hpp"
#include "models/DemolitionCard.hpp"
#include "core/Board.hpp"
#include "models/Tile.hpp"
#include "models/PropertyTile.hpp"
#include "models/RailroadTile.hpp"
#include "models/StreetTile.hpp"

#include <bits/stdc++.h>
using namespace std;

string SaveManager::openLoadFileDialog() {
    char filename[1024];
    FILE *f = popen("zenity --file-selection --title=\"Pilih File Save Nimonspoli\" --file-filter=\"*.txt\"", "r");
    if (!f) return ""; 
    
    if (fgets(filename, sizeof(filename), f) != nullptr) {
        std::string result(filename);
        if (!result.empty() && result.back() == '\n') result.pop_back();
       
        pclose(f);
        return result;
    }
    pclose(f);
    return ""; 
}

string SaveManager::openSaveFileDialog() {
    char filename[1024] = {0};
    // FAKTA: Hapus --confirm-overwrite karena sudah deprecated di Ubuntu baru
    FILE *f = popen("zenity --file-selection --save --title=\"Simpan Game Nimonspoli\" --file-filter=\"*.txt\"", "r");
    
    if (!f) return ""; 
    
    if (fgets(filename, sizeof(filename), f) != nullptr) {
        std::string result(filename);
        
        // Buang karakter newline (\n atau \r) di akhir string
        result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());

        // Tambah .txt jika belum ada
        if (result.length() < 4 || result.substr(result.length() - 4) != ".txt") {
            result += ".txt";
        }

        pclose(f);
        return result; // Mengembalikan path lengkap (misal: /home/user/save.txt)
    }
    
    pclose(f);
    return ""; 
}


SaveManager::SaveManager(string dir) : saveDirectory(dir) {}
bool SaveManager::saveGameState(const string& filename, const GameManager& gm) {
    string targetFile = filename.empty() ? openSaveFileDialog() : filename;
    if (targetFile.empty()) return false; 

    // FAKTA: Smart Path Detection agar file mendarat di folder yang benar
    string fullPath = (targetFile[0] != '/') ? (saveDirectory + "/" + targetFile) : targetFile;

    GameManager& mut_gm = const_cast<GameManager&>(gm); 
    ofstream out(fullPath, ios::out | ios::trunc);
    if (!out.is_open()) throw FileException("Gagal menulis file", fullPath);

    // 1. <TURN_SAAT_INI> <MAX_TURN>
    out << mut_gm.getCurrentTurnCount() << " " << mut_gm.getMaxTurn() << "\n";

    // 2. <JUMLAH_PEMAIN>
    auto& players = mut_gm.getAllPlayers();
    out << players.size() << "\n";

    // 3. <STATE_PEMAIN>
    for (Player* p : players) {
        string posCode = mut_gm.getBoard().getTile(p->getPosition()).getCode();
        out << p->getUsername() << " " << p->getBalance() << " " << posCode << " " << p->getStatus() << "\n";
        
        auto& hand = p->getHand();
        out << hand.size() << "\n";
        for (SkillCard* card : hand) {
            // FORMAT: <JENIS> <NILAI (Optional)> <DURASI (Optional)>
            if (auto* m = dynamic_cast<MoveCard*>(card)) 
                out << "MoveCard " << m->getValue() << "\n";
            else if (auto* d = dynamic_cast<DiscountCard*>(card)) 
                out << "DiscountCard " << d->getValue() << " " << d->getDuration() << "\n";
            else if (dynamic_cast<ShieldCard*>(card)) out << "ShieldCard\n";
            else if (dynamic_cast<TeleportCard*>(card)) out << "TeleportCard\n";
            else if (dynamic_cast<LassoCard*>(card)) out << "LassoCard\n";
            else if (dynamic_cast<DemolitionCard*>(card)) out << "DemolitionCard\n";
        }
    }

    // 4. <URUTAN_GILIRAN>
    for (size_t i = 0; i < players.size(); ++i) {
        out << players[i]->getUsername() << (i == players.size() - 1 ? "" : " ");
    }
    out << "\n";

    // 5. <GILIRAN_AKTIF_SAAT_INI>
    out << mut_gm.getCurrentPlayer().getUsername() << "\n";

    // 6. <STATE_PROPERTI>
    Board& board = mut_gm.getBoard();
    vector<PropertyTile*> allProps;
    for (int i = 0; i < board.getTileCount(); i++) {
        if (auto* p = dynamic_cast<PropertyTile*>(&board.getTile(i))) allProps.push_back(p);
    }
    
    out << allProps.size() << "\n";
    for (PropertyTile* prop : allProps) {
        string type = "utility";
        int fMult = 1, fDur = 0;
        string nStr = "0";

        if (auto* s = dynamic_cast<StreetTile*>(prop)) {
            type = "street";
            fMult = 1; // Default 1 jika tidak festival
            fDur = 0;  
            int count = s->getBuildingCount();
            nStr = (count >= 5) ? "H" : to_string(count); // FAKTA: 5 Rumah = Hotel 'H'
        } else if (dynamic_cast<RailroadTile*>(prop)) type = "railroad";

        out << prop->getCode() << " " << type << " " << prop->getOwner() << " " 
            << prop->getPropertyStatus() << " " << fMult << " " << fDur << " " << nStr << "\n";
    }

    // 7. <STATE_DECK>
    out << "0\n"; 

    // 8. <STATE_LOG>
    const auto& logs = mut_gm.getLogger().getLogs();
    out << logs.size() << "\n";
    for (const string& log : logs) out << log << "\n";

    out.flush();
    out.close();
    return true;
}

bool SaveManager::loadGameState(const string& filename, GameManager& gm) {
    string targetFile = filename.empty() ? openLoadFileDialog() : filename;
    if (targetFile.empty()) return false;

    // FAKTA: Tambahin Smart Path di load juga biar gak Gagal Baca
    string fullPath = (targetFile[0] != '/') ? (saveDirectory + "/" + targetFile) : targetFile;

    ifstream in(fullPath);
    if (!in.is_open()) throw FileException("Gagal membaca save data", fullPath);

    int turn, maxT;
    in >> turn >> maxT;

    int numPlayers;
    in >> numPlayers;

    struct TempPlayer {
        string name, posCode, status;
        int bal;
        vector<SkillCard*> cards;
    };
    vector<TempPlayer> tempPlayers;
    vector<string> humanNames;
    bool hasCom = false;

    for (int i = 0; i < numPlayers; i++) {
        TempPlayer tp;
        int numCards;
        in >> tp.name >> tp.bal >> tp.posCode >> tp.status;
        if (tp.name == "COM") hasCom = true; else humanNames.push_back(tp.name);

        in >> numCards;
        string line;
        getline(in, line); // Buang sisa newline
        for(int j = 0; j < numCards; j++) {
            getline(in, line);
            stringstream ss(line);
            string type; ss >> type;
            if (type == "MoveCard") { int v; ss >> v; tp.cards.push_back(new MoveCard(v)); }
            else if (type == "DiscountCard") { int v, d; ss >> v >> d; tp.cards.push_back(new DiscountCard(v)); }
            else if (type == "ShieldCard") tp.cards.push_back(new ShieldCard());
            else if (type == "TeleportCard") tp.cards.push_back(new TeleportCard());
            else if (type == "LassoCard") tp.cards.push_back(new LassoCard());
            else if (type == "DemolitionCard") tp.cards.push_back(new DemolitionCard());
        }
        tempPlayers.push_back(tp);
    }

    if (gm.getAllPlayers().empty()) gm.initializeGame(humanNames.size(), humanNames, hasCom);
    gm.setTurnData(turn, 0);

    auto& players = gm.getAllPlayers();
    for (int i = 0; i < (int)tempPlayers.size(); i++) {
        *players[i] -= players[i]->getBalance();
        *players[i] += tempPlayers[i].bal;
        int posIdx = gm.getBoard().getTileIndexByCode(tempPlayers[i].posCode);
        players[i]->setPosition(posIdx != -1 ? posIdx : 0); 
        players[i]->setStatus(tempPlayers[i].status);
        for (SkillCard* c : tempPlayers[i].cards) {
            try { players[i]->addSkillCard(c); } catch (...) { delete c; }
        }
    }

    in >> ws; 
    string urutan, giliranAktif;
    getline(in, urutan);
    getline(in, giliranAktif);

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i]->getUsername() == giliranAktif) {
            gm.setTurnData(turn, i);
            break;
        }
    }

    int numProps;
    in >> numProps;
    Board& board = gm.getBoard();
    for (int i = 0; i < numProps; i++) {
        string code, type, owner, status, nStr;
        int fMult, fDur;
        in >> code >> type >> owner >> status >> fMult >> fDur >> nStr;

        int idx = board.getTileIndexByCode(code);
        if (idx != -1) {
            PropertyTile* prop = dynamic_cast<PropertyTile*>(&board.getTile(idx));
            if (prop && owner != "BANK") {
                prop->setOwner(owner);
                if (status == "MORTGAGED") prop->mortgageProperty();
                
                if (auto* street = dynamic_cast<StreetTile*>(prop)) {
                    int bCount = (nStr == "H") ? 5 : stoi(nStr); // FAKTA: Konversi 'H' jadi 5
                    for(int b=0; b<bCount; b++) street->buildHouse();
                }
                for (Player* p : players) {
                    if (p->getUsername() == owner) { p->addProperty(prop); break; }
                }
            }
        }
    }
    in.close();
    return true;
}