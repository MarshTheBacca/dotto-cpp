#include "piece.h"

#include <iostream>  // std::cout
#include <set>
#include <utility>  // std::pair

#include "cell.h"

/**
 * @brief Constructs a new Piece object
 * @param coord The coordinate of the piece
 * @param cell The cell of the piece
 * @param isBishop Whether the piece is a bishop
 */
Piece::Piece(const std::pair<int, int> &coord, const Cell &cell, const bool isBishop)
    : coord(coord), cell(cell), isBishop(isBishop) {}

/**
 * @brief Updates the position of the piece
 * @param newCoord The new coordinate of the piece
 */
void Piece::updatePosition(const std::pair<int, int> &newCoord) {
    coord = newCoord;
}

/**
 * @brief Upgrades the piece to a bishop
 * @param bishopCell The cell of the bishop
 * @note If the piece is already a bishop, a message is displayed
 */
void Piece::bishopUpgrade(const Cell &bishopCell) {
    isBishop = true;
    cell = bishopCell;
}

/**
 * @brief Gets the directions the piece can move
 * @return The directions the piece can move
 */
std::set<DirectionData> Piece::getDirections(const bool isHop) const {
    if (isBishop && !isHop) {
        return {{'E', "Top right", {-1, 1}},
                {'Q', "Top left", {-1, -1}},
                {'A', "Bottom left", {1, -1}},
                {'D', "Bottom right", {1, 1}}};
    } else if (isBishop && isHop) {
        return {{'E', "Top right", {-2, 2}},
                {'Q', "Top left", {-2, -2}},
                {'A', "Bottom left", {2, -2}},
                {'D', "Bottom right", {2, 2}}};
    } else if (!isBishop && !isHop) {
        return {{'W', "Up", {-1, 0}},
                {'A', "Left", {0, -1}},
                {'S', "Down", {1, 0}},
                {'D', "Right", {0, 1}}};
    } else {
        return {{'W', "Up", {-2, 0}},
                {'A', "Left", {0, -2}},
                {'S', "Down", {2, 0}},
                {'D', "Right", {0, 2}}};
    }
}