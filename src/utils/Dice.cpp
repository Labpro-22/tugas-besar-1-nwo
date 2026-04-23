#include <iostream>
#include "utils/Dice.hpp"

Dice::Dice() : die1(0), die2(0), consecutiveDoubles(0) { srand(time(NULL)); }
void Dice::rollRandom() {
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    if (die1 == die2) consecutiveDoubles++;
    else consecutiveDoubles = 0;
}
void Dice::setManual(int d1, int d2) {
    die1 = d1; die2 = d2;
    if (die1 == die2) consecutiveDoubles++;
    else consecutiveDoubles = 0;
}
int Dice::getTotal() const { return die1 + die2; }
bool Dice::isDouble() const { return die1 == die2; }
int Dice::getConsecutiveDoubles() const { return consecutiveDoubles; }
void Dice::resetDoubles() { consecutiveDoubles = 0; }

int Dice::getDie1() const { return die1;}
int Dice::getDie2() const { return die2;}