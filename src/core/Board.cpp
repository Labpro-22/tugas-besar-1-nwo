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

using namespace std;

Board::Board() {}

Board::~Board() {
    for (Tile* t : tiles) delete t;
}
void Board::initDynamicBoard(ConfigReader& config) {
    // 1. Definisikan urutan papan (Index 0 - 39)
    // Isi string ini dengan Kode Properti (GRT, TSK) atau Kode Spesial
    string layout[40] = {
        "GO", "CHANCE", "GRT", "CHEST", "TSK",        // 0-4
        "GBR", "BGR", "CHANCE", "DPK", "BKS",         // 5-9
        "JAIL", "MGL", "PLN", "SOL", "YOG",           // 10-14
        "STB", "MAL", "CHEST", "SMG", "SBY",          // 15-19
        "FESTIVAL", "MKS", "CHANCE", "BLP", "MND",    // 20-24
        "TUG", "PLB", "PAM", "PKB", "MED",            // 25-29
        "GO_TO_JAIL", "BDG", "DEN", "CHEST", "MTR",   // 30-34
        "GUB", "CHANCE", "JKT", "TAX", "IKN"          // 35-39
    };

    // Ambil data gaji GO dari special.txt (Atau hardcode 200 kalau ga ada)
    int goSalary = config.getSpecialData("GO_SALARY"); 
    if (goSalary == 0) goSalary = 200;

    // 2. Loop instansiasi objek Polymorphic
    for (int i = 0; i < 40; i++) {
        string code = layout[i];

        // --- PENANGANAN PETAK SPESIAL ---
        if (code == "GO") {
            tiles.push_back(new GoTile(i, code, "START", goSalary));
        } 
        else if (code == "CHANCE") {
            tiles.push_back(new CardTile(i, code, "Kesempatan", "CHANCE"));
        } 
        else if (code == "CHEST") {
            tiles.push_back(new CardTile(i, code, "Dana Umum", "CHEST"));
        } 
        else if (code == "FESTIVAL") {
            tiles.push_back(new FestivalTile(i, code, "Festival"));
        } 
        else if (code == "JAIL") {
            tiles.push_back(new JailTile(i, code, "Jail", 50)); 
        } 
        else if (code == "GO_TO_JAIL") {
            tiles.push_back(new GoToJailTile(i, code, "Go To Jail")); 
        }
        else if (code == "TAX") {
            
            int taxAmount = config.getSpecialData("TAX_AMOUNT"); 
            
        } 
        else {
            map<string, string> propData = config.getPropertyData(code);
            
            if (!propData.empty()) {
               string type = propData["TYPE"];
                string name = propData["NAME"];
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

                    tiles.push_back(new StreetTile(i, code, name, price, mortgage, color, hPrice, rents));
                } 

                else if (type == "RAILROAD") {
                    map<int, int> rrRents;
                    
                    rrRents[1] = config.getRailroadRent(1);
                    rrRents[2] = config.getRailroadRent(2);
                    rrRents[3] = config.getRailroadRent(3);
                    rrRents[4] = config.getRailroadRent(4);

                    tiles.push_back(new RailroadTile(i, code, name, price, mortgage, rrRents));
                } 
                else if (type == "UTILITY") {
                    map<int, int> utilMults;
                    utilMults[1] = config.getUtilityMultiplier(1);
                    utilMults[2] = config.getUtilityMultiplier(2);

                    tiles.push_back(new UtilityTile(i, code, name, price, mortgage, utilMults));
                }
            } else {
                cout << "[WARNING] Petak indeks " << i << " (" << code << ") tidak ditemukan di config!\n";
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
