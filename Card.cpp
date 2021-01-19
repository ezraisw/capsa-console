/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "Card.h"

#include <iostream>
#include "consts.h"
#include "printhelper.h"

bool Card::initialized = false;
std::vector<Card*> Card::deck;

Card::Card(int suit, int value) :
    suit(suit),
    value(value)
{
}

void Card::render(short x, short y) const
{
    render(x, y, FOREGROUND_WHITE);
}

void Card::render(short x, short y, unsigned short color) const
{
    printColor(color | FOREGROUND_INTENSITY);
    printAt(x, y, "+----+");
    printCursor(x, y + 1);
    std::cout << "|";
    switch(suit) {
    case SUIT_DIAMOND:
        printColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "\4";
        break;
    case SUIT_CLUB:
        printColor(FOREGROUND_WHITE);
        std::cout << "\5";
        break;
    case SUIT_HEART:
        printColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "\3";
        break;
    case SUIT_SPADE:
        printColor(FOREGROUND_WHITE);
        std::cout << "\6";
        break;
    }

    printColor(color | FOREGROUND_INTENSITY);
    std::cout << " ";
    std::cout.width(2);
    switch(value) {
    case 1:
        std::cout << "A";
        break;
    case 11:
        std::cout << "J";
        break;
    case 12:
        std::cout << "Q";
        break;
    case 13:
        std::cout << "K";
        break;
    default:
        std::cout << value;
        break;
    }
    std::cout << "|";
    printAt(x, y + 2, "|    |");
    printAt(x, y + 3, "+----+");

    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
}

int Card::getSuit() const
{
    return suit;
}

int Card::getValue(bool bombed) const
{
    switch(value) {
    case 1:
        return 14;
    case 2:
        return bombed ? 2 : 15;
    default:
        return value;
    }
}

bool Card::lessThan(const Card& other, bool bombed) const
{
    return getValue(bombed) == other.getValue(bombed) ? getSuit() < other.getSuit() : getValue(bombed) < other.getValue(bombed);
}

bool Card::lessThanEqual(const Card& other, bool bombed) const
{
    return getValue(bombed) == other.getValue(bombed) ? getSuit() <= other.getSuit() : getValue(bombed) < other.getValue(bombed);
}

void Card::initCards()
{
    if(initialized) {
        return;
    }

    for(size_t i = 1; i <= 4; i++) {
        for(size_t j = 1; j <= 13; j++) {
            deck.push_back(new Card(i, j));
        }
    }
}

void Card::mergeSort(std::vector<Card*>& cardList, size_t start, size_t end)
{
    if(start >= end) {
        return;
    }

    size_t length = end - start + 1;
    size_t mid = (end - start) / 2 + start;

    mergeSort(cardList, start, mid);
    mergeSort(cardList, mid + 1, end);

    std::vector<Card*> temp(length);

    for(size_t i = start, j = mid + 1, k = 0; k < length; k++) {
        if(i <= mid && (j > end || cardList[i]->lessThan(*cardList[j], false))) {
            temp[k] = cardList[i++];
            continue;
        }

        if(j <= end && (i > mid || cardList[j]->lessThanEqual(*cardList[j], false))) {
            temp[k] = cardList[j++];
            continue;
        }
    }

    for(size_t i = 0; i < length; i++) {
        cardList[start + i] = temp[i];
    }
}

void Card::sortCards(std::vector<Card*>& cardList)
{
    mergeSort(cardList, 0, cardList.size() - 1);
}
