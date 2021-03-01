/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "menu.h"

#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "consts.h"
#include "Game.h"
#include "printhelper.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

void menuPlay()
{
    char playerNumber[30];
    size_t playerAmount = DEFAULT_PLAYER_AMOUNT;

    std::vector<std::string> playerNames;
    for(size_t i = 1; i <= playerAmount; i++) {
        sprintf(playerNumber, "Player %u", i);
        playerNames.push_back(playerNumber);
    }

    bool run = true;
    while(run) {
        int menu;

        clearScreen();

        for(size_t i = 0; i < playerAmount; i++)
        {
            short x = i % 2 ? 42 : 7;
            short y = 2 + (i / 2) * 4;

            printCursor(x, y);
            printAt(x, y, "+=========================+");
            printCursor(x, y + 1);
            std::cout << "| ";
            std::cout.width(14);
            std::cout << std::left << playerNames[i];
            std::cout.width(3);
            std::cout << std::right << (Card::deck.size() / playerAmount) << " Cards |";
            printAt(x, y + 2, "+=========================+");
        }

        printAt(7, 15, "1. Start Game");
        printAt(7, 16, "2. Edit Player Name");
        printAt(7, 17, "3. Return");
        printAt(7, 19, ">> ");

        std::cin >> menu;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Game *game = new Game();
        switch(menu) {
        case 1:
            for(size_t i = 0; i < playerAmount; i++) {
                game->addPlayer(playerNames[i]);
            }
            game->play();

            delete game;
        case 3:
            run = false;
            break;
        case 2: {
            bool editing = true;
            while(editing) {
                int index = 0;

                clearScreen();

                for(size_t i = 0; i < playerAmount; i++) {
                    sprintf(playerNumber, "%d. ", i + 1);
                    printAt(7, 14 + i, playerNumber + playerNames[i]);
                }

                printAt(7, 16 + playerAmount, "Select[0 to return]: ");

                std::cin >> index;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if(index >= 1 && index <= playerAmount) {
                    std::string newName;

                    sprintf(playerNumber, "%d. ", index);

                    printColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    printAt(7, 13 + index, playerNumber + playerNames[index - 1]);

                    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);

                    bool pass = false;
                    do {
                        clearLine(18 + playerAmount);
                        printAt(7, 18 + playerAmount, "Input player name: ");
                        getline(std::cin, newName);

                        if(newName.length() >= 1 && newName.length() <= 14) {
                            pass = true;
                            clearLine(19 + playerAmount);
                        } else {
                            printAt(7, 19 + playerAmount, "Name must be between 1 and 14 characters!");
                        }
                    } while(!pass);

                    playerNames[index - 1] = newName;
                } else if(index == 0) {
                    editing = false;
                }
            } break; }
        }
    }
}

void menuAbout()
{
    clearScreen();
    logo(5, 3);

    printAt(7, 13, "Capsa is a console replica of the popular card game");
    printAt(7, 14, "Big 2 / Cap sa.");
    printAt(7, 15, "The name \"Big 2\" came from one of the rules of the game, in which");
    printAt(7, 16, "the biggest card is the number 2 card.");
    printAt(7, 18, "Big 2 / Cap sa is a turn-based card game which consists of 4 people.");
    printAt(7, 19, "Every person will receive 13 cards (52 cards / 4 people) that they can use");
    printAt(7, 20, "to compete until they run out of cards.");
    printAt(7, 22, "The last person to run out of cards will be the loser.");
    printAt(7, 23, "Each person can choose to pass or compete their cards. If said person");
    printAt(7, 24, "chooses to pass, they cannot compete until the current turn ends.");
    printAt(7, 26, "If every other player passes to current card, the owner of the last");
    printAt(7, 27, "competing card will receive a leading turn, in which they can start");
    printAt(7, 28, "the turn with their own card combination.");
    printAt(71, 31, "Page 1 of 5");

    pause();

    clearScreen();
    logo(5, 3);

    printAt(7, 13, "It is important to notice that every person in the game has to follow");
    printAt(7, 14, "the current turn's card combination. Otherwise they cannot compete and");
    printAt(7, 15, "will be forced to pass.");
    printAt(7, 17, "There are 4 valid card combinations in \"Big 2\":");
    printAt(9, 18, "1. Single");
    printAt(9, 19, "2. Pair");
    printAt(9, 20, "3. Triple");
    printAt(9, 21, "4. Pack of 5");
    printAt(11, 22, "- Straight");
    printAt(11, 23, "- Flush");
    printAt(11, 24, "- Full House");
    printAt(11, 25, "- Four-of-a-kind");
    printAt(11, 26, "- Straight Flush");
    printAt(71, 31, "Page 2 of 5");

    pause();

    clearScreen();
    logo(5, 3);

    printAt(7, 13, "1. Single");
    printAt(7, 14, "A straightforward card combination, in which every person will have to");
    printAt(7, 15, "use a single card of a higher value than the previous.");

    printAt(7, 17, "2. Pair");
    printAt(7, 18, "A card combination in which every person will have to give a pair of card");
    printAt(7, 19, "with the same value (e.g. 2 spade with 2 heart, etc) but higher than the");
    printAt(7, 20, "last competing card. If the last competing card have the same value, ");
    printAt(7, 21, "the pair with a more valuable card suit will be able to compete.");

    printAt(7, 23, "3. Triple");
    printAt(7, 24, "A card combination similar to pair, but with 3 of the same cards instead.");

    printAt(7, 26, "4. Pack of 5");
    printAt(7, 27, "Pack of 5 is a card combination consisting of a sub-combination");
    printAt(7, 28, "(from lowest to highest value): Straight, Flush, Full House,");
    printAt(7, 29, "Four-of-a-kind, and Straight Flush.");
    printAt(71, 31, "Page 3 of 5");

    pause();

    clearScreen();
    logo(5, 3);

    printAt(7, 13, "- Straight");
    printAt(7, 14, "Straight is a combination of cards in which each the card's value in ");
    printAt(7, 15, "the pack forms a sequence.");

    printAt(7, 17, "- Flush");
    printAt(7, 18, "Flush is a combination of cards in which all the card's suit is the same.");

    printAt(7, 20, "- Full House");
    printAt(7, 21, "Full House is a combination of cards consisting of a triple and");
    printAt(7, 22, "a pair. The dominant value of this pack is the triple.");

    printAt(7, 24, "- Four-of-a-kind");
    printAt(7, 25, "Four-of-a-kind consists of 4 of the same cards and another single card.");
    printAt(7, 26, "Competing with this combination will set all 2's value as the smallest.");

    printAt(71, 31, "Page 4 of 5");

    pause();

    clearScreen();
    logo(5, 3);

    printAt(7, 13, "- Straight Flush");
    printAt(7, 14, "Straight Flush is similar to Straight but all the cards have the same");
    printAt(7, 15, "suit like Flush.");
    printAt(7, 16, "Competing with this combination will set all 2's value as the smallest.");

    printAt(7, 20, "Press ENTER to return");

    printAt(71, 31, "Page 5 of 5");

    pause();
}

void menuStart()
{
    srand(time(NULL));

    short logoCenterX = SIZE_X / 2;
    short logoCenterY = SIZE_Y / 2;

    clearScreen();

    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
    printAt(logoCenterX - 33, logoCenterY - 15, "     .-~~-.");
    printAt(logoCenterX - 33, logoCenterY - 14, "    {      }");
    printAt(logoCenterX - 33, logoCenterY - 13, " .-~-.    .-~-.");
    printAt(logoCenterX - 33, logoCenterY - 12, "{              }");
    printAt(logoCenterX - 33, logoCenterY - 11, " `.__.'||`.__.'");
    printAt(logoCenterX - 33, logoCenterY - 10, "       ||");
    printAt(logoCenterX - 33, logoCenterY - 9, "      '--`");

    Sleep(500);

    printColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    printAt(logoCenterX - 14, logoCenterY - 15, "     /\\");
    printAt(logoCenterX - 14, logoCenterY - 14, "   .'  `.");
    printAt(logoCenterX - 14, logoCenterY - 13, "  '      `.");
    printAt(logoCenterX - 14, logoCenterY - 12, "<          >");
    printAt(logoCenterX - 14, logoCenterY - 11, " `.      .'");
    printAt(logoCenterX - 14, logoCenterY - 10, "   `.  .'");
    printAt(logoCenterX - 14, logoCenterY - 9, "     \\/");

    Sleep(500);

    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
    printAt(logoCenterX + 1, logoCenterY - 15, "       /\\");
    printAt(logoCenterX + 1, logoCenterY - 14, "     .'  `.");
    printAt(logoCenterX + 1, logoCenterY - 13, "    '      `.");
    printAt(logoCenterX + 1, logoCenterY - 12, " .'          `.");
    printAt(logoCenterX + 1, logoCenterY - 11, "{              }");
    printAt(logoCenterX + 1, logoCenterY - 10, " ~-...-||-...-~");
    printAt(logoCenterX + 1, logoCenterY - 9, "       ||");
    printAt(logoCenterX + 1, logoCenterY - 8, "      '--`");

    Sleep(500);

    printColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    printAt(logoCenterX + 18, logoCenterY - 14, " .-~~~-__-~~~-.");
    printAt(logoCenterX + 18, logoCenterY - 13, "{              }");
    printAt(logoCenterX + 18, logoCenterY - 12, " `.          .'");
    printAt(logoCenterX + 18, logoCenterY - 11, "   `.      .'");
    printAt(logoCenterX + 18, logoCenterY - 10, "     `.  .'");
    printAt(logoCenterX + 18, logoCenterY - 9, "       \\/");

    Sleep(750);

    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
    printAt(logoCenterX - 15, logoCenterY - 3, "   ______");
    printAt(logoCenterX - 15, logoCenterY - 2, "  / ____/___ _____  _________ _");
    printAt(logoCenterX - 15, logoCenterY - 1, " / /   / __ `/ __ \\/ ___/ __ `/");
    printAt(logoCenterX - 15, logoCenterY, "/ /___/ /_/ / /_/ (__  ) /_/ /");
    printAt(logoCenterX - 15, logoCenterY + 1, "\\____/\\__,_/ .___/____/\\__,_/");
    printAt(logoCenterX - 15, logoCenterY + 2, "          /_/");

    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);

    printCenter(logoCenterY + 8, "Press ENTER to continue");
    pause();

    Card::initCards();

    bool run = true;
    while(run)
    {
        int menu;

        clearScreen();
        logo(5, 3);

        printAt(7, 15, "1. Play");
        printAt(7, 16, "2. About");
        printAt(7, 17, "3. Exit");
        printAt(7, 19, ">> ");

        std::cin >> menu;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(menu) {
        case 1:
            menuPlay();
            break;
        case 2:
            menuAbout();
            break;
        case 3:
            run = false;
            break;
        }
    }
}
