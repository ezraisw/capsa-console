/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#include <vector>

#define SUIT_DIAMOND 1
#define SUIT_CLUB 2
#define SUIT_HEART 3
#define SUIT_SPADE 4

class Card
{
private:
    static bool initialized;
    int suit, value;

    static void mergeSort(std::vector<Card*>& cardList, size_t start, size_t end);

public:
    static std::vector<Card*> deck;

    Card(int, int);
    void render(short x, short y) const;
    void render(short x, short y, unsigned short color) const;
    int getSuit() const;
    int getValue(bool bombed) const;
    bool lessThan(const Card& other, bool bombed) const;
    bool lessThanEqual(const Card& other, bool bombed) const;

    static void initCards();
    static void sortCards(std::vector<Card*>& cardList);
};