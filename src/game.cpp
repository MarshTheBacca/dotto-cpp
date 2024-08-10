#include "game.h"

#include <algorithm>
#include <format>  // std::format
#include <iostream>
#include <map>
#include <memory>  // std::shared_ptr
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <tuple>    // std::apply
#include <utility>  // std::pair
#include <vector>

#include "board.h"
#include "enums.h"
#include "globals.h"
#include "other_tools.h"
#include "portal.h"
#include "random.h"
#include "validation_tools.h"

/**
 * @brief Scans the board for pieces of a specific cell type
 * @param board The board to scan
 * @param targetCell The cell to scan for
 * @return A set of pieces with the target cell
 */
std::set<Piece> scanPieces(const Board &board, const Cell &targetCell) {
    std::set<Piece> pieces;
    for (int i = 0; i < board.length; i++) {
        for (int j = 0; j < board.width; j++) {
            if (board.getCell({i, j}) == targetCell) {
                pieces.emplace(std::pair<int, int>(i, j), targetCell, false);
            }
        }
    }
    return pieces;
}

/**
 * @brief Construct a new Game object from a SettingsData object
 * @note Inventories are blank and currentPlayerID and turnNumber are set to 1
 */
Game::Game(const SettingsData &settingsData) : settings(settingsData),
                                               board(settingsData),
                                               player1(std::make_shared<Player>(1, PLAYER_1_CELL, BISHOP_1_CELL, scanPieces(board, PLAYER_1_CELL))),
                                               player2(std::make_shared<Player>(1, PLAYER_2_CELL, BISHOP_2_CELL, scanPieces(board, PLAYER_2_CELL))),
                                               crumbliesCoords(board.scanCells(CRUMBLY_CELL)),
                                               powerupSourceCoords(board.scanCells(POWERUP_SOURCE_CELL)),
                                               barrierCoords(board.scanCells(BARRIER_CELL)) {}

/**
 * @brief Prompts the user for a valid coordinate and edits it appropriately
 * @param prompt The prompt to display to the user
 * @param targetCell The character to check for at the coordinate
 * @param newCell The character to replace the targetCell with
 */
std::optional<std::pair<int, int>> Game::editCoord(const std::string &prompt,
                                                   const Cell &targetCell,
                                                   const Cell &newCell) {
    while (true) {
        const auto coord = getValidCoord(prompt, board.length, board.length);
        if (coord == std::nullopt) {
            return std::nullopt;
        }
        // .value() returns the value of coord if it isn't std::nullopt, and throws an error otherwise
        if (board.getCell(coord.value()) == targetCell) {
            board.setCell(coord.value(), newCell);
            return coord;
        }
        std::cout << "Coordinate does not correspond to " << targetCell.repr() << std::endl;
    }
}

/**
 * @brief Updates the position of a dot after moving through a portal
 * @param coord The coordinate of the portal
 * @return The exit of the portal
 * @throws std::invalid_argument if the coordinate is not a member of the board's portals
 */
std::pair<int, int> Game::updatePortals(const std::pair<int, int> &coord) {
    for (const auto &portal : portals) {
        if (portal.isMember(coord)) {
            board.replaceCell(coord, REGULAR_CELL);
            const std::pair<int, int> destination = portal.getOpposite(coord);
            // remove portal from the set of portals
            portals.erase(portal);
            // return the exit of the portal
            return destination;
        }
    }
    throw std::invalid_argument("Coordinate is not a portal: " + coordToString(coord));
}

/**
 * @brief Processes a move by updating the board and inventories
 * @param origin The origin of the move
 * @param destination The destination of the move
 * @note If the destination is a powerup, it is added to the player's inventory
 */
void Game::processMove(const std::pair<int, int> &origin, std::pair<int, int> &destination) {
    // destination cannot be const because it may be updated by updatePortals
    // If the origin is a crumbly cell, remove it from the set of crumbly cells
    const auto originCell = board.getCell(origin);
    if (crumbliesCoords.contains(origin)) {
        crumbliesCoords.erase(origin);
        board.replaceCell(origin, BLANK_CELL);
    } else if (powerupSourceCoords.contains(origin)) {
        board.replaceCell(origin, POWERUP_SOURCE_CELL);
    } else {  // otherwise, replace the origin with a regular cell
        board.replaceCell(origin, REGULAR_CELL);
    }

    // handle actions based on the destination cell
    if (const Cell destinationCell = board.getCell(destination); destinationCell == DESTROYER_CELL) {
        getAllyPlayer()->addPowerup(Powerup::DESTROYER);
        std::cout << std::format("Player {} has found a Destroyer!", currentPlayerID) << std::endl;
    } else if (destinationCell == HOP_CELL) {
        getAllyPlayer()->addPowerup(Powerup::HOP);
        std::cout << std::format("Player {} has found a Hop!", currentPlayerID) << std::endl;
    } else if (destinationCell == BISHOP_POWER_CELL) {
        getAllyPlayer()->addPowerup(Powerup::BISHOP);
        std::cout << std::format("Player {} has found a Bishop!", currentPlayerID) << std::endl;
    } else if (destinationCell == PORTAL_POWER_CELL) {
        getAllyPlayer()->addPowerup(Powerup::PORTAL);
        std::cout << std::format("Player {} has found a Portal!", currentPlayerID) << std::endl;
    } else if (destinationCell == PORTAL_CELL) {
        destination = updatePortals(destination);
    } else if (destinationCell == getTargetCell()) {
        // capture their piece
        getTargetPlayer()->removePiece(destination);
    }
    // move the dot to the destination, and update the player's piece
    board.replaceCell(destination, originCell);
    getAllyPlayer()->updatePiece(origin, destination);
}

/**
 * @brief Checks if a player has lost the game
 * @return True if the player has lost, false otherwise
 * @note A player loses if they have no dots left
 */
bool Game::checkDefeat() const {
    return getTargetPlayer()->pieces.empty();
}

/**
 * @brief Prompts the user to use a powerup
 * @return True if a powerup was used, false otherwise
 */
bool Game::usePowerup() {
    const std::optional<Powerup> chosenPowerup = getAllyPlayer()->selectPowerup();
    if (!chosenPowerup.has_value()) {
        return false;
    } else if (chosenPowerup.value() == Powerup::PORTAL) {
        const std::optional<std::pair<int, int>> coord_1 = editCoord("Enter the first portal coordinate", REGULAR_CELL, PORTAL_CELL);
        if (!coord_1.has_value()) {
            return false;
        }
        const std::optional<std::pair<int, int>> coord_2 = editCoord("Enter the second portal coordinate", REGULAR_CELL, PORTAL_CELL).value();
        if (!coord_2.has_value()) {
            board.replaceCell(coord_1.value(), REGULAR_CELL);  // undo the first portal
            return false;
        }
        portals.emplace(coord_1.value(), coord_2.value());
    } else if (chosenPowerup.value() == Powerup::HOP) {
        auto originDest = getAllyPlayer()->attemptMove(board, true);
        if (!originDest.has_value()) {
            return false;
        }
        auto [origin, destination] = originDest.value();
        processMove(origin, destination);
    } else if (chosenPowerup.value() == Powerup::DESTROYER && !editCoord("Which barrier would you like to destroy?", BARRIER_CELL, REGULAR_CELL).has_value()) {
        return false;
    } else if (chosenPowerup.value() == Powerup::BISHOP) {
        auto chosenPiece = getAllyPlayer()->selectPiece();
        if (!chosenPiece.has_value()) {
            return false;
        }
        if (!getAllyPlayer()->upgradePiece(chosenPiece.value())) {
            return false;
        }
        board.setCell(chosenPiece.value().coord, getAllyBishopCell());
    }
    getAllyPlayer()->removePowerup(chosenPowerup.value());
    return true;
}

/**
 * @brief Prompts the user to save their score and saves it in the scores file
 */
void Game::scoreSave() const {
    if (confirm("Would you like to save the score?")) {
        std::vector<std::vector<std::string>> scores = import2D(SCORESPATH);
        std::string scoreName = getValidString("Enter your names (c to cancel): ", 1, 20, "c", std::nullopt, std::set<char>{',', '\n'}).value();
        scores.push_back({scoreName, std::to_string(board.length), std::to_string(board.width), std::to_string(settings.numDots), std::to_string(turnNumber)});
        export2D(SCORESPATH, scores);
    }
}

/**
 * @brief Places a powerup on the board at a random powerup source cell
 * @note If no powerup source cells are available, the function does nothing
 */
void Game::placePowerup() {
    // Convert the set to a vector for shuffling
    std::vector<std::pair<int, int>> potentialCells(powerupSourceCoords.begin(), powerupSourceCoords.end());
    Random::getInstance().shuffleVector(potentialCells);
    for (const auto &powerupCoord : potentialCells) {
        if (board.getCell(powerupCoord) == POWERUP_SOURCE_CELL) {
            // Get random powerup
            const Powerup newPowerup = generateRandomPowerup();
            // Replace the powerup source cell with the new powerup cell
            board.replaceCell(powerupCoord, powerupToCell(newPowerup));
            return;
        }
    }
}

/**
 * @brief Gets the cell of the current player
 */
const Cell &Game::getTargetCell() const {
    return getTargetPlayer()->cell;
}

/**
 * @brief Gets the bishop cell of the current opponent
 */
const Cell &Game::getTargetBishopCell() const {
    return getTargetPlayer()->bishopCell;
}

/**
 * @brief Gets the cell of the current player
 */
const Cell &Game::getAllyCell() const {
    return getAllyPlayer()->cell;
}

/**
 * @brief Gets the bishop cell of the current player
 */
const Cell &Game::getAllyBishopCell() const {
    return getAllyPlayer()->bishopCell;
}

const std::shared_ptr<Player> &Game::getTargetPlayer() const {
    return currentPlayerID == 1 ? player2 : player1;
}

const std::shared_ptr<Player> &Game::getAllyPlayer() const {
    return currentPlayerID == 1 ? player1 : player2;
}

/**
 * @brief Main game loop - plays the game until a player wins or concedes
 */
void Game::play() {
    while (true) {
        if (turnNumber % settings.powerupPlacementFrequency == 0) {
            placePowerup();
        }
        board.show();
        std::cout << std::format("Player {}'s turn  \t\tTurn: {}", currentPlayerID, turnNumber) << std::endl;
        const int option = getValidInt("What would you like to do? \n1) Move\n2) Use a Powerup\n3) Concede", 1, 3);
        if (option == 1) {
            // can't be const because processMove may change destination
            auto originDest = getAllyPlayer()->attemptMove(board, false);
            if (!originDest.has_value()) {
                continue;
            }
            auto [origin, destination] = originDest.value();
            processMove(origin, destination);
        } else if (option == 2 && !usePowerup()) {
            continue;
        } else if (option == 3) {
            if (confirm("Are you sure you want to concede?")) {
                std::cout << "Player " << currentPlayerID << " has conceded." << std::endl;
                break;
            } else {
                continue;
            }
        }
        if (checkDefeat()) {
            board.show();
            break;
        }
        currentPlayerID = 3 - currentPlayerID;
        turnNumber += 1;
    }
    std::cout << std::format("Player {} has won in {} turns!", currentPlayerID, turnNumber) << std::endl;
    scoreSave();
}