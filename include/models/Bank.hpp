#pragma once

#include <bits/stdc++.h>
#include "core/GameManager.hpp"
class GameManager;
class Bank {
public:
    Bank();
    void collect(int amount,GameManager& gm);
    void dispense(int amount,GameManager& gm);
};
