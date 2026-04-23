#include "models/ConfigReader.hpp"
#include "core/GameManager.hpp"
#include "utils/TransactionLogger.hpp"
#include <iostream>

ConfigReader::ConfigReader(std::string dir)
    : configDirectory(dir), maxTurn(0), initialBalance(0) {}
 
// loadAllConfigs: membaca semua file konfigurasi sekaligus.
// Dipanggil sekali di awal program sebelum game dimulai.
void ConfigReader::loadAllConfigs() {
    loadPropertyConfig();
    loadRailroadConfig();
    loadUtilityConfig();
    loadTaxConfig();
    loadSpecialConfig();
    loadMiscConfig();
}
 
// -------------------------------------------------------
// property.txt
// Format per baris:
// ID KODE NAMA JENIS WARNA HARGA_LAHAN NILAI_GADAI
//   UPG_RUMAH UPG_HT RENT_L0 RENT_L1 RENT_L2 RENT_L3 RENT_L4 RENT_L5
// -------------------------------------------------------
void ConfigReader::loadPropertyConfig() {
    std::string path = configDirectory + "/property.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // skip komentar/baris kosong
 
        std::istringstream iss(line);
        std::map<std::string, std::string> data;
 
        std::string id, code, name, type, color;
        int hargaLahan, nilaiGadai, upgRumah, upgHotel;
        int r0, r1, r2, r3, r4, r5;
 
        iss >> id >> code >> name >> type >> color
            >> hargaLahan >> nilaiGadai;
        
        // Masih harus diperbaiki
        data["ID"]           = id;
        data["KODE"]         = code;
        data["NAMA"]         = name;
        data["JENIS"]        = type;
        data["WARNA"]        = color;
        data["HARGA_LAHAN"]  = std::to_string(hargaLahan);
        data["NILAI_GADAI"]  = std::to_string(nilaiGadai);
 
        // Railroad dan Utility tidak punya kolom upgrade/rent table
        if (type == "STREET") {
            iss >> upgRumah >> upgHotel
                >> r0 >> r1 >> r2 >> r3 >> r4 >> r5;
 
            data["UPG_RUMAH"] = std::to_string(upgRumah);
            data["UPG_HT"]    = std::to_string(upgHotel);
            data["RENT_L0"]   = std::to_string(r0);
            data["RENT_L1"]   = std::to_string(r1);
            data["RENT_L2"]   = std::to_string(r2);
            data["RENT_L3"]   = std::to_string(r3);
            data["RENT_L4"]   = std::to_string(r4);
            data["RENT_L5"]   = std::to_string(r5);
        }
 
        propertyConfig[code] = data;
    }
    file.close();
}
 
// -------------------------------------------------------
// railroad.txt
// Format: JUMLAH_RAILROAD BIAYA_SEWA
// -------------------------------------------------------
void ConfigReader::loadRailroadConfig() {
    std::string path = configDirectory + "/railroad.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (firstLine) { firstLine = false; continue; } // skip header
 
        std::istringstream iss(line);
        int count, rent;
        iss >> count >> rent;
        railroadConfig[count] = rent;
    }
    file.close();
}
 
// -------------------------------------------------------
// utility.txt
// Format: JUMLAH_UTILITY FAKTOR_PENGALI
// -------------------------------------------------------
void ConfigReader::loadUtilityConfig() {
    std::string path = configDirectory + "/utility.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (firstLine) { firstLine = false; continue; } // skip header
 
        std::istringstream iss(line);
        int count, multiplier;
        iss >> count >> multiplier;
        utilityConfig[count] = multiplier;
    }
    file.close();
}
 
// -------------------------------------------------------
// tax.txt
// Format: PPH_FLAT PPH_PERSENTASE PBM_FLAT
// -------------------------------------------------------
void ConfigReader::loadTaxConfig() {
    std::string path = configDirectory + "/tax.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (firstLine) { firstLine = false; continue; } // skip header
 
        std::istringstream iss(line);
        int pphFlat, pphPct, pbmFlat;
        iss >> pphFlat >> pphPct >> pbmFlat;
 
        taxConfig["PPH_FLAT"]        = pphFlat;
        taxConfig["PPH_PERSENTASE"]  = pphPct;
        taxConfig["PBM_FLAT"]        = pbmFlat;
        break; // hanya satu baris data
    }
    file.close();
}
 
// -------------------------------------------------------
// special.txt
// Format: GO_SALARY JAIL_FINE
// -------------------------------------------------------
void ConfigReader::loadSpecialConfig() {
    std::string path = configDirectory + "/special.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (firstLine) { firstLine = false; continue; } // skip header
 
        std::istringstream iss(line);
        int salary, fine;
        iss >> salary >> fine;
 
        specialConfig["GO_SALARY"] = salary;
        specialConfig["JAIL_FINE"] = fine;
        break;
    }
    file.close();
}
 
// -------------------------------------------------------
// misc.txt
// Format: MAX_TURN SALDO_AWAL
// -------------------------------------------------------
void ConfigReader::loadMiscConfig() {
    std::string path = configDirectory + "/misc.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + path);
    }
 
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (firstLine) { firstLine = false; continue; } // skip header
 
        std::istringstream iss(line);
        iss >> maxTurn >> initialBalance;
        break;
    }
    file.close();
}
 
// -------------------------------------------------------
// Getters ConfigReader
// -------------------------------------------------------
std::map<std::string, std::string> ConfigReader::getPropertyData(std::string code) const {
    auto it = propertyConfig.find(code);
    if (it == propertyConfig.end()) {
        throw std::runtime_error("Properti dengan kode '" + code + "' tidak ditemukan.");
    }
    return it->second;
}
 
int ConfigReader::getRailroadRent(int count) const {
    auto it = railroadConfig.find(count);
    if (it == railroadConfig.end()) return 0;
    return it->second;
}
 
int ConfigReader::getUtilityMultiplier(int count) const {
    auto it = utilityConfig.find(count);
    if (it == utilityConfig.end()) return 0;
    return it->second;
}
 
int ConfigReader::getTaxData(std::string type) const {
    auto it = taxConfig.find(type);
    if (it == taxConfig.end()) {
        throw std::runtime_error("Data pajak '" + type + "' tidak ditemukan.");
    }
    return it->second;
}
 
int ConfigReader::getSpecialData(std::string type) const {
    auto it = specialConfig.find(type);
    if (it == specialConfig.end()) {
        throw std::runtime_error("Data spesial '" + type + "' tidak ditemukan.");
    }
    return it->second;
}
 
int ConfigReader::getMaxTurn() const {
    return maxTurn;
}
 
int ConfigReader::getInitialBalance() const {
    return initialBalance;
}