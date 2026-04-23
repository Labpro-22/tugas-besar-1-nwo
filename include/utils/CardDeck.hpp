#pragma once

// FAKTA: Buang <bits/stdc++.h> jauh-jauh! Pake yang beneran dibutuhin aja.
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

template <typename T>
class CardDeck {
private:
    std::vector<T*> deck;
    std::vector<T*> discardPile;

public:
    // Constructor
    CardDeck() {}

    // Destructor (Penting biar gak Memory Leak)
    ~CardDeck() {
        for (T* card : deck) delete card;
        for (T* card : discardPile) delete card;
    }

    // Tambah kartu ke tumpukan (biasanya pas inisialisasi awal)
    void addCard(T* card) {
        deck.push_back(card);
    }

    // Ambil kartu paling atas
    T* drawCard() {
        if (deck.empty()) {
            if (discardPile.empty()) return nullptr; // Habis total
            
            // FAKTA: Kalau deck habis, kocok ulang dari tempat sampah!
            deck = discardPile;
            discardPile.clear();
            shuffle();
        }
        
        T* drawnCard = deck.back();
        deck.pop_back();
        return drawnCard;
    }

    // Buang kartu yang udah dipake
    void discardCard(T* card) {
        discardPile.push_back(card);
    }

    // Kocok tumpukan kartu
    void shuffle() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    }

    // Cek apakah bener-bener kosong
    bool isEmpty() const {
        return deck.empty() && discardPile.empty();
    }
};