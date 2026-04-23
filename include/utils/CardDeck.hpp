#pragma once

#include <bits/stdc++.h>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <iostream>
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
    const std::vector<T*>& getDeckCards() const { return deck; }
};
//Implementasi dari kelas CardDeck

template <typename T>
CardDeck<T>::CardDeck(){
    //Melakukan insialisasi konstruktor
}

template <typename T>
CardDeck<T>::~CardDeck(){
    for (T* card : deck){
        delete card;
    }
    deck.clear();

    for(T* card : discardPile){
        delete card;
    }
    discardPile.clear();
}

template <typename T>
void CardDeck<T>::addCard(T* card){
    deck.push_back(card);
}

template <typename T>
T* CardDeck<T>::drawCard(){
    if (deck.empty()){
        if (discardPile.empty()){
            throw std::runtime_error("Deck dan discard pile kosong, tidak ada kartu yang bisa diambil");
        }
        deck = discardPile;
        discardPile.clear();
        shuffle();
    }

    T* card = deck.front();
    deck.erase(deck.begin());
    return card;
}

template <typename T>
void CardDeck<T>::discardCard(T* Card){
    discardPile.push_back(card);
}

template <typename T>
void CardDeck<T>::shuffle(){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(deck.begin(), deck.end(), rng);
}

template <typename T>
bool CardDeck<T>::isEmpty() const{
    return deck.empty() && discardPile.empty();
}



