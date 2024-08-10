#ifndef OTHER_TOOLS_H
#define OTHER_TOOLS_H

#include <filesystem>
#include <functional>
#include <map>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "board.h"
#include "cell.h"
#include "enums.h"

std::string coordToString(const std::pair<int, int> &coord);
std::pair<int, int> stringToCoord(const std::string &coordString);
std::pair<int, int> vectorAddition(const std::pair<int, int> &vector_1, const std::pair<int, int> &vector_2);

template <typename T>
std::vector<std::vector<T>> import2DTemplate(const std::filesystem::path &path, std::function<T(const std::string &)> processCell);
std::vector<std::vector<std::string>> import2D(const std::filesystem::path &path);
std::vector<std::vector<char>> importChar2D(const std::filesystem::path &path);

std::vector<std::vector<Cell>> readMap(const Map &map);

void export2D(const std::filesystem::path &path, const std::vector<std::vector<std::string>> &data);
void showScores(const std::vector<std::vector<std::string>> &scores);
void showCoord(const std::pair<int, int> &coord);
void showMoves(const std::map<char, std::pair<int, int>, std::less<>> &moves);
std::string verboseCoord(const std::pair<int, int> &coord);

Map getValidMap();
Powerup generateRandomPowerup();

// template functions must be defined in the header file

/**
 * @brief Removes an element from a vector
 * @tparam T The type of the elements
 * @param vector The vector to remove the element from
 * @param value The element to remove
 */
template <typename T>
void vectorRemove(std::vector<T> &vector, const T &value) {
    std::erase(vector, value);
}

#endif  // OTHER_TOOLS_H