#include "models/Board.hpp"

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
        else if (code == "JAIL" || code == "GO_TO_JAIL") {
            tiles.push_back(new JailTile(i,code,"Jail",2));
            // tiles.push_back(new Tile(i, code, "Penjara", "JAIL")); 
        } 
        else if (code == "TAX") {
            // Asumsi ambil data pajak dari specialConfig
            int taxAmount = config.getSpecialData("TAX_AMOUNT"); 
            // tiles.push_back(new TaxTile(i, code, "Pajak", taxAmount));
        } 
        // --- PENANGANAN PETAK PROPERTI ---
        else {
            // Ambil data properti dari map ConfigReader
            map<string, string> propData = config.getPropertyData(code);
            
            if (!propData.empty()) {
               string type = propData["TYPE"];
                string name = propData["NAME"];
                cout << "[DEBUG] Membaca petak: " << code 
              << " | String PRICE: '" << propData["PRICE"] << "'"
              << " | String MORTGAGE: '" << propData["MORTGAGE"] << "'\n";
                int price = stoi(propData["PRICE"]);
                int mortgage = stoi(propData["MORTGAGE"]); // Atau price / 2 kalau ga ada di txt

                if (type == "STREET") {
                    string color = propData["COLOR"];
                    
                    // Asumsi lu nyimpen harga rumah dengan key "HOUSE_PRICE" di map ConfigReader
                    int hPrice = stoi(propData["HOUSE_PRICE"]); 
                    
                    // Tarik data sewa ke dalam vector. 
                    // Sesuaikan nama key ("RENT_0", dll) dengan logika parsing lu di loadAllConfigs
                    vector<int> rents;
                    
                    rents.push_back(stoi(propData["RENT_0"])); // Sewa tanah kosong
                    rents.push_back(stoi(propData["RENT_1"])); // Sewa 1 rumah
                    rents.push_back(stoi(propData["RENT_2"])); // Sewa 2 rumah
                    rents.push_back(stoi(propData["RENT_3"])); // Sewa 3 rumah
                    rents.push_back(stoi(propData["RENT_4"])); // Sewa 4 rumah
                    rents.push_back(stoi(propData["RENT_5"])); // Sewa hotel

                    // Instansiasi sesuai urutan parameter lo: 
                    // (idx, code, name, price, mortgage, color, hPrice, rents)
                    tiles.push_back(new StreetTile(i, code, name, price, mortgage, color, hPrice, rents));
                } 

                else if (type == "RAILROAD") {
                    map<int, int> rrRents;
                    // Monopoli standar punya 4 stasiun, jadi kita tarik 4 data
                    rrRents[1] = config.getRailroadRent(1);
                    rrRents[2] = config.getRailroadRent(2);
                    rrRents[3] = config.getRailroadRent(3);
                    rrRents[4] = config.getRailroadRent(4);

                    tiles.push_back(new RailroadTile(i, code, name, price, mortgage, rrRents));
                } 
                else if (type == "UTILITY") {
                    map<int, int> utilMults;
                    // Monopoli standar punya 2 utility (PLN & PAM), tarik 2 data
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
    for (auto i=0;i<tiles.size();i++){
        if (code == tiles[i]->getCode())return i;
    }
    return -1;
}
