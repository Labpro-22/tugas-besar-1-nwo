#pragma once

#include <bits/stdc++.h>

class TransactionLogger {
private:
    std::vector<std::string> logEntries;
public:
    void logAction(int turn, std::string username, std::string action, std::string detail);
    void printLogs(int count = -1) const;
    const std::vector<std::string>& getLogs() const;
    void clearLogs();
    void addRawLog(const std::string& entry);
};