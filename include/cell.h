#ifndef CELL_H
#define CELL_H

#include <string>

// define colours
static const std::string RED = "\033[31m";
static const std::string BLUE = "\033[34m";
static const std::string GREEN = "\033[32m";
static const std::string PINK = "\033[95m";
static const std::string PURPLE = "\033[35m";
static const std::string RESET = "\033[0m";  // reset character to disable colour

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

Cell charToCell(const char &character);

static const Cell BLANK_CELL = Cell(' ', RESET);
static const Cell REGULAR_CELL = Cell('/', RESET);
static const Cell PLAYER_1_CELL = Cell('O', BLUE);
static const Cell PLAYER_2_CELL = Cell('O', RED);
static const Cell POWERUP_SOURCE_CELL = Cell('S', PINK);
static const Cell BARRIER_CELL = Cell('#', RESET);
static const Cell CRUMBLY_CELL = Cell('~', RESET);
static const Cell PORTAL_CELL = Cell('@', PURPLE);
static const Cell HOP_CELL = Cell('H', PINK);
static const Cell PORTAL_POWER_CELL = Cell('P', PINK);
static const Cell DESTROYER_CELL = Cell('D', PINK);
static const Cell BISHOP_POWER_CELL = Cell('B', PINK);
static const Cell BISHOP_1_CELL = Cell('!', BLUE);
static const Cell BISHOP_2_CELL = Cell('!', RED);

#endif  // CELL_H