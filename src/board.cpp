#include "board.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <random>
#include <ranges>

#include "random.h"

const auto TRIANGLENUMS = std::vector<int>{1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105};
const auto LETTERS = std::vector<char>{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
/**
 * @brief Generates a random coordinate
 * @param length The length of the field
 * @param width The width of the field
 * @return A random coordinate
 */
std::pair<int, int> generateRandomCoord(const int length, const int width) {
    return std::pair<int, int>(
        Random::getInstance().getInt(0, length - 1),
        Random::getInstance().getInt(0, width - 1));
}
/**
 * @brief Rotates the field 180 degrees (in-place)
 * @param field The field to rotate
 */
void rotateField(std::vector<std::vector<char>>& field) {
    std::vector<std::vector<char>> newField(field.size(), std::vector<char>(field[0].size()));
    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field[0].size(); j++) {
            newField[field.size() - i - 1][field[0].size() - j - 1] = field[i][j];
        }
    }
    field = newField;
}

/**
 * @brief Places dots on the field (in-place)
 * @param field The field to place the dots on
 * @param dotChar The character to use for the dots
 * @param dotsToPlace The number of dots to place
 */
void placeDots(std::vector<std::vector<char>>& field, const char& dotChar, int dotsToPlace) {
    int maxNumDotsInRow = 0;
    for (int i = 0; i < TRIANGLENUMS.size(); i++) {
        if (dotsToPlace <= TRIANGLENUMS[i]) {
            maxNumDotsInRow = i + 1;
            break;
        }
    }
    std::ranges::for_each(field, [&](auto& row) {
        // Pipe operator essentially truncates the row to the first maxNumDotsInRow elements
        std::ranges::for_each(row | std::views::take(maxNumDotsInRow), [&](auto& cell) {
            if (dotsToPlace != 0) {
                cell = dotChar;
                dotsToPlace--;
            }
        });
        maxNumDotsInRow--;
    });
}

/**
 * @brief Randomly replaces characters in the field (in-place)
 * @param field The field to replace characters in
 * @param numToReplace The number of characters to replace
 * @param oldChar The character to replace
 * @param newChar The character to replace with
 * @note This function is not guaranteed to replace the exact number of
 * characters if no free spaces are available
 */
std::vector<std::pair<int, int>> randomReplace(std::vector<std::vector<char>>& field,
                                               int numToReplace, const char& oldChar,
                                               const char& newChar) {
    const int MAXITERATIONS = 100;
    int iterations = 0;
    std::vector<std::pair<int, int>> returnCoords;

    while (iterations < MAXITERATIONS && numToReplace > 0) {
        auto [x, y] = generateRandomCoord(field.size(), field[0].size());
        // We can initialise col here because it is only needed in the if statement
        if (field[x][y] == oldChar) {
            field[x][y] = newChar;
            returnCoords.push_back({x, y});
            numToReplace--;
        }
        iterations++;
    }
    return returnCoords;
}

/**
 * @brief checks if a barrier can be placed on the field
 * @param field The field to check
 * @param baseCoord The base coordinate to place the barrier
 * @param barrierShape The shape of the barrier
 * @return True if the barrier can be placed, false otherwise
 */
bool canPlaceBarrier(std::vector<std::vector<char>> const& field, std::pair<int, int> const& baseCoord,
                     std::vector<std::pair<int, int>> const& barrierShape) {
    bool canPlace = true;
    std::ranges::for_each(barrierShape, [&](const auto& coord) {
        const auto& [dx, dy] = coord;
        int x = baseCoord.first + dx;
        int y = baseCoord.second + dy;
        // If the coordinates are out of bounds or the cell is not empty, return false
        if (x < 0 || x >= field.size() || y < 0 || y >= field[0].size() || field[x][y] != '/') {
            canPlace = false;
            return false;
        }
        return;
    });
    return true;
}

/**
 * @brief Places a barrier on the field (in-place)
 * @param field The field to place the barrier on
 * @param baseCoord The base coordinate to place the barrier
 * @param barrierShape The shape of the barrier
 */
void placeBarrier(std::vector<std::vector<char>>& field, const std::pair<int, int>& baseCoord, const std::vector<std::pair<int, int>>& barrierShape, std::vector<std::pair<int, int>>& barrierCoords) {
    std::ranges::for_each(barrierShape, [&](const auto& coord) {
        const auto& [dx, dy] = coord;
        int x = baseCoord.first + dx;
        int y = baseCoord.second + dy;
        field[x][y] = '#';
        barrierCoords.push_back({x, y});
    });
}

/**
 * @brief Places barriers on the field
 * @param field The field to place the barriers on
 * @param settings The settings data
 * @return A vector of coordinates where the barriers were placed
 * @note This function is not guaranteed to place the exact number of
 * barriers if no free spaces are available
 */
std::vector<std::pair<int, int>> placeBarriers(std::vector<std::vector<char>> field,
                                               SettingsData const& settings) {
    //   #       #     #     #
    //   # # #   # #   #   # #
    //           #     #
    std::vector<std::vector<std::pair<int, int>>> barrierLayouts = {{{0, 0}, {1, 0}, {0, 1}, {0, 2}},
                                                                    {{0, 0}, {1, 0}, {0, 1}, {-1, 0}},
                                                                    {{0, 0}, {1, 0}, {-1, 0}},
                                                                    {{0, 0}, {0, -1}, {1, 0}}};
    int barriersToPlace = (settings.length / settings.barrierDensity) * (settings.width / settings.barrierDensity);
    const int MAXITERATIONS = 1000;
    int numIterations = 0;
    std::vector<std::pair<int, int>> barrierCoords;

    while (barriersToPlace > 0 && numIterations < MAXITERATIONS) {
        std::pair<int, int> randCoord = generateRandomCoord(field.size(), field[0].size());
        const auto& randBarrier = barrierLayouts[Random::getInstance().getInt(0, barrierLayouts.size() - 1)];
        if (canPlaceBarrier(field, randCoord, randBarrier)) {
            placeBarrier(field, randCoord, randBarrier, barrierCoords);
            barriersToPlace--;
        }
        numIterations++;
    }
    return barrierCoords;
}

/**
 * @brief Constructs a board with the given settings
 * @param settingsData The settings data
 */
Board::Board(SettingsData const& settingsData) : field(std::vector<std::vector<char>>(settingsData.length,
                                                                                      std::vector<char>(settingsData.width, '/'))),
                                                 length(settingsData.length),
                                                 width(settingsData.width) {
    placeDots(field, 'X', settingsData.numDots);
    rotateField(field);
    placeDots(field, 'O', settingsData.numDots);
    std::vector<std::pair<int, int>> powerupCoords = randomReplace(field, settingsData.numInitialPowerups, '/', '?');
    std::vector<std::pair<int, int>> barrierCoords = placeBarriers(field, settingsData);
    std::vector<std::pair<int, int>> crumbliesCoords = randomReplace(field, settingsData.numInitialCrumblies, '/', '~');
}

/**
 * @brief Scans the field for coordinates with a certain character
 * @param targetChar The character to scan for
 * @return A vector of coordinates with the character
 */
std::vector<std::pair<int, int>> Board::scanCharCoords(const char& targetChar) const {
    std::vector<std::pair<int, int>> coords;
    // Track indexes as it is more efficient than pointer arithmetic
    int rowIndex = 0;
    // Use for_each for better readability
    std::ranges::for_each(field, [&](const auto& row) {
        int colIndex = 0;
        std::ranges::for_each(row, [&](const auto& cell) {
            if (cell == targetChar) {
                coords.emplace_back(rowIndex, colIndex);
            }
            ++colIndex;
        });
        ++rowIndex;
    });
    return coords;
}

void Board::placePowerup() {
    std::mt19937 generator(std::random_device{}());
    std::pair<int, int> coord = generateRandomCoord(length, width);
    while (field[coord.first][coord.second] != '/') {
        coord = generateRandomCoord(length, width);
    }
    field[coord.first][coord.second] = '?';
    powerups.push_back(coord);
}

/**
 * @brief Replaces a character in the field
 * @param coord The coordinate to replace
 * @param newChar The character to replace with
 */
void Board::replaceChar(std::pair<int, int> coord, const char& newChar) {
    field[coord.first][coord.second] = newChar;
}

/**
 * @brief Checks if a coordinate is within the bounds of the field
 * @param coord The coordinate to check
 * @return True if the coordinate is within bounds, false otherwise
 */
bool Board::isWithinBounds(const std::pair<int, int>& coord) const {
    return coord.first >= 0 && coord.first < length && coord.second >= 0 && coord.second < width;
}

/**
 * @brief Displays the field
 */
void Board::show() const {
    std::cout << std::endl;
    for (int i = 0; i < field.size(); i++) {
        std::cout << LETTERS[i] << "\t";
        for (int j = 0; j < field[0].size(); j++) {
            std::cout << field[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "\t";
    for (int i = 1; i <= width; i++) {
        if (width > 9 && i < 10) {
            std::cout << "0";
        }
        std::cout << i << "\t";
    }
}
