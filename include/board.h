#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <set>
#include <vector>

#include "portal.h"
#include "settings_data.h"

struct Board {
    std::vector<std::vector<char>> field;
    const int length;
    const int width;
    std::set<std::pair<int, int>> crumbliesCoords;
    std::set<std::pair<int, int>> powerupCoords;
    std::set<std::pair<int, int>> barrierCoords;
    std::set<Portal> portals;
    std::map<int, std::set<std::pair<int, int>>> dotCoords;

    explicit Board(SettingsData const &settingsData);

    std::set<std::pair<int, int>> scanCharCoords(const char &c) const;
    void placePowerup();
    void replaceChar(std::pair<int, int> coord, const char &newChar);
    bool isWithinBounds(const std::pair<int, int> &coord) const;
    void show() const;
    char getChar(const std::pair<int, int> &coord) const;
    void setChar(const std::pair<int, int> &coord, const char &newChar);
};

#endif  // BOARD_H