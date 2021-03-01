/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "printhelper.h"

#include <conio.h>
#include <iostream>
#include "consts.h"

static Console console(GetStdHandle(STD_OUTPUT_HANDLE), SIZE_X, SIZE_Y);

void printColor(unsigned short c)
{
    console.color(c);
}

void printAt(short x, short y, std::string str)
{
    console.cursor(x, y);
    std::cout << str;
}

void printCenter(short y, std::string str)
{
    printAt((SIZE_X - str.length()) / 2, y, str);
}

void printCursor(short x, short y)
{
    console.cursor(x, y);
}

void line(size_t length)
{
    for(size_t i = 0; i < length; i++) {
        std::cout << "=";
    }
}

void clearScreen()
{
    system("cls");
}

void clearLine(short y)
{
    console.cursor(0, y);
    for(size_t i = 0; i < SIZE_X; i++) {
        std::cout << " ";
    }
    console.cursor(0, y);
}

void pause()
{
    char c;
    while((c = getch()) != '\r' && c != '\n');
}

void logo(short x, short y)
{
    printColor(FOREGROUND_WHITE | FOREGROUND_INTENSITY);
    printAt(x, y, "   ______");
    printAt(x, y + 1, "  / ____/___ _____  _________ _");
    printAt(x, y + 2, " / /   / __ `/ __ \\/ ___/ __ `/");
    printAt(x, y + 3, "/ /___/ /_/ / /_/ (__  ) /_/ /");
    printAt(x, y + 4, "\\____/\\__,_/ .___/____/\\__,_/");
    printAt(x, y + 5, "          /_/");
}