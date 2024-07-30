#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "portal.h"
#include "settings_data.h"

struct Board {
    std::vector<std::vector<char>> field;
    const int length;
    const int width;
    std::vector<std::pair<int, int>> crumblies;
    std::vector<std::pair<int, int>> powerups;
    std::vector<std::pair<int, int>> barriers;
    std::vector<Portal> portals;

    explicit Board(SettingsData const &settingsData);

    std::vector<std::pair<int, int>> scanCharCoords(const char &c) const;
    void placePowerup();
    void replaceChar(std::pair<int, int> coord, const char &newChar);
    bool isWithinBounds(const std::pair<int, int> &coord) const;
    void show() const;
};

#endif  // BOARD_H