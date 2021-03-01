/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#include <string>
#include "Console.h"

/* Console color flags */
#define FOREGROUND_WHITE 0x0007

#ifndef FOREGROUND_BLUE
#define FOREGROUND_BLUE 0x0001
#endif

#ifndef FOREGROUND_GREEN
#define FOREGROUND_GREEN 0x0002
#endif

#ifndef FOREGROUND_RED
#define FOREGROUND_RED 0x0004
#endif

#ifndef FOREGROUND_INTENSITY
#define FOREGROUND_INTENSITY 0x0008
#endif

#ifndef BACKGROUND_BLUE
#define BACKGROUND_BLUE 0x0010
#endif

#ifndef BACKGROUND_GREEN
#define BACKGROUND_GREEN 0x0020
#endif

#ifndef BACKGROUND_RED
#define BACKGROUND_RED 0x0040
#endif

#ifndef BACKGROUND_INTENSITY
#define BACKGROUND_INTENSITY 0x0080
#endif

void printColor(unsigned short c);
void printAt(short x, short y, std::string str);
void printCenter(short y, std::string str);
void printCursor(short x, short y);
void line(size_t length);
void clearScreen();
void clearLine(short y);
void pause();
void logo(short x, short y);
