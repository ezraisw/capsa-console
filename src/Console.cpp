/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "Console.h"

Console::Console(HANDLE hConsoleOut, short width, short height) :
    hConsoleOut(hConsoleOut)
{
    GetConsoleScreenBufferInfo(hConsoleOut, &defaultBufferInfo);

    SMALL_RECT window = {0, 0, width - 1, height - 1};
    SetConsoleWindowInfo(hConsoleOut, true, &window);

    COORD buffer = {width, height};
    SetConsoleScreenBufferSize(hConsoleOut, buffer);
}

Console::~Console()
{
    SetConsoleTextAttribute(hConsoleOut, defaultBufferInfo.wAttributes);
    SetConsoleScreenBufferSize(hConsoleOut, defaultBufferInfo.dwSize);
    SetConsoleWindowInfo(hConsoleOut, true, &defaultBufferInfo.srWindow);
}

void Console::cursor(short x, short y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(hConsoleOut, coord);
}

void Console::color(WORD color)
{
    SetConsoleTextAttribute(hConsoleOut, color);
}
