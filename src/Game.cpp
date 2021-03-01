/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "Game.h"

#include <iostream>
#include <limits>
#include "consts.h"
#include "printhelper.h"
#include "utility.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

Game::Game() :
    playing(false),
    bombed(false),
    playType(PLAY_FREE),
    currentPlayerIndex(-1),
    lastCompetingPlayer(-1),
    phase(0),
    phaseCards(0),
    usedPhaseCards(0),
    winners(0)
{
}

Game::~Game()
{
    for(size_t i = 0; i < players.size(); i++) {
        delete players[i];
    }
}

//Add a new player into the game
void Game::addPlayer(std::string name)
{
    if(playing) {
        return;
    }

    players.push_back(new Player(name));
}

//Shuffle the deck of cards
void Game::shuffleCards()
{
    std::vector<Card*> deckCopy;
    deckCopy.insert(deckCopy.end(), Card::deck.begin(), Card::deck.end());

    std::vector<Card*> shuffledDeck;
    while(!deckCopy.empty()) {
        size_t index = randInt(0, deckCopy.size() - 1);

        shuffledDeck.push_back(deckCopy[index]);
        deckCopy.erase(deckCopy.begin() + index);
    }

    for(size_t i = 0; i < shuffledDeck.size(); i++) {
        players[i % players.size()]->getHand().push_back(shuffledDeck[i]);
    }

    for(size_t i = 0; i < players.size(); i++) {
        Card::sortCards(players[i]->getHand());
    }
}

//Reminder screen
void Game::remindHidden(Player *player)
{
    clearScreen();
    printCenter(SIZE_Y / 2 - 3, player->getName() + "'s turn");
    printCenter(SIZE_Y / 2 - 1, "Only " + player->getName() + " can see the next screen!");
    Sleep(1000);
    printCenter(SIZE_Y / 2 + 2, "Press ENTER to continue");
    pause();
}

//Display the table for the game
void Game::displayTable()
{
    clearScreen();
    for(size_t i = 0; i < players.size(); i++) {
        Player *player = players[i];

        short playerX;
        short playerY;
        if(i <= roundToInt((float)players.size() / 2) - 1) {
            playerX = i * 30 + 10;
            playerY = SIZE_Y / 6 - 4;
        } else {
            playerX = (players.size() - i - roundToInt((float)(6 - players.size()) / 3)) * 30 + 10;
            playerY = SIZE_Y / 6 * 3 + 4;
        }

        if(i == currentPlayerIndex) {
            printColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        } else {
            if(!player->getHand().empty()) {
                char cardCount[30];
                sprintf(cardCount, player->getHand().size() == 1 ? "%d card" : "%d cards", player->getHand().size());
                printAt(playerX + 8, playerY + 1, cardCount);
            }

            printAt(playerX, playerY + 4, player->getName());
        }

        if(!player->getHand().empty()) {
            printAt(playerX, playerY, "+----+");
            printAt(playerX, playerY + 1, "| EZ |");
            printAt(playerX, playerY + 2, "|    |");
            printAt(playerX, playerY + 3, "+----+");
        } else {
            printAt(playerX + 1, playerY + 2, ordinal(player->getWinningPlace()));
        }

        printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
    }

    short cardY = SIZE_Y / 3 - 3;
    if(!lastCards.empty()) {
        for(size_t i = 0; i < lastCards.size(); i++) {
            short cardX = i * 6 + 10;
            lastCards[i]->render(cardX, cardY);
        }

        std::string playTypeInfo;
        switch(playType) {
        case PLAY_SINGLE:
            playTypeInfo = "Single";
            break;
        case PLAY_PAIR:
            playTypeInfo = "Pair";
            break;
        case PLAY_TRIPLE:
            playTypeInfo = "Triple";
            break;
        case PLAY_PACK: {
            Pack pack = checkPack(lastCards);
            playTypeInfo = "Pack";
            switch(pack.identifier) {
            case PACK_STRAIGHT:
                playTypeInfo += " (Straight)";
                break;
            case PACK_FLUSH:
                playTypeInfo += " (Flush)";
                break;
            case PACK_FULLHOUSE:
                playTypeInfo += " (Full House)";
                break;
            case PACK_FOUROFKIND:
                playTypeInfo += " (Four of a Kind)";
                break;
            case PACK_ROYALFLUSH:
                playTypeInfo += " (Straight Flush)";
                break;
            } break; }
        }

        printAt(10, cardY + 5, playTypeInfo);
        printAt(10, cardY + 6, "by " + players[lastCompetingPlayer]->getName());
    }

    if(bombed) {
        printAt(20, cardY + 8, "***2 Bombed***");
    }

    if(phaseCards > 0) {
        char cardCount[30];

        printColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
        printAt(40, cardY, "+----+");
        printAt(40, cardY + 1, "| EZ |");
        printAt(40, cardY + 2, "|    |");
        printAt(40, cardY + 3, "+----+");
        printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);

        sprintf(cardCount, phaseCards == 1 ? "%d card" : "%d cards", phaseCards);
        printAt(40, cardY + 5, cardCount);
    }

    if(usedPhaseCards > 0) {
        char cardCount[30];

        printColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printAt(50, cardY, "+----+");
        printAt(50, cardY + 1, "| EZ |");
        printAt(50, cardY + 2, "|    |");
        printAt(50, cardY + 3, "+----+");
        printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);

        sprintf(cardCount, usedPhaseCards == 1 ? "%d used card" : "%d used cards", usedPhaseCards);
        printAt(50, cardY + 5, cardCount);
    }
}

//Card selection prompt
bool Game::selectCard(Player *player)
{
    DWORD selected;
    size_t total;
    int error;
    bool confirm = false;

    short promptY = (player->getHand().size() / MAX_CARD_PER_LINE + 1) * 6 + 6;

    do {
        std::vector<Card*> selectedCards;
        selected = 0;
        total = 0;
        error = 0;

        do {
            bool selecting = true;

            selectedCards.clear();
            selected = 0;
            total = 0;

            while(selecting) {
                int selection = -2;
                clearScreen();

                switch(error) {
                case INVALID_COMBINATION:
                    printCenter(2, "Invalid card combination");
                    break;
                case INVALID_COMPATIBILITY:
                    printCenter(2, "The selected combination is incompatible");
                    break;
                case INVALID_STRENGTH:
                    printCenter(2, "The selected combination is not strong enough");
                    break;
                case INVALID_THREE:
                    printCenter(2, "The combination must contain a 3 diamond");
                    break;
                }

                player->renderHand(4, true, selected);
                printAt(10, promptY, "Select cards[0 to confirm|-1 to return]: ");

                std::cin >> selection;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if(selection > 0 && selection <= player->getHand().size()) {
                    selected ^= 1UL << (selection - 1);
                    total += (selected & (1UL << (selection - 1))) != 0 ? 1 : -1;
                }

                if(selection == 0 || total >= 5) {
                    selecting = false;
                }

                if(selection == -1) {
                    return false;
                }
            }

            selectedCards = getSelectedCards(player, selected);
        } while((error = validateCards(selectedCards)) >= 0);

        if(error < 0) {
            std::string confirmation;

            do {
                clearScreen();
                player->renderHand(4, true, selected);

                printAt(10, promptY, "Confirm? [Y|N]: ");
                getline(std::cin, confirmation, '\n');
            } while(confirmation != "Y" && confirmation != "y" && confirmation != "N" && confirmation != "n");

            if(confirm = (confirmation == "Y" || confirmation == "y")) {
                changeLastCard(selectedCards);
                playType = -error;

                std::vector<Card*> &playerCards = player->getHand();
                for(size_t i = 0; i < selectedCards.size(); i++) {
                    for(std::vector<Card*>::iterator it = playerCards.begin(); it != playerCards.end(); it++) {
                        if(selectedCards[i] == *it) {
                            playerCards.erase(it);
                            break;
                        }
                    }
                }
            }
        }
    } while(!confirm);

    return true;
}

void Game::end()
{
    clearScreen();

    short centerY = SIZE_Y / 2;
    int lastWinningPlace = -1;
    for(size_t i = 0; i < players.size(); i++) {
        Player *player = players[i];

        if(player->getWinningPlace() > lastWinningPlace) {
            lastWinningPlace = player->getWinningPlace();
        }

        if(player->getWinningPlace() != -1) {
            int place = player->getWinningPlace();
            printAt(8, centerY + place - (players.size() / 2), ordinal(place) + "  " + player->getName());
        }
    }

    for(size_t i = 0; i < players.size(); i++) {
        Player *player = players[i];

        if(player->getWinningPlace() == -1) {
            int place = lastWinningPlace += 1;
            printAt(8, centerY + place - (players.size() / 2), ordinal(place) + "  " + player->getName());
        }
    }

    logo(SIZE_X - 50, centerY - 4);

    printCenter(SIZE_Y - 7, "Press ENTER to return");

    pause();
}

//Game Loop Logic
void Game::play()
{
    clearScreen();
    printCenter(SIZE_Y / 2 - 2, "Shuffling cards...");

    shuffleCards();

    printCenter(SIZE_Y / 2, "+===+");
    printCenter(SIZE_Y / 2 + 1, "|   |");
    printCenter(SIZE_Y / 2 + 2, "+===+");
    for(size_t i = 0; i < 20; i++) {
        std::string loadingChar;
        switch(i % 8) {
            case 0:
            case 4:
                loadingChar = "|";
                break;
            case 1:
            case 5:
                loadingChar = "/";
                break;
            case 2:
            case 6:
                loadingChar = "-";
                break;
            case 3:
            case 7:
                loadingChar = "\\";
                break;
        }
        printCenter(SIZE_Y / 2 + 1, loadingChar);
        Sleep(100);
    }

    clearScreen();

    printCenter(SIZE_Y / 2 - 2, "Capsa is ready to be played");
    printCenter(SIZE_Y / 2 + 2, "Press ENTER to continue");

    pause();

    Card smallestCard(INT_MAX, INT_MAX);
    for(size_t i = 0; i < players.size(); i++) {
        Player *player = players[i];

        remindHidden(player);
        clearScreen();

        printCenter(SIZE_Y / 2 - 6, player->getName());
        printCenter(SIZE_Y / 2 - 4, "Here are your hand for the current game");
        player->renderHand(SIZE_Y / 2, false);

        if(player->getHand()[0]->lessThan(smallestCard, false)) {
            smallestCard = *player->getHand()[0];
            currentPlayerIndex = i;
        }

        pause();
    }

    clearScreen();

    printCenter(SIZE_Y / 2 - 1, "+===================+");
    printCenter(SIZE_Y / 2, "|    Game  Start!   |");
    printCenter(SIZE_Y / 2 + 1, "+===================+");

    Sleep(1000);

    size_t prevPlayerIndex = -1;
    bool run = true;
    while(run) {
        Player *player = players[currentPlayerIndex];

        if(prevPlayerIndex != currentPlayerIndex) {
            if(!player->getHand().empty()) {
                remindHidden(player);
            }

            prevPlayerIndex = currentPlayerIndex;
        }

        if(lastCompetingPlayer == currentPlayerIndex) {
            nextPhase();

            if(player->getHand().empty()) {
                displayTable();
                printCenter(SIZE_Y - 8, player->getName() + " bequeathed their challenge rights!");
                Sleep(1000);
                moveToNextPlayer();
                continue;
            }
        }

        displayTable();

        int menu;

        if(playType == PLAY_FREE) {
            printAt(7, SIZE_Y - 9, "1. Challenge");
        } else {
            printAt(7, SIZE_Y - 9, "1. View Hand / Compete");
            printAt(7, SIZE_Y - 8, "2. Pass");
        }
        printAt(7, SIZE_Y - 6, ">> ");

        std::cin >> menu;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(menu) {
        case 1:
            if(selectCard(player)) {
                if(player->getHand().empty()) {
                    winners++;
                    player->setWinningPlace(winners);
                }

                displayTable();
                printCenter(SIZE_Y - 8, player->getName() + " has put down their card!");

                Sleep(1000);
                moveToNextPlayer();
            }
            break;
        case 2:
            if(playType == PLAY_FREE) {
                break;
            }

            displayTable();
            printCenter(SIZE_Y - 8, player->getName() + " passed!");
            player->setPassed(true);

            Sleep(1000);
            moveToNextPlayer();
            break;
        }

        if(winners == players.size() - 1) {
            displayTable();
            printCenter(SIZE_Y - 8, "Game over!");
            printCenter(SIZE_Y - 7, "Press ENTER to continue");
            pause();
            end();
            run = false;
        }
    }
}

//Vector of cards from selection bits
std::vector<Card*> Game::getSelectedCards(Player *player, DWORD selected)
{
    std::vector<Card*> cards;

    for(size_t i = 0; i < player->getHand().size(); i++) {
        if((selected & (1UL << i)) != 0) {
            cards.push_back(player->getHand()[i]);
        }
    }

    return cards;
}

//Validate poker packs
Pack Game::checkPack(const std::vector<Card*> &cards)
{
    bool straight = true, flush = true;

    int straightBiggestValue = -1, straightBiggestSuit = -1, lastValue = -1,
        flushSuit = cards[0]->getSuit(), flushBiggestValue = -1,
        valueCount[13] = {0};

    for(size_t i = 0; i < cards.size(); i++) {
        int value = cards[i]->getValue(true);
        int suit = cards[i]->getSuit();

        if(straight) {
            if(lastValue != -1 && (lastValue - 1) % 13 + 2 != value) {
                straight = false;
            }

            if(straightBiggestValue < value) {
                straightBiggestValue = value;
                straightBiggestSuit = suit;
            }

            lastValue = value;
        }

        if(flush) {
            if(cards[i]->getSuit() != flushSuit) {
                flush = false;
            }

            if(flushBiggestValue < value) {
                flushBiggestValue = value;
            }
        }

        valueCount[value - 2]++;
    }

    if(straight && !flush) {
        return Pack(PACK_STRAIGHT, straightBiggestValue, straightBiggestSuit);
    } else if(!straight && flush) {
        return Pack(PACK_FLUSH, flushSuit, flushBiggestValue);
    } else if(straight && flush) {
        return Pack(PACK_ROYALFLUSH, flushSuit, flushBiggestValue);
    }

    int singleCard = -1, twoCard = -1, threeCard = -1, fourCard = -1;
    for(size_t i = 0; i < 13; i++) {
        switch(valueCount[i]) {
        case 1:
            singleCard = i;
            break;
        case 2:
            twoCard = i;
            break;
        case 3:
            threeCard = i;
            break;
        case 4:
            fourCard = i;
            break;
        }
    }

    if(twoCard != -1 && threeCard != -1) {
        return Pack(PACK_FULLHOUSE, threeCard + 2, twoCard + 2);
    }

    if(fourCard != -1) {
        return Pack(PACK_FOUROFKIND, fourCard + 2, singleCard + 2);
    }

    return Pack(PACK_UNKNOWN, 0, 0);
}

//Validate selected cards
int Game::validateCards(const std::vector<Card*> &cards)
{
    if(cards.empty() || cards.size() == 4 || cards.size() > 5) {
        return INVALID_COMBINATION;
    }

    if(phase == 0 && playType == PLAY_FREE && (cards[0]->getValue(bombed) != 3 || cards[0]->getSuit() != SUIT_DIAMOND)) {
        return INVALID_THREE;
    }

    if(playType == PLAY_FREE) {
        switch(cards.size()) {
        case 1:
            return -1;
        case 2:
            if(cards[0]->getValue(true) == cards[1]->getValue(true)) {
                return -2;
            }
            break;
        case 3:
            if(cards[0]->getValue(true) == cards[1]->getValue(true) && cards[1]->getValue(true) == cards[2]->getValue(true)) {
                return -3;
            }
            break;
        case 5:
            if(checkPack(cards).identifier != PACK_UNKNOWN) {
                return -5;
            }
            break;
        }

        return INVALID_COMBINATION;
    }

    switch(cards.size()) {
    case 1:
        if(playType != PLAY_SINGLE) {
            return INVALID_COMPATIBILITY;
        }

        if(lastCards[0]->lessThan(*cards[0], bombed)) {
            return -1;
        }

        return INVALID_STRENGTH;
    case 2:
        if(playType == PLAY_PAIR) {
            if(cards[0]->getValue(true) == cards[1]->getValue(true)) {
                if(lastCards[1]->lessThan(*cards[1], true)) {
                    return -2;
                }

                return INVALID_STRENGTH;
            }
            return INVALID_COMBINATION;
        }
        return INVALID_COMPATIBILITY;
    case 3:
        if(playType == PLAY_TRIPLE) {
            if(cards[0]->getValue(true) == cards[1]->getValue(true) && cards[1]->getValue(true) == cards[2]->getValue(true)) {
                if(lastCards[2]->lessThan(*cards[2], true)) {
                    return -3;
                }
                return INVALID_STRENGTH;
            }
            return INVALID_COMBINATION;
        }
        return INVALID_COMPATIBILITY;
    case 5:
        if(playType == PLAY_PACK) {
            Pack pack = checkPack(cards);
            if(pack.identifier != PACK_UNKNOWN) {
                Pack lastPack = checkPack(lastCards);
                if(lastPack < pack) {
                    return -5;
                }
                return INVALID_STRENGTH;
            }
            return INVALID_COMBINATION;
        }
        return INVALID_COMPATIBILITY;
    }

    return INVALID_COMBINATION;
}

//Move the turn to the next player
void Game::moveToNextPlayer()
{
    currentPlayerIndex++;

    if(currentPlayerIndex >= players.size()) {
        currentPlayerIndex %= players.size();
    }

    if(players[currentPlayerIndex]->hasPassed() || (players[currentPlayerIndex]->getHand().empty() && currentPlayerIndex != lastCompetingPlayer)) {
        moveToNextPlayer();
    }
}

//Change into the next play phase
void Game::nextPhase()
{
    usedPhaseCards += phaseCards + lastCards.size();
    phaseCards = 0;

    phase++;

    lastCards.clear();
    lastCompetingPlayer = -1;
    playType = PLAY_FREE;

    for(size_t i = 0; i < players.size(); i++) {
        players[i]->setPassed(false);
    }
}

//Change the last card that is put on top
void Game::changeLastCard(std::vector<Card*> newCards)
{
    phaseCards += lastCards.size();

    if(newCards.size() == 5) {
        Pack pack = checkPack(newCards);
        if(pack.identifier == PACK_FOUROFKIND || pack.identifier == PACK_ROYALFLUSH) {
            bombed = !bombed;
        }
    }

    lastCards = newCards;
    lastCompetingPlayer = currentPlayerIndex;
}