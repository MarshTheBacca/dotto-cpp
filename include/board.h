#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <set>
#include <vector>

#include "cell.h"
#include "portal.h"
#include "settings_data.h"

struct Board {
    std::vector<std::vector<Cell>> field;
    const int length;
    const int width;

    explicit Board(SettingsData const &settingsData);

    std::vector<std::vector<Cell>> generateRandomMap(const SettingsData &settingsData) const;
    std::set<std::pair<int, int>> scanCells(const Cell &targetCell) const;
    std::set<std::pair<int, int>> scanCells(const std::set<Cell> &targetCells) const;

    void placePowerup();
    void replaceCell(const std::pair<int, int> &coord, const Cell &newCell);
    bool isWithinBounds(const std::pair<int, int> &coord) const;
    void show() const;
    Cell getCell(const std::pair<int, int> &coord) const;
    void setCell(const std::pair<int, int> &coord, const Cell &newCell);
};

#endif  // BOARD_H