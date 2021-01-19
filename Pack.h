/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#pragma once

#define PACK_UNKNOWN 0
#define PACK_STRAIGHT 1
#define PACK_FLUSH 2
#define PACK_FULLHOUSE 3
#define PACK_FOUROFKIND 4
#define PACK_ROYALFLUSH 5

struct Pack
{
    int identifier, packValue1, packValue2;
    Pack(int identifier, int packValue1, int packValue2);
    bool operator<(const Pack&);
};
