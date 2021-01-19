/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#include <string>
#include "Card.h"

#define MAX_CARD_PER_LINE 10

#define DEFAULT_PLAYER_AMOUNT 4
#define MIN_PLAYERS 2
#define MAX_PLAYERS 6

class Player
{
private:
    std::string name;
    bool passed;
    int winningPlace;
    std::vector<Card*> hand;

public:
    Player(std::string name);
    std::string getName() const;
    std::vector<Card*>& getHand();
    void renderHand(short top, bool numbers, unsigned short highlight = 0) const;
    bool hasPassed() const;
    void setPassed(bool passed);
    int getWinningPlace() const;
    void setWinningPlace(int winningPlace);
};
