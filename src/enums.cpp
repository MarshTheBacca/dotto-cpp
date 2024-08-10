#include "enums.h"

#include <map>
#include <string>

#include "cell.h"

/**
 * @brief converts a map into a string
 * @param map The map to convert
 */
std::string mapToString(const Map& map) {
    switch (map) {
        case Map::RANDOM:
            return "Generated";
        case Map::BREAKOUT:
            return "Breakout";
        default:
            return "Unknown";
    }
    return "Unknown";
}

/**
 * @brief converts a powerup into a string
 * @param powerup The powerup to convert
 * @return The string representation of the powerup
 */
std::string powerupToString(const Powerup& powerup) {
    switch (powerup) {
        case Powerup::HOP:
            return "Hop";
        case Powerup::DESTROYER:
            return "Destroyer";
        case Powerup::PORTAL:
            return "Portal";
        case Powerup::BISHOP:
            return "Bishop";
        default:
            return "Unknown";
    }
}

/**
 * @brief Converts a powerup into a cell
 * @param powerup The powerup to convert
 * @return The cell representation of the powerup
 */
Cell PowerupToCell(Powerup powerup) {
    switch (powerup) {
        case Powerup::HOP:
            return HOP_CELL;
        case Powerup::DESTROYER:
            return DESTROYER_CELL;
        case Powerup::PORTAL:
            return PORTAL_POWER_CELL;
        case Powerup::BISHOP:
            return BISHOP_POWER_CELL;
        default:
            return REGULAR_CELL;
    }
}

/**
 * @brief Converts a cell to a powerup
 * @param cell The cell to convert
 * @return The powerup representation of the cell
 */
Powerup cellToPowerup(const Cell& cell) {
    const std::map<Cell, Powerup> cellMap = {
        {HOP_CELL, Powerup::HOP},
        {DESTROYER_CELL, Powerup::DESTROYER},
        {PORTAL_POWER_CELL, Powerup::PORTAL},
        {BISHOP_POWER_CELL, Powerup::BISHOP},
    };
    if (!cellMap.contains(cell)) {
        return Powerup::COUNT;
    }
    return cellMap.at(cell);
}

Cell powerupToCell(const Powerup& powerup) {
    switch (powerup) {
        case Powerup::HOP:
            return HOP_CELL;
        case Powerup::DESTROYER:
            return DESTROYER_CELL;
        case Powerup::PORTAL:
            return PORTAL_POWER_CELL;
        case Powerup::BISHOP:
            return BISHOP_POWER_CELL;
        default:
            return REGULAR_CELL;
    }
}