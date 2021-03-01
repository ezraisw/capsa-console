/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#include <vector>
#include "Card.h"
#include "Pack.h"
#include "Player.h"

#define PLAY_FREE 0
#define PLAY_SINGLE 1
#define PLAY_PAIR 2
#define PLAY_TRIPLE 3
#define PLAY_DUMMY 4
#define PLAY_PACK 5

#define INVALID_STRENGTH 1
#define INVALID_COMBINATION 2
#define INVALID_COMPATIBILITY 3
#define INVALID_THREE 4

class Game
{
private:
    bool playing, bombed;
    int playType, phase, phaseCards, usedPhaseCards, winners;
    size_t currentPlayerIndex, lastCompetingPlayer;
    std::vector<Card*> lastCards;
    std::vector<Player*> players;

    void end();
    void remindHidden(Player* player);
    void displayTable();
    void moveToNextPlayer();
    void changeLastCard(std::vector<Card*> newCards);
    void nextPhase();
    std::vector<Card*> getSelectedCards(Player* player, unsigned long selected);
    Pack checkPack(const std::vector<Card*>& cards);
    int validateCards(const std::vector<Card*>& cards);
    bool selectCard(Player* player);
    void shuffleCards();

public:
    Game();
    ~Game();
    void addPlayer(std::string name);
    void play();
};