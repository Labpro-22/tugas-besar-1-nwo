#pragma once

#include <bits/stdc++.h>

class ConfigReader {
private:
    std::string configDirectory;
    std::map<std::string, std::map<std::string, std::string>> propertyConfig;
    std::map<int, int> railroadConfig;
    std::map<int, int> utilityConfig;
    std::map<std::string, int> taxConfig;
    std::map<std::string, int> specialConfig;
    int maxTurn;
    int initialBalance;

    void loadPropertyConfig();
    void loadRailroadConfig();
    void loadUtilityConfig();
    void loadTaxConfig();
    void loadSpecialConfig();
    void loadMiscConfig();

public:
    ConfigReader(std::string dir);
    void loadAllConfigs();
    std::map<std::string, std::string> getPropertyData(std::string code) const;
    int getRailroadRent(int count) const;
    int getUtilityMultiplier(int count) const;
    int getTaxData(std::string type) const;
    int getSpecialData(std::string type) const;
    int getMaxTurn() const;
    int getInitialBalance() const;
};