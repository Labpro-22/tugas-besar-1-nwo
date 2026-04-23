#include "models/ConfigReader.hpp"
#include "core/GameManager.hpp"
#include "utils/TransactionLogger.hpp"
#include "utils/InvalidPropertyException.hpp"
#include <bits/stdc++.h>
using namespace std;
void ConfigReader::loadAllConfigs() {
    // 1. Parsing property.txt -> Masuk ke propertyConfig
    ifstream propFile(configDirectory + "/property.txt");
    string line;
    while (getline(propFile, line)) {
        if (line.empty()) continue; // Skip baris kosong biar ga error

        stringstream ss(line);
        string id, code, name, type;
        
        // 4 Kolom pertama selalu sama untuk semua jenis properti
        ss >> id >> code >> name >> type;
        
        propertyConfig[code]["TYPE"] = type;
        propertyConfig[code]["NAME"] = name;

        // Percabangan berdasarkan TYPE karena jumlah kolom STREET beda dengan yang lain
        if (type == "STREET") {
            string color, price, mortgage, hPrice;
            // Baca sisa kolom untuk Street
            ss >> color >> price >> mortgage >> hPrice;
            
            propertyConfig[code]["COLOR"] = color;
            propertyConfig[code]["PRICE"] = price;
            propertyConfig[code]["MORTGAGE"] = mortgage;
            propertyConfig[code]["HOUSE_PRICE"] = hPrice;
            for (int j = 0; j <= 5; j++) {
                string tempValue;
                if (ss >> tempValue) {
                    // Cek kalau ada titik di dalam string-nya
                    if (tempValue.find(".") != string::npos || tempValue == "…") {
                        propertyConfig[code]["RENT_" + to_string(j)] = "0"; 
                    } else {
                        propertyConfig[code]["RENT_" + to_string(j)] = tempValue;
                    }
                } else {
                    propertyConfig[code]["RENT_" + to_string(j)] = "0";
                }
            }
            // propertyConfig[code]["RENT_0"] = r0;
            // propertyConfig[code]["RENT_1"] = r1;
            // propertyConfig[code]["RENT_2"] = r2;
            // propertyConfig[code]["RENT_3"] = r3;
            // propertyConfig[code]["RENT_4"] = r4;
            // propertyConfig[code]["RENT_5"] = r5;
        } 
        else if (type == "RAILROAD" || type == "UTILITY") {
            string color, price, mortgage;
            // Baca sisa kolom untuk Railroad/Utility (biasanya cuma warna, harga, dan gadai)
            ss >> color >> price >> mortgage; 
            
            propertyConfig[code]["COLOR"] = color;
            propertyConfig[code]["PRICE"] = price;
            propertyConfig[code]["MORTGAGE"] = mortgage;
        }
        // Logika split string per baris
        // map["GRT"]["PRICE"] = "60"; dst.
    }

    // 2. Parsing railroad.txt -> Masuk ke railroadConfig
    ifstream rrFile(configDirectory + "/railroad.txt");
    int count, rent;
    while (rrFile >> count >> rent) {
        railroadConfig[count] = rent;
    }

    ifstream utilFile(configDirectory + "/utility.txt");
    int uCount, uMult;
    while (utilFile >> uCount >> uMult) {
        utilityConfig[uCount] = uMult;
    }

    // 3. Parsing special.txt -> Masuk ke map specialConfig (Isi: GO_SALARY, JAIL_FINE, dll)
    ifstream specFile(configDirectory + "/special.txt");
    string specKey;
    int specValue;
    while (specFile >> specKey >> specValue) {
        specialConfig[specKey] = specValue;
    }

    // 4. Parsing misc.txt -> Tarik variabel penting (Isi: MAX_TURN, SALDO_AWAL, dll)
    ifstream miscFile(configDirectory + "/misc.txt");
    if (miscFile.is_open()) {
        // Langsung tembak masukin ke variabel
        miscFile >> this->maxTurn >> this->initialBalance;
        miscFile.close();
    } else {
        cout << "[WARNING] File misc.txt tidak ditemukan!\n";
    }
}

map<string, string> ConfigReader::getPropertyData(string code) const {
    if (propertyConfig.find(code) != propertyConfig.end()) {
        return propertyConfig.at(code);
    }
    throw InvalidPropertyException(code);
}

int ConfigReader::getRailroadRent(int count) const { return railroadConfig.at(count); }
int ConfigReader::getUtilityMultiplier(int count) const { return utilityConfig.at(count); }
int ConfigReader::getTaxData(string type) const { return taxConfig.at(type); }
int ConfigReader::getMaxTurn() const { return maxTurn; }
int ConfigReader::getInitialBalance() const { return initialBalance; }
int ConfigReader::getSpecialData(string type) const {
    // Cari key di dalam map specialConfig
    auto it = specialConfig.find(type);
    
    // Kalau ketemu, return nilainya
    if (it != specialConfig.end()) {
        return it->second;
    }
    
    // Kalau nggak ketemu, return 0 (atau lu bisa throw exception kalo mau ketat)
    // cout << "[WARNING] Data spesial '" << type << "' tidak ditemukan di config!\n";
    return 0; 
}