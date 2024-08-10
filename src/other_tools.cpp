#include "other_tools.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tabulate/table.hpp>
#include <utility>
#include <vector>

#include "cell.h"
#include "globals.h"  // for the EXE_PATH variable
#include "random.h"
#include "validation_tools.h"

/**
 * @brief Converts a pair of integers to a string in the format "A1", "B2", etc.
 * @param coord The pair of integers to convert
 */
std::string coordToString(const std::pair<int, int>& coord) {
    int colNum = coord.first;
    std::vector<char> colParts;
    while (colNum >= 0) {
        colParts.push_back('A' + colNum % 26);
        colNum = colNum / 26 - 1;  // C++ rounds to nearest integer for integer division
    }
    std::string colStr(colParts.rbegin(), colParts.rend());
    return std::format("{}{}", coord.second + 1, colStr);
}

/**
 * @brief Converts a string in the format "A1", "B2", etc. to a pair of integers
 * @param coordStr The string to convert
 * @return The pair of integers
 */
std::pair<int, int> stringToCoord(const std::string& coordStr) {
    // Extract row part (digits)
    std::string rowStr;
    std::ranges::copy_if(coordStr, std::back_inserter(rowStr), ::isdigit);

    // Extract column part (alphabetic characters)
    std::string colStr;
    std::ranges::copy_if(coordStr, std::back_inserter(colStr), ::isalpha);

    // Error handling for invalid input
    if (rowStr.empty() || colStr.empty()) {
        throw std::invalid_argument("Invalid coordinate string: " + coordStr);
    }

    // Convert row part to integer and adjust
    const int rowNum = std::stoi(rowStr) - 1;

    // Convert column part to integer
    std::ranges::transform(colStr, colStr.begin(), ::toupper);
    const int colNum = std::accumulate(colStr.begin(), colStr.end(), 0, [](int sum, const char& currentChar) {
                           return sum * 26 + (currentChar - 'A' + 1);
                       }) -
                       1;

    return {colNum, rowNum};
}

/**
 * @brief Adds two pairs of integers together
 * @param vector_1 The first pair of integers
 * @param vector_2 The second pair of integers
 * @return The sum of the two pairs
 */
std::pair<int, int> vectorAddition(const std::pair<int, int>& vector_1, const std::pair<int, int>& vector_2) {
    return {vector_1.first + vector_2.first, vector_1.second + vector_2.second};
}

/**
 * @brief Imports a 2D vector from a CSV file
 * @param path The path to the CSV file
 * @param processCell A function to process each cell in the CSV file
 * @return The 2D vector
 */
template <typename T>
std::vector<std::vector<T>> import2DTemplate(const std::filesystem::path& path, std::function<T(const std::string&)> processCell) {
    std::vector<std::vector<T>> result;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Could not open file, returning empty vector: " << path << std::endl;
        return result;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<T> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(processCell(cell));
        }
        result.push_back(row);
    }
    file.close();
    return result;
}

/**
 * @brief Imports a 2D vector of strings from a CSV file
 * @param path The path to the CSV file
 * @return The 2D vector of strings
 */
std::vector<std::vector<std::string>> import2D(const std::filesystem::path& path) {
    return import2DTemplate<std::string>(path, [](const std::string& cell) {
        return cell;
    });
}

/**
 * @brief Imports a 2D vector of characters from a CSV file
 * @param path The path to the CSV file
 * @return The 2D vector of characters
 */
std::vector<std::vector<char>> importChar2D(const std::filesystem::path& path) {
    return import2DTemplate<char>(path, [](const std::string& cell) {
        return cell[0];
    });
}

/**
 * @brief Converts a map type to a string by reading from a file
 * @param mapType The map type to convert
 * @return The vector form of the map (field)
 */
std::vector<std::vector<Cell>> readMap(const Map& mapType) {
    const std::filesystem::path mapPath = EXE_PATH / std::format("maps/{}.csv", mapToString(mapType));
    auto map = importChar2D(mapPath);
    std::vector<std::vector<Cell>> field;
    field.reserve(map.size());  // Preallocate space for the outer vector
    for (const auto& row : map) {
        std::vector<Cell> fieldRow;
        fieldRow.reserve(row.size());  // Preallocate space for the inner vector
        for (const auto& character : row) {
            fieldRow.push_back(charToCell(character));
        }
        field.push_back(fieldRow);
    }
    return field;
}

/**
 * @brief Exports a 2D vector to a CSV file
 * @param path The path to the CSV file
 * @param array The 2D vector to export (must be a vector of vectors of strings)
 */
void export2D(const std::filesystem::path& path, const std::vector<std::vector<std::string>>& array) {
    std::ofstream file(path, std::ios::app);  // std::ios::app will create the file if it does not exist and append to it
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path.string());
    }
    for (const auto& row : array) {
        for (std::size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ",";  // only add comma if not the last element
            }
        }
        file << "\n";  // doesn't matter if last line is empty
    }
    file.close();
}

/**
 * @brief Displays a 2D vector of strings as a table
 * @param scores The 2D vector of strings to display
 * @note Uses the tabulate library to display the table
 */
void showScores(const std::vector<std::vector<std::string>>& scores) {
    tabulate::Table table;
    table.add_row({"Name", "Length", "Width", "Dots", "Turns"});
    std::ranges::for_each(scores, [&table](const auto& row) {
        table.add_row({row.begin(), row.end()});
    });
    std::cout << table << std::endl;
}

/**
 * @brief Converts a pair of integers to a verbose string in the format "(0, 0) : A1"
 * @param coord The pair of integers to convert
 * @return The verbose string
 */
std::string verboseCoord(const std::pair<int, int>& coord) {
    return std::format("({}, {}) : {} ", coord.first, coord.second, coordToString(coord));
}

/**
 * @brief Displays a pair of integers as a verbose string in the format "(0, 0) : A1"
 * @param coord The pair of integers to display
 * @note Uses the verboseCoord function to convert the pair to a string
 */
void showCoord(const std::pair<int, int>& coord) {
    std::cout << verboseCoord(coord) << std::endl;
}

/**
 * @brief Displays a map of characters to pairs of integers as a list of moves
 * @param moves The map of characters to pairs of integers to display
 * @note Uses the verboseCoord function to convert the pairs to strings
 */
void showMoves(const std::map<char, std::pair<int, int>, std::less<>>& moves) {
    std::cout << "Possible moves:\n";
    for (const auto& [key, value] : moves) {
        std::cout << key << " : " << verboseCoord(value) << "\n";
    }
}

/**
 * @brief Prompts the user to choose a map using an integer menu
 * @returns The map type
 */
Map getValidMap() {
    std::string prompt = "Choose a map:";
    for (int i = 0; i < static_cast<int>(Map::COUNT); i++) {
        prompt += std::format("\n{}) {}", i + 1, mapToString(static_cast<Map>(i)));
    }
    return static_cast<Map>(getValidInt(prompt, 1, static_cast<int>(Map::COUNT)) - 1);
}

/**
 * @brief Generates a random powerup
 * @return A random powerup
 */
Powerup generateRandomPowerup() {
    return static_cast<Powerup>(Random::getInstance().getInt(0, static_cast<int>(Powerup::COUNT) - 1));
}
