#include "game.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>

#include "board.h"
#include "globals.h"
#include "other_tools.h"
#include "portal.h"
#include "random.h"
#include "validation_tools.h"

const std::map<int, Cell> DOT_CELLS = {{1, PLAYER_1_CELL}, {2, PLAYER_2_CELL}};
const std::map<char, std::string, std::less<>> DIRECTIONS{{'D', "Right"}, {'A', "Left"}, {'S', "Down"}, {'W', "Up"}};
const std::vector<std::string> POWERUPS{"Portal", "Double-Jump", "Destroyer"};

/**
 * @brief Construct a new Game object from a SettingsData object
 * @note Inventories are blank and turn and turnNumber are set to 1
 */
Game::Game(const SettingsData &settingsData) : settings(settingsData),
                                               board(settingsData),
                                               inventory({{1, {}}, {2, {}}}),
                                               deletes({{1, settings.numDeletes}, {2, settings.numDeletes}}),
                                               creates({{1, settings.numCreates}, {2, settings.numCreates}}),
                                               turnNumber(1),
                                               turn(1){};

/**
 * @brief Prompts the user for a valid coordinate and edits it appropriately
 * @param prompt The prompt to display to the user
 * @param targetCell The character to check for at the coordinate
 * @param newCell The character to replace the targetCell with
 */
std::optional<std::pair<int, int>> Game::editCoord(const std::string &prompt, const Cell &targetCell, const Cell &newCell) {
    std::optional<std::pair<int, int>> coord;
    while (true) {
        coord = getValidCoord(prompt, settings.length, settings.width);
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
 * @brief Prompts the user to select a dot to move and returns the dot's coordinate
 * @return The coordinate of the selected dot or std::nullopt if the user cancels
 */
std::optional<std::pair<int, int>> Game::getOrigin() const {
    std::ostringstream prompt;
    prompt << "Which dot would you like to move?";

    const auto &coords = board.dotCoords.at(turn);
    for (std::size_t i = 0; i < coords.size(); ++i) {
        auto it = std::next(coords.begin(), i);                        // skip to the i'th element
        prompt << std::format("\n{}) {}", i + 1, coordToString(*it));  // dereference the iterator to get the pair
    }
    auto exitNum = static_cast<int>(coords.size()) + 1;
    prompt << std::format("\n{}) Cancel", exitNum);
    int selected = getValidInt(prompt.str(), 0, exitNum);
    if (selected == exitNum) {
        return std::nullopt;
    }
    return *std::next(coords.begin(), selected - 1);  // skip to the selected element and return it
}

/**
 * @brief Prompts the user to select a destination for the dot and returns the destination
 * @param moves The possible moves for the dot
 * @return The destination of the dot or std::nullopt if the user cancels
 */
std::optional<std::pair<int, int>> Game::getDestination(const std::map<char, std::pair<int, int>, std::less<>> &moves) const {
    std::ostringstream prompt;
    prompt << "Where would you like to move the dot?";
    for (const auto &[key, _] : moves) {
        prompt << std::format("\n{}) {}", key, DIRECTIONS.at(key));
    }
    prompt << "\nC) Cancel";
    std::set<char> accepted = {'C', 'c'};
    std::cout << prompt.str() << std::endl;
    for (const auto &[key, _] : moves) {
        accepted.insert(key);
        accepted.emplace(static_cast<char>(std::tolower(key)));
    }
    char wasd = getValidString(prompt.str(), 1, 1, "C", std::make_optional(accepted)).value()[0];
    // convert to upper case
    wasd = static_cast<char>(std::toupper(wasd));
    if (wasd == 'C') {
        return std::nullopt;
    }
    return moves.at(wasd);
}

/**
 * @brief Calculates the new position of a dot after moving in a direction
 * @param position The current position of the dot
 * @param vector The direction to move in
 * @return The new position of the dot or std::nullopt if the move is invalid
 */
std::optional<std::pair<int, int>> Game::calculateMove(const std::pair<int, int> &position, const std::pair<int, int> &vector) const {
    std::pair<int, int> newPos = vectorAddition(position, vector);
    // If the new position is off the board, return std::nullopt
    if (!board.isWithinBounds(newPos)) {
        return std::nullopt;
    }
    std::set<Cell> allowedCells = {REGULAR_CELL, BLANK_CELL, POWERUP_CELL, CRUMBLY_CELL, PORTAL_CELL, getTargetCell()};
    // If the new position is not one of the allowed characters, return std::nullopt
    if (const Cell destinationCell = board.getCell(newPos); !allowedCells.contains(destinationCell)) {
        return std::nullopt;
        // If the new position is a blank space, calculate the move again in the same direction, effectively hopping over the space
    } else if (destinationCell == BLANK_CELL) {
        return calculateMove(newPos, vector);
    }
    return std::make_optional(newPos);  // have to convert to optional to agree with return type
}

/**
 * @brief Updates the position of a dot after moving through a portal
 * @param coord The coordinate of the portal
 * @return The exit of the portal
 * @throws std::invalid_argument if the coordinate is not a member of the board's portals
 */
std::pair<int, int> Game::updatePortals(const std::pair<int, int> &coord) {
    for (const auto &portal : board.portals) {
        if (portal.isMember(coord)) {
            board.replaceCell(coord, REGULAR_CELL);
            std::pair<int, int> destination = portal.getOpposite(coord);
            // remove portal from the set of portals
            board.portals.erase(portal);
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
    if (board.crumbliesCoords.contains(origin)) {
        board.crumbliesCoords.erase(origin);
        board.replaceCell(origin, BLANK_CELL);
    } else {
        board.replaceCell(origin, REGULAR_CELL);
    }
    if (Cell destinationCell = board.getCell(destination); destinationCell == POWERUP_CELL) {
        // add random powerup to user's inventory
        std::string newPowerup = POWERUPS.at(Random::getInstance().getInt(0, POWERUPS.size() - 1));
        inventory.at(turn).push_back(newPowerup);
        std::cout << std::format("Player {} has found a {}!", turn, newPowerup) << std::endl;
    } else if (destinationCell == PORTAL_CELL) {
        destination = updatePortals(destination);
    } else if (destinationCell == CRUMBLY_CELL) {
        board.crumbliesCoords.emplace(destination);
    } else if (destinationCell == getTargetCell()) {
        // capture their piece
        board.dotCoords.at(3 - turn).erase(destination);
    }
    board.replaceCell(destination, getAllyCell());
    board.dotCoords.at(turn).erase(origin);
    board.dotCoords.at(turn).emplace(destination);
    // sets in C++ are already sorted, so no need to resort the dotCoords
}

/**
 * @brief Detects the possible moves for a dot
 * @param origin The origin of the dot
 * @param vectors The vectors to move in
 * @return A map of directions to destination coordinates if the destination is valid
 */
std::map<char, std::pair<int, int>, std::less<>> Game::detectMoves(const std::pair<int, int> &origin,
                                                                   const std::vector<std::pair<int, int>> &vectors) const {
    std::map<char, std::pair<int, int>, std::less<>> moves;
    const std::vector<char> DIRECTIONKEYS = {'D', 'A', 'S', 'W'};
    for (std::size_t i = 0; i < vectors.size(); ++i) {
        if (auto destination = calculateMove(origin, vectors.at(i)); destination.has_value()) {
            // Use the DIRECTIONKEYS vector to get the correct direction character
            moves.try_emplace(DIRECTIONKEYS.at(i), destination.value());
        }
    }
    return moves;
}

/**
 * @brief Attempts to perform a move by the user
 * @param vectors The vectors to move in
 * @return True if the move was successful, false otherwise
 * @note The function will keep prompting the user until a valid move is made
 */
bool Game::attemptMove(const std::vector<std::pair<int, int>> &vectors) {
    // initialise variables outside of while loop because they're needed after
    std::map<char, std::pair<int, int>, std::less<>> moves;
    std::optional<std::pair<int, int>> origin;
    while (true) {
        origin = getOrigin();
        if (origin == std::nullopt) {
            return false;
        }
        moves = detectMoves(origin.value(), vectors);
        if (moves.empty()) {
            std::cout << "This dot cannot move." << std::endl;
            continue;
        }
        break;
    }
    std::optional<std::pair<int, int>> destination = getDestination(moves);
    if (destination == std::nullopt) {
        return false;
    }
    processMove(origin.value(), destination.value());
    return true;
}

/**
 * @brief Checks if a player has lost the game
 * @return True if the player has lost, false otherwise
 * @note A player loses if they have no dots left
 */
bool Game::checkDefeat() const {
    return board.dotCoords.at(3 - turn).empty();
}

/**
 * @brief Handles the deletion or creation of a space
 * @param actionName The name of the action to display to the user
 * @param countDict The dictionary of the number of actions left for each player
 * @param prompt The prompt to display to the user
 * @param targetCell The character to check for at the coordinate
 * @param newCell The character to replace the targetCell with
 */
bool Game::handleDeleteCreate(const std::string_view &actionName, std::map<int, int> &countDict, const std::string &prompt,
                              const Cell &targetCell, const Cell &newCell) {
    if (countDict.at(turn) == 0) {
        std::cout << "You ran out of " << actionName << "s." << std::endl;
        return false;
    }
    if (!editCoord(prompt, targetCell, newCell).has_value()) {
        return false;
    }
    countDict.at(turn) -= 1;
    return true;
}

/**
 * @brief Prompts the user to use a powerup
 * @return True if a powerup was used, false otherwise
 */
bool Game::usePowerup() {
    if (inventory.at(turn).empty()) {
        std::cout << "You don't have any powerups" << std::endl;
        return false;
    }
    std::ostringstream prompt;
    prompt << "Which powerup would you like to use?";
    for (std::size_t i = 0; i < inventory.at(turn).size(); ++i) {
        prompt << std::format("\n{}) {}", i + 1, inventory.at(turn).at(i));
    }
    auto exitNum = static_cast<int>(inventory.at(turn).size()) + 1;
    prompt << "\n"
           << exitNum << ") Cancel";
    const int choice = getValidInt(prompt.str(), 1, exitNum);
    if (choice == exitNum) {
        return false;
    }
    if (std::string chosenPowerup = inventory.at(turn).at(choice - 1); chosenPowerup == "Portal") {
        std::optional<std::pair<int, int>> coord_1 = editCoord("Enter the first portal coordinate", REGULAR_CELL, PORTAL_CELL);
        if (!coord_1.has_value()) {
            return false;
        }
        std::optional<std::pair<int, int>> coord_2 = editCoord("Enter the second portal coordinate", REGULAR_CELL, PORTAL_CELL).value();
        if (!coord_2.has_value()) {
            board.replaceCell(coord_1.value(), BLANK_CELL);  // undo the first portal
            return false;
        }
        board.portals.emplace(coord_1.value(), coord_2.value());
    } else if (chosenPowerup == "Double-Jump" && !attemptMove({{0, 2}, {0, -2}, {2, 0}, {-2, 0}})) {
        return false;
    } else if (chosenPowerup == "Destroyer" && !editCoord("Which barrier would you like to destroy?", BARRIER_CELL, REGULAR_CELL).has_value()) {
        return false;
    }
    inventory.at(turn).erase(inventory.at(turn).begin() + choice - 1);
    return true;
}

/**
 * @brief Prompts the user to save their score and saves it in the scores file
 */
void Game::scoreSave() {
    if (confirm("Would you like to save the score?")) {
        std::vector<std::vector<std::string>> scores = import2D(SCORESPATH);
        std::string scoreName = getValidString("Enter your names (c to cancel): ", 1, 20, "c", std::nullopt, std::set<char>{',', '\n'}).value();
        scores.push_back({scoreName, std::to_string(settings.length), std::to_string(settings.width), std::to_string(settings.numDots), std::to_string(turnNumber)});
        export2D(SCORESPATH, scores);
    }
}

/**
 * @brief Main game loop - plays the game until a player wins or concedes
 */
void Game::play() {
    while (true) {
        if (turnNumber % settings.powerupPlacementFrequency == 0) {
            board.placePowerup();
        }
        board.show();
        std::cout << std::format("Player {}'s turn \t\t\tTurn: {}", turn, turnNumber) << std::endl;
        int option = getValidInt("What would you like to do? \n1) Move\n2) Delete a space\n3) Create a space\n4) Use a Powerup\n5) Concede", 1, 5);
        // Clear the input buffer after reading the integer input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //                                  D       A        S        W
        if (option == 1 && !attemptMove({{0, 1}, {0, -1}, {1, 0}, {-1, 0}})) {
            continue;
        } else if (option == 2 && !handleDeleteCreate("delete", deletes, "Which space would you like to delete?", REGULAR_CELL, BLANK_CELL)) {
            continue;
        } else if (option == 3 && !handleDeleteCreate("create", creates, "Which space would you like to create?", BLANK_CELL, REGULAR_CELL)) {
            continue;
        } else if (option == 4 && !usePowerup()) {
            continue;
        } else if (option == 5) {
            if (confirm("Are you sure you want to concede?")) {
                std::cout << "Player " << turn << " has conceded." << std::endl;
                break;
            } else {
                continue;
            }
        }
        if (checkDefeat()) {
            board.show();
            std::cout << "Player " << turn << " has won!" << std::endl;
            break;
        }
        turn = 3 - turn;
        turnNumber += 1;
    }
    std::cout << std::format("Player {} has won in {} turns!", turn, turnNumber) << std::endl;
    scoreSave();
}

/**
 * @brief Gets the character of the current player
 */
Cell Game::getTargetCell() const {
    return DOT_CELLS.at(3 - turn);
}

/**
 * @brief Gets the character of the current opponent
 */
Cell Game::getAllyCell() const {
    return DOT_CELLS.at(turn);
}