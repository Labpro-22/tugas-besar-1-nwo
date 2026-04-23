#pragma once

#include <bits/stdc++.h>

class Dice {
private:
    int die1;
    int die2;
    int consecutiveDoubles;
public:
    Dice();
    void rollRandom();
    void setManual(int d1, int d2);
    int getTotal() const;
    bool isDouble() const;
    int getConsecutiveDoubles() const;
    void resetDoubles();
    int getDie1() const;
    int getDie2() const;
};