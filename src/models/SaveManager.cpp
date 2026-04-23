#include "models/SaveManager.hpp"
#include "core/GameManager.hpp"
#include <iostream>



SaveManager::SaveManager(std::string dir) : saveDirectory(dir) {}
 
// -------------------------------------------------------
// SAVE GAME STATE
// Format:
// <TURN_SAAT_INI> <MAX_TURN>
// <JUMLAH_PEMAIN>
// <STATE_PEMAIN_1..N>
// <URUTAN_GILIRAN>
// <GILIRAN_AKTIF>
// <STATE_PROPERTI>
// <STATE_DECK>
// <STATE_LOG>
// -------------------------------------------------------
bool SaveManager::saveGameState(const std::string& filename, const GameManager& gm) {
    std::string path = saveDirectory + "/" + filename;
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Gagal menyimpan file: " << path << "\n";
        return false;
    }
 
    // --- Baris 1: turn saat ini dan max turn ---
    file << gm.getCurrentTurnCount() << " " << gm.getMaxTurn() << "\n";
 
    // --- Baris 2: jumlah pemain ---
    const std::vector<Player*>& players = gm.getAllPlayersConst();
    file << players.size() << "\n";
 
    // --- State setiap pemain ---
    for (Player* p : players) {
        // USERNAME UANG POSISI STATUS
        std::string posCode = gm.getBoardConst()
                                .getTile(p->getPosition()).getCode();
        file << p->getUsername() << " "
             << p->getBalance()  << " "
             << posCode          << " "
             << p->getStatus()   << "\n";
 
        // Jumlah kartu di tangan
        const std::vector<SkillCard*>& hand = p->getHand();
        file << hand.size() << "\n";
 
        // Detail setiap kartu
        for (SkillCard* card : hand) {
            // JENIS_KARTU NILAI_KARTU SISA_DURASI
            // Tentukan jenis kartu via dynamic_cast
            std::string cardType = "UNKNOWN";
            std::string cardValue = "";
            std::string cardDuration = "";
 
            if (dynamic_cast<MoveCard*>(card)) {
                cardType  = "MoveCard";
                cardValue = std::to_string(card->getValue());
                // MoveCard tidak punya durasi
            } else if (dynamic_cast<DiscountCard*>(card)) {
                cardType     = "DiscountCard";
                cardValue    = std::to_string(card->getValue());
                cardDuration = std::to_string(card->getDuration());
            } else if (dynamic_cast<ShieldCard*>(card)) {
                cardType = "ShieldCard";
            } else if (dynamic_cast<TeleportCard*>(card)) {
                cardType = "TeleportCard";
            } else if (dynamic_cast<LassoCard*>(card)) {
                cardType = "LassoCard";
            } else if (dynamic_cast<DemolitionCard*>(card)) {
                cardType = "DemolitionCard";
            }
 
            file << cardType;
            if (!cardValue.empty())    file << " " << cardValue;
            if (!cardDuration.empty()) file << " " << cardDuration;
            file << "\n";
        }
    }
 
    // --- Urutan giliran ---
    for (int i = 0; i < (int)players.size(); i++) {
        file << players[i]->getUsername();
        if (i < (int)players.size() - 1) file << " ";
    }
    file << "\n";
 
    // --- Giliran aktif saat ini ---
    file << gm.getCurrentPlayerConst().getUsername() << "\n";
 
    // --- State properti ---
    // Kumpulkan semua PropertyTile dari board
    const Board& board = gm.getBoardConst();
    int tileCount = board.getTileCount();
 
    // Hitung dulu jumlah properti
    int propCount = 0;
    for (int i = 0; i < tileCount; i++) {
        if (dynamic_cast<const PropertyTile*>(&board.getTile(i))) {
            propCount++;
        }
    }
    file << propCount << "\n";
 
    for (int i = 0; i < tileCount; i++) {
        const PropertyTile* prop = dynamic_cast<const PropertyTile*>(&board.getTile(i));
        if (!prop) continue;
 
        // KODE JENIS PEMILIK STATUS FMULT FDUR N_BANGUNAN
        std::string jenis = "street";
        int fmult = 1, fdur = 0, nBangunan = 0;
        std::string nBangunanStr = "0";
 
        if (const StreetTile* st = dynamic_cast<const StreetTile*>(prop)) {
            jenis = "street";
            fmult = st->getFestivalMultiplier();
            fdur  = st->getFestivalDuration();
            int bc = st->getBuildingCount();
            // 0-4 = rumah, 5 = hotel (H)
            nBangunanStr = (bc == 5) ? "H" : std::to_string(bc);
        } else if (dynamic_cast<const RailroadTile*>(prop)) {
            jenis = "railroad";
        } else if (dynamic_cast<const UtilityTile*>(prop)) {
            jenis = "utility";
        }
 
        file << prop->getCode()           << " "
             << jenis                     << " "
             << prop->getOwner()          << " "
             << prop->getPropertyStatus() << " "
             << fmult                     << " "
             << fdur                      << " "
             << nBangunanStr              << "\n";
    }
 
    // --- State deck skill ---
    const CardDeck<SkillCard>& skillDeck = gm.getSkillDeckConst();
    const std::vector<SkillCard*>& deckCards = skillDeck.getDeckCards();
    file << deckCards.size() << "\n";
    for (SkillCard* card : deckCards) {
        if (dynamic_cast<MoveCard*>(card))        file << "MoveCard\n";
        else if (dynamic_cast<DiscountCard*>(card)) file << "DiscountCard\n";
        else if (dynamic_cast<ShieldCard*>(card))   file << "ShieldCard\n";
        else if (dynamic_cast<TeleportCard*>(card)) file << "TeleportCard\n";
        else if (dynamic_cast<LassoCard*>(card))    file << "LassoCard\n";
        else if (dynamic_cast<DemolitionCard*>(card)) file << "DemolitionCard\n";
    }
 
    // --- State log ---
    const std::vector<std::string>& logs = gm.getLogger().getLogs();
    file << logs.size() << "\n";
    for (const std::string& entry : logs) {
        file << entry << "\n";
    }
 
    file.close();
    std::cout << "Permainan berhasil disimpan ke: " << filename << "\n";
    return true;
}
 
// -------------------------------------------------------
// LOAD GAME STATE
// Membaca file save dan merestorasi seluruh state GameManager.
// -------------------------------------------------------
bool SaveManager::loadGameState(const std::string& filename, GameManager& gm) {
    std::string path = saveDirectory + "/" + filename;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File \"" << filename << "\" tidak ditemukan.\n";
        return false;
    }
 
    try {
        std::string line;
 
        // --- Baris 1: turn saat ini dan max turn ---
        int currentTurn, maxTurn;
        file >> currentTurn >> maxTurn;
        gm.setCurrentTurnCount(currentTurn);
        gm.setMaxTurn(maxTurn);
 
        // --- Baris 2: jumlah pemain ---
        int numPlayers;
        file >> numPlayers;
        file.ignore(); // buang newline
 
        // --- State setiap pemain ---
        std::vector<std::string> playerOrder; // untuk urutan giliran
        for (int i = 0; i < numPlayers; i++) {
            std::string username, posCode, status;
            int balance;
            file >> username >> balance >> posCode >> status;
            file.ignore();
 
            // Buat player baru dan restore state-nya
            Player* p = new HumanPlayer(username, balance);
            int posIndex = gm.getBoard().getTileIndexByCode(posCode);
            p->setPosition(posIndex);
            p->setStatus(status);
 
            // Baca kartu di tangan
            int handSize;
            file >> handSize;
            file.ignore();
 
            for (int j = 0; j < handSize; j++) {
                std::getline(file, line);
                std::istringstream iss(line);
                std::string cardType;
                iss >> cardType;
 
                SkillCard* card = nullptr;
                if (cardType == "MoveCard") {
                    int steps; iss >> steps;
                    card = new MoveCard(steps);
                } else if (cardType == "DiscountCard") {
                    int pct, dur; iss >> pct >> dur;
                    card = new DiscountCard(pct);
                    // restore durasi tersisa
                    for (int d = card->getDuration(); d > dur; d--)
                        card->decrementDuration();
                } else if (cardType == "ShieldCard") {
                    card = new ShieldCard();
                } else if (cardType == "TeleportCard") {
                    card = new TeleportCard();
                } else if (cardType == "LassoCard") {
                    card = new LassoCard();
                } else if (cardType == "DemolitionCard") {
                    card = new DemolitionCard();
                }
 
                if (card) p->addSkillCard(card);
            }
 
            gm.addPlayer(p);
            playerOrder.push_back(username);
        }
 
        // --- Urutan giliran ---
        std::getline(file, line);
        // (urutan sudah diambil dari playerOrder di atas)
 
        // --- Giliran aktif saat ini ---
        std::string activeUsername;
        file >> activeUsername;
        file.ignore();
        gm.setCurrentPlayerByUsername(activeUsername);
 
        // --- State properti ---
        int propCount;
        file >> propCount;
        file.ignore();
 
        for (int i = 0; i < propCount; i++) {
            std::string code, jenis, owner, status, nBangunanStr;
            int fmult, fdur;
            file >> code >> jenis >> owner >> status >> fmult >> fdur >> nBangunanStr;
            file.ignore();
 
            // Cari tile di board dan restore state-nya
            int tileIdx = gm.getBoard().getTileIndexByCode(code);
            if (tileIdx == -1) continue;
 
            PropertyTile* prop = dynamic_cast<PropertyTile*>(&gm.getBoard().getTile(tileIdx));
            if (!prop) continue;
 
            prop->setOwner(owner == "BANK" ? "" : owner);
            if (status == "MORTGAGED") prop->mortgageProperty();
 
            // Restore bangunan khusus StreetTile
            if (StreetTile* st = dynamic_cast<StreetTile*>(prop)) {
                st->applyFestival(fmult, fdur);
                int targetCount = (nBangunanStr == "H") ? 5 : std::stoi(nBangunanStr);
                for (int b = 0; b < targetCount; b++) {
                    st->buildHouse();
                }
            }
 
            // Restore kepemilikan di Player
            if (owner != "BANK" && !owner.empty()) {
                for (Player* p : gm.getAllPlayers()) {
                    if (p->getUsername() == owner) {
                        p->addProperty(prop);
                        break;
                    }
                }
            }
        }
 
        // --- State deck skill ---
        int deckSize;
        file >> deckSize;
        file.ignore();
 
        for (int i = 0; i < deckSize; i++) {
            std::getline(file, line);
            SkillCard* card = nullptr;
            if (line == "MoveCard") {
                // Nilai langkah diacak ulang karena tidak disimpan di deck
                std::mt19937 rng(std::random_device{}());
                int steps = std::uniform_int_distribution<>(1, 6)(rng);
                card = new MoveCard(steps);
            } else if (line == "DiscountCard") {
                std::mt19937 rng(std::random_device{}());
                int pct = std::uniform_int_distribution<>(10, 50)(rng);
                card = new DiscountCard(pct);
            } else if (line == "ShieldCard")     card = new ShieldCard();
            else if (line == "TeleportCard")     card = new TeleportCard();
            else if (line == "LassoCard")        card = new LassoCard();
            else if (line == "DemolitionCard")   card = new DemolitionCard();
 
            if (card) gm.getSkillDeck().addCard(card);
        }
 
        // --- State log ---
        int logCount;
        file >> logCount;
        file.ignore();
 
        gm.getLogger().clearLogs();
        for (int i = 0; i < logCount; i++) {
            std::getline(file, line);
            // Log disimpan langsung sebagai string ke vector
            gm.getLogger().addRawLog(line);
        }
 
        file.close();
        std::cout << "Permainan berhasil dimuat. Melanjutkan giliran "
                  << activeUsername << "...\n";
        return true;
 
    } catch (const std::exception& e) {
        std::cerr << "Gagal memuat file! File rusak atau format tidak dikenali.\n";
        std::cerr << "Detail: " << e.what() << "\n";
        file.close();
        return false;
    }
}