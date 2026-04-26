#include "core/Board.hpp"
#include "models/Player.hpp"
#include "models/Tile.hpp"
#include "utils/Dice.hpp"
#include "models/ConfigReader.hpp"
#include "models/CardTile.hpp"
#include "models/FestivalTile.hpp"
#include "models/JailTile.hpp"
#include "models/GoTile.hpp"
#include "models/StreetTile.hpp"
#include "models/RailroadTile.hpp"
#include "models/GoToJailTile.hpp"
#include "models/UtilityTile.hpp"
#include "models/TaxTile.hpp"
#include "models/FreeParkingTile.hpp"

using namespace std;

Board::Board() {}

Board::~Board() {
    for (Tile* t : tiles) delete t;
}
void Board::initDynamicBoard(ConfigReader& config) {
    int goSalary = config.getSpecialData("GO_SALARY"); 
    if (goSalary == 0) goSalary = 200;
    int jailFine = config.getSpecialData("JAIL_FINE");
    if (jailFine == 0) jailFine = 50;

    map<int, string> layout = config.getBoardLayout();

    // 2. Loop instansiasi objek Polymorphic berdasarkan ID 1-40
    for (int i = 1; i <= 40; i++) {
        if (layout.find(i) == layout.end()) {
            cout << "[WARNING] Petak ID " << i << " tidak ditemukan di layout!\n";
            continue;
        }

        string code = layout[i];
        map<string, string> propData;
        
        try {
            propData = config.getPropertyData(code);
        } catch (...) {
            cout << "[WARNING] Petak ID " << i << " (" << code << ") tidak ditemukan di config!\n";
            continue;
        }

        string type = propData["TYPE"];
        string name = propData["NAME"];

        if (type == "SPESIAL") {
            if (code == "GO") tiles.push_back(new GoTile(i-1, code, name, goSalary));
            else if (code == "PEN") tiles.push_back(new JailTile(i-1, code, name, jailFine));
            else if (code == "BBP") tiles.push_back(new FreeParkingTile(i-1, code, name));
            else if (code == "PPJ") tiles.push_back(new GoToJailTile(i-1, code, name));
        }
        else if (type == "KARTU") {
            if (code == "DNU") tiles.push_back(new CardTile(i-1, code, name, "CHEST"));
            else if (code == "KSP") tiles.push_back(new CardTile(i-1, code, name, "CHANCE"));
        }
        else if (type == "FESTIVAL") {
            tiles.push_back(new FestivalTile(i-1, code, name));
        }
        else if (type == "PAJAK") {
            if (code == "PPH") {
                tiles.push_back(new TaxTile(i-1, code, name, "INCOME", config.getTaxData("INCOME_FLAT"), config.getTaxData("INCOME_PCT")));
            } else if (code == "PBM") {
                tiles.push_back(new TaxTile(i-1, code, name, "LUXURY", config.getTaxData("LUXURY_FLAT"), 0));
            }
        }
        else {
            cout << "[DEBUG] Membaca petak: " << code 
              << " | String PRICE: '" << propData["PRICE"] << "'"
              << " | String MORTGAGE: '" << propData["MORTGAGE"] << "'\n";
            
            int price = stoi(propData["PRICE"]);
            int mortgage = stoi(propData["MORTGAGE"]);

            if (type == "STREET") {
                string color = propData["COLOR"];
                int hPrice = stoi(propData["HOUSE_PRICE"]); 
                vector<int> rents;
                
                rents.push_back(stoi(propData["RENT_0"])); 
                rents.push_back(stoi(propData["RENT_1"])); 
                rents.push_back(stoi(propData["RENT_2"]));
                rents.push_back(stoi(propData["RENT_3"])); 
                rents.push_back(stoi(propData["RENT_4"]));
                rents.push_back(stoi(propData["RENT_5"])); 

                tiles.push_back(new StreetTile(i-1, code, name, price, mortgage, color, hPrice, rents));
            } 
            else if (type == "RAILROAD") {
                map<int, int> rrRents;
                rrRents[1] = config.getRailroadRent(1);
                rrRents[2] = config.getRailroadRent(2);
                rrRents[3] = config.getRailroadRent(3);
                rrRents[4] = config.getRailroadRent(4);

                tiles.push_back(new RailroadTile(i-1, code, name, price, mortgage, rrRents));
            } 
            else if (type == "UTILITY") {
                map<int, int> utilMults;
                utilMults[1] = config.getUtilityMultiplier(1);
                utilMults[2] = config.getUtilityMultiplier(2);

                tiles.push_back(new UtilityTile(i-1, code, name, price, mortgage, utilMults));
            }
        }
    }
    
    cout << "[SYSTEM] Papan berhasil diinisialisasi dengan " << tiles.size() << " petak.\n";
}
Tile& Board::getTile(int index) const { return *tiles[index]; }
int Board::getTileCount() const { return tiles.size(); }
int Board::getTileIndexByCode(string code) const {
    for (long unsigned int i=0;i<tiles.size();i++){
        if (code == tiles[i]->getCode())return i;
    }
    return -1;
}
