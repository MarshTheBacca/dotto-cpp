#ifndef PIECE_H
#define PIECE_H

#include <optional>
#include <set>
#include <string>
#include <utility>  // std::pair

#include "cell.h"

struct DirectionData {
    const char key;
    const std::string name;
    std::pair<int, int> vector;
    std::optional<std::pair<int, int>> destination = std::nullopt;

    bool operator<(const DirectionData &other) const {
        return key < other.key;
    }
};

struct Piece {
    std::pair<int, int> coord;
    Cell cell;
    bool isBishop;

    Piece(const std::pair<int, int> &coord, const Cell &cell, const bool isBishop);
    void updatePosition(const std::pair<int, int> &newCoord);
    void bishopUpgrade(const Cell &bishopCell);
    std::set<DirectionData> getDirections(const bool isHop) const;

    bool operator<(const Piece &other) const {
        return coord < other.coord;
    }
};

#endif  // PIECE_H