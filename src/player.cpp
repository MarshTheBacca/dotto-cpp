#include "player.h"

#include <algorithm>
#include <format>
#include <iostream>  // std::cout
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <utility>  // std::pair
#include <vector>

#include "board.h"
#include "enums.h"
#include "other_tools.h"
#include "piece.h"
#include "validation_tools.h"

Player::Player(const int id, const Cell &cell, const Cell &bishopCell,
               const std::set<Piece> &pieces)
    : id(id), cell(cell), bishopCell(bishopCell), pieces(pieces) {}

/**
 * @brief Adds a powerup to the player's inventory
 * @param powerup The powerup to add
 */
void Player::addPowerup(const Powerup &powerup) {
    inventory.push_back(powerup);
}

/**
 * @brief Removes a powerup from the player's inventory
 * @param powerup The powerup to remove
 */
void Player::removePowerup(const Powerup &powerup) {
    vectorRemove(inventory, powerup);
}

/**
 * @brief Checks if the player has a specific powerup
 * @param powerup The powerup to check for
 * @return True if the player has the powerup, false otherwise
 */
bool Player::hasPowerup(const Powerup &powerup) const {
    return std::ranges::find(inventory, powerup) != std::ranges::end(inventory);
}

/**
 * @brief Checks if the player has any powerups
 * @return True if the player has powerups, false otherwise
 */
bool Player::hasPowerups() const {
    return !inventory.empty();
}

/**
 * @brief Prompts the player to select a powerup from their inventory and returns the selected powerup
 * @return The selected powerup or std::nullopt if the user cancels
 * @note If the player has no powerups, a message is displayed and std::nullopt is returned
 */
std::optional<Powerup> Player::selectPowerup() const {
    if (!hasPowerups()) {
        std::cout << "You have no powerups!" << std::endl;
        return std::nullopt;
    }
    std::ostringstream prompt;
    prompt << "Which powerup would you like to use?";
    for (std::size_t i = 0; i < inventory.size(); ++i) {
        prompt << std::format("\n{}) {}", i + 1, powerupToString(inventory.at(i)));
    }
    const auto exitNum = static_cast<int>(inventory.size()) + 1;
    prompt << "\n"
           << exitNum << ") Cancel";
    const int choice = getValidInt(prompt.str(), 1, exitNum);
    if (choice == exitNum) {
        return std::nullopt;
    }
    return inventory.at(choice - 1);
}

/**
 * @brief Prompts the player to select a dot to move and returns the dot's coordinate
 * @return The coordinate of the selected dot or std::nullopt if the user cancels
 */
std::optional<Piece> Player::selectPiece() const {
    std::ostringstream prompt;
    prompt << "Which piece would you like to move?";
    int count = 1;
    for (const auto &piece : pieces) {
        prompt << std::format("\n{}) {}", count++, coordToString(piece.coord));
    }
    const int exitNum = count;
    prompt << std::format("\n{}) Cancel", exitNum);
    const int selected = getValidInt(prompt.str(), 1, exitNum);
    if (selected == exitNum) {
        return std::nullopt;
    }
    // skip to the selected element and return its coordinate
    return std::make_optional(*std::next(pieces.begin(), selected - 1));
}

/**
 * @brief Calculates the new position of a dot after moving in a direction
 * @param board The game board
 * @param origin The current position of the dot
 * @param vector The direction to move in
 * @return The new position of the dot or std::nullopt if the move is invalid
 */
std::optional<std::pair<int, int>> Player::getDestination(const Board &board,
                                                          const std::pair<int, int> &origin,
                                                          const std::pair<int, int> &vector) const {
    const std::pair<int, int> newPos = vectorAddition(origin, vector);
    // If the new position is off the board, return std::nullopt
    if (!board.isWithinBounds(newPos)) {
        return std::nullopt;
    }
    const std::set<Cell> forbiddenCells = {BARRIER_CELL, cell, bishopCell};
    // If the new position is not one of the allowed characters, return std::nullopt
    if (const Cell destinationCell = board.getCell(newPos); forbiddenCells.contains(destinationCell)) {
        return std::nullopt;
        // If the new position is a blank space, calculate the move again in the same direction, effectively hopping over the space
    } else if (destinationCell == BLANK_CELL) {
        return getDestination(board, newPos, vector);
    }
    return newPos;  // have to convert to optional to agree with return type
}

/**
 * @brief Prompts the user to select a destination for the dot and returns the destination
 * @param moves The possible moves for the dot
 * @return The vector of the selected direction or std::nullopt if the user cancels
 */
std::optional<std::pair<int, int>> Player::selectDestination(const std::map<DirectionData, std::pair<int, int>> &moves) const {
    std::ostringstream prompt;
    prompt << "Where would you like to move the dot?";
    std::set<char> accepted = {'C', 'c'};
    for (const auto &[move, _] : moves) {
        prompt << std::format("\n{}) {}", move.key, move.name);
        accepted.insert(move.key);
        accepted.emplace(static_cast<char>(std::tolower(move.key)));
    }
    prompt << "\nC) Cancel";
    // convert input to upper case character
    const auto wasd = static_cast<char>(std::toupper(getValidString(prompt.str(), 1, 1, "C", std::make_optional(accepted)).value()[0]));
    if (wasd == 'C') {
        return std::nullopt;
    }
    return std::ranges::find_if(moves, [&wasd](const auto &move) { return move.first.key == wasd; })->second;
}

/**
 * @brief Detects the possible moves for a dot
 * @param board The game board
 * @param piece The dot to move
 * @param isHop Whether the a hop powerup is used
 * @return A map of directions to destination coordinates if the destination is valid
 */
std::map<DirectionData, std::pair<int, int>> Player::detectMoves(const Board &board, const Piece &piece, const bool isHop) const {
    std::map<DirectionData, std::pair<int, int>> moves;
    for (const auto &direction : piece.getDirections(isHop)) {
        if (const auto destination = getDestination(board, piece.coord, direction.vector); destination.has_value()) {
            // try_emplace will only insert the element if the key does not already exist
            moves[direction] = destination.value();
        }
    }
    return moves;
}

/**
 * @brief Attempts to perform a move by the user
 * @param vectors The vectors to move in
 * @return a pair of the origin and destination of the move or std::nullopt if the user cancels
 */
std::optional<std::pair<std::pair<int, int>, std::pair<int, int>>> Player::attemptMove(const Board &board, const bool isHop) const {
    std::map<DirectionData, std::pair<int, int>> moves;
    std::optional<Piece> selectedPiece;
    while (true) {
        selectedPiece = selectPiece();
        if (!selectedPiece.has_value()) {  // check if user cancelled piece selection
            return std::nullopt;
        }
        moves = detectMoves(board, selectedPiece.value(), isHop);
        if (moves.empty()) {
            std::cout << "This dot cannot move." << std::endl;
            continue;
        }
        break;
    }
    const std::optional<std::pair<int, int>> destination = selectDestination(moves);
    if (!destination.has_value()) {  // check if user cancelled destination selection
        return std::nullopt;
    }
    return std::make_pair(selectedPiece.value().coord, destination.value());
}

/**
 * @brief Adds a piece to the player's pieces
 * @param piece The piece to add
 */
void Player::addPiece(const Piece &piece) {
    pieces.insert(piece);
}

/**
 * @brief Removes a piece from the player's pieces
 * @param piece The piece to remove
 */
void Player::removePiece(const Piece &piece) {
    pieces.erase(piece);
}

/**
 * @brief Removes a piece from the player's pieces
 * @param coord The coordinate of the piece to remove
 */
void Player::removePiece(const std::pair<int, int> &coord) {
    pieces.erase(std::ranges::find_if(pieces, [&coord](const Piece &piece) { return piece.coord == coord; }));
}

/**
 * @brief Updates the position of a piece
 * @param piece The piece to update
 * @param newCoord The new coordinate of the piece
 */
void Player::updatePiece(Piece &piece, const std::pair<int, int> &newCoord) {
    piece.coord = newCoord;
}

/**
 * @brief Updates the position of a piece
 * @param coord The coordinate of the piece to update
 * @param newCoord The new coordinate of the piece
 * @throws std::invalid_argument if no piece is found at the given coordinate
 */
void Player::updatePiece(const std::pair<int, int> &coord, std::pair<int, int> &newCoord) {
    auto matchingPiece = std::ranges::find_if(pieces, [&coord](const Piece &piece) { return piece.coord == coord; });
    if (matchingPiece == pieces.end()) {
        throw std::invalid_argument(std::format("No piece found at the given coordinate: {}", coordToString(coord)));
    }
    // We cannot edit elements of a set directly, so we must remove the original piece and insert an updated copy
    Piece updatedPiece = *matchingPiece;
    // Remove the original piece from the set
    pieces.erase(matchingPiece);
    updatedPiece.coord = newCoord;
    // Reinsert the updated piece into the set
    pieces.insert(updatedPiece);
}

/**
 * @brief Upgrades a piece to a bishop
 * @param piece The piece to upgrade
 */
bool Player::upgradePiece(const Piece &piece) {
    Piece upgradedPiece = piece;
    if (upgradedPiece.isBishop) {
        std::cout << "This piece is already a bishop!" << std::endl;
        return false;
    }
    upgradedPiece.bishopUpgrade(bishopCell);
    pieces.erase(piece);
    pieces.insert(upgradedPiece);
    return true;
}
