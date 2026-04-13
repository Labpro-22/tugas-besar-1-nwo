#pragma once

#include <bits/stdc++.h>

template <typename T>
class CardDeck {
private:
    std::vector<T*> deck;
    std::vector<T*> discardPile;
public:
    CardDeck();
    ~CardDeck();
    void addCard(T* card);
    T* drawCard();
    void discardCard(T* card);
    void shuffle();
    bool isEmpty() const;
};
