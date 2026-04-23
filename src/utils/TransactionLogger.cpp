#include "utils/TransactionLogger.hpp"
#include <iostream>
#include <stdexcept>

void TransactionLogger::logAction(int turn, std::string username, std::string action, std::string detail){
    // Mengikuti format dari spek
    std::string entry = "[Turn " + std::to_string(turn) + "] " + username + " | " + action + " | " + detail;
    logEntries.push_back(entry);
}

void TransactionLogger::printLogs(int count) const {
    if (logEntries.empty()) {
        std::cout << "Belum ada log transaksi.\n";
        return;
    }

    if (count == -1){
        std::cout << "=== Log Transaksi Penuh ===\n";
        for (const std::string& entry : logEntries) {
            std::cout << entry << "\n";
        }
    } else {
        std::cout << "=== Log Transaksi (" << count << " Terakhir) ====\n";
        int start = (int)logEntries.size() - count;
        if (start < 0) start = 0;
        for (int i = start; i < (int)logEntries.size(); i++){
            std::cout << logEntries[i] << "\n";
        }
    }
}

const std::vector<std::string>& TransactionLogger::getLogs() const {
    return logEntries;
}
 
void TransactionLogger::clearLogs() {
    logEntries.clear();
}

void TransactionLogger::addRawLog(const std::string& entry){
    logEntries.push_back(entry);
}