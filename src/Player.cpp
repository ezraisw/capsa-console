/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "Player.h"

#include <cstdio>
#include "consts.h"
#include "printhelper.h"

Player::Player(std::string name) :
    name(name),
    passed(false),
    winningPlace(-1)
{
}

std::string Player::getName() const
{
    return name;
}

std::vector<Card*>& Player::getHand()
{
    return hand;
}

void Player::renderHand(short top, bool numbers, unsigned short highlight /* =0 */) const
{
    int maxPerRow = std::min<int>(hand.size(), MAX_CARD_PER_LINE);
    for(size_t i = 0; i < hand.size(); i++) {
        short x = (SIZE_X - maxPerRow * 7) / 2 + (i % maxPerRow) * 7;
        short y = top + (i / maxPerRow) * (numbers ? 6 : 4);

        if(numbers) {
            char number[3];
            sprintf(number, "%d", i + 1);
            printAt(x + 2, y + 4, number);
        }

        if((highlight & (1UL << i)) != 0) {
            hand[i]->render(x, y, FOREGROUND_GREEN | FOREGROUND_RED);
        } else {
            hand[i]->render(x, y);
        }
    }
}

bool Player::hasPassed() const
{
    return passed;
}

void Player::setPassed(bool passed)
{
    this->passed = passed;
}

int Player::getWinningPlace() const
{
    return winningPlace;
}

void Player::setWinningPlace(int winningPlace)
{
    this->winningPlace = winningPlace;
}