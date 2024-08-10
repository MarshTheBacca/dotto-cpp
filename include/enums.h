#ifndef ENUMS_H
#define ENUMS_H

#include <string>

#include "cell.h"

enum class Map {
    RANDOM,
    BREAKOUT,

    COUNT  // Variable at the end to get the number of maps
};

enum class Powerup {
    HOP,
    DESTROYER,
    PORTAL,
    BISHOP,

    COUNT  // Variable at the end to get the number of powerups
};

std::string mapToString(const Map &map);

std::string powerupToString(const Powerup &powerup);
Cell powerupToCell(const Powerup &powerup);
Powerup cellToPowerup(const Cell &cell);

#endif  // ENUMS_H