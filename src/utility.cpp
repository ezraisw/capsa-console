/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "utility.h"

#include <cmath>

int roundToInt(float f)
{
    return (f - (int)f) >= 0.5 ? (int)f + 1 : (int)f;
}

float randFloat()
{
    return (float) rand() / RAND_MAX;
}

int randInt(int min, int max)
{
    return roundToInt(randFloat() * (max - min) + min);
}

int wrap(int val, int bound)
{
    return val >= 0 ? val % bound : bound + (val % bound);
}

std::string ordinal(int num)
{
    char ordBuffer[20];
    int tenth = num % 10, hundth = num % 100;

    sprintf(ordBuffer, "%d", num);

    if(hundth - tenth != 10) {
        switch(tenth) {
        case 1:
            return std::string(ordBuffer) + "st";
        case 2:
            return std::string(ordBuffer) + "nd";
        case 3:
            return std::string(ordBuffer) + "rd";
        }
    }

    return std::string(ordBuffer) + "th";
}