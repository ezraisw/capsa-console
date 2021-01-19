/**
 * Capsa
 *
 * Copyright (C) pwnedgod (https://github.com/pwnedgod)
 */
#include "Pack.h"

Pack::Pack(int identifier, int packValue1, int packValue2) :
    identifier(identifier),
    packValue1(packValue1),
    packValue2(packValue2)
{
}

bool Pack::operator<(const Pack& other)
{
    if(identifier == other.identifier) {
        if(packValue1 == other.packValue1) {
            return packValue2 < other.packValue2;
        }
        return packValue1 < other.packValue1;
    }
    return identifier < other.identifier;
}