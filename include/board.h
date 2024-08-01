#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <set>
#include <vector>

#include "cell.h"
#include "portal.h"
#include "settings_data.h"

const Cell BLANK_CELL = Cell(' ', RESET);
const Cell REGULAR_CELL = Cell('/', RESET);
const Cell PLAYER_1_CELL = Cell('O', BLUE);
const Cell PLAYER_2_CELL = Cell('O', RED);
const Cell POWERUP_CELL = Cell('?', PINK);
const Cell BARRIER_CELL = Cell('#', RESET);
const Cell CRUMBLY_CELL = Cell('~', RESET);
const Cell PORTAL_CELL = Cell('@', PURPLE);

struct Board {
    std::vector<std::vector<Cell>> field;
    const int length;
    const int width;
    std::set<std::pair<int, int>> crumbliesCoords;
    std::set<std::pair<int, int>> powerupCoords;
    std::set<std::pair<int, int>> barrierCoords;
    std::set<Portal> portals;
    std::map<int, std::set<std::pair<int, int>>> dotCoords;

    explicit Board(SettingsData const &settingsData);

    std::set<std::pair<int, int>> scanCellCoords(const Cell &targetCell) const;
    void placePowerup();
    void replaceCell(std::pair<int, int> coord, const Cell &newCell);
    bool isWithinBounds(const std::pair<int, int> &coord) const;
    void show() const;
    Cell getCell(const std::pair<int, int> &coord) const;
    void setCell(const std::pair<int, int> &coord, const Cell &newCell);
};

#endif  // BOARD_H