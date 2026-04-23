#include "utils/TransactionLogger.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

void TransactionLogger::logAction(int turn, string username, string action, string detail) {
    string log = "[Turn " + to_string(turn) + "] " + username + " | " + action + " | " + detail;
    logEntries.push_back(log);
}
const vector<string>& TransactionLogger::getLogs() const { 
    return logEntries; 
}

void TransactionLogger::clearLogs() { 
    logEntries.clear(); 
}

void TransactionLogger::printLogs(int count) const {
    // Kalau -1 atau kelebihan, print semua. Kalau ada angka, print sebanyak angka itu (dari bawah).
    int limit = (count == -1 || count > logEntries.size()) ? logEntries.size() : count;
    int start = logEntries.size() - limit;
    
    std::cout << "\n--- SYSTEM LOGS ---\n";
    for (int i = start; i < logEntries.size(); i++) {
        std::cout << logEntries[i] << "\n";
    }
    std::cout << "-------------------\n";
}