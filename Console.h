/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#include <Windows.h>

class Console {
private:
    HANDLE hConsoleOut;
    CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
public:
    Console(HANDLE, short, short);
    ~Console();
    void cursor(short, short);
    void color(WORD);
};
