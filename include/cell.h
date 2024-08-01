#ifndef CELL_H
#define CELL_H

#include <string>

// define colours
const std::string RED = "\033[31m";
const std::string BLUE = "\033[34m";
const std::string GREEN = "\033[32m";
const std::string PINK = "\033[95m";
const std::string PURPLE = "\033[35m";
const std::string RESET = "\033[0m";  // reset character to disable colour

struct Cell {
    char character;
    std::string colour;

    Cell() = default;
    explicit Cell(const char character, const std::string_view &colour);

    std::string repr() const;
    void update(const Cell &cell);
    void update(const char &newCharacter, const std::string_view &newColour);

    bool operator==(const Cell &cell) const = default;

    // Define the < operator to allow Cells to be in sets
    bool operator<(const Cell &other) const {
        if (character != other.character) {
            return character < other.character;
        }
        return colour < other.colour;
    }
};

#endif  // CELL_H