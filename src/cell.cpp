#include "cell.h"

#include <map>
#include <stdexcept>

Cell::Cell(const char character, const std::string_view &colour) : character(character), colour(colour) {}

/**
 * @brief Returns a string representation of the cell in colour
 */
std::string Cell::repr() const {
    return colour + character + RESET;
}

/**
 * @brief Updates the cell with the character and colour of another cell
 */
void Cell::update(const Cell &cell) {
    character = cell.character;
    colour = cell.colour;
}

/**
 * @brief Updates the cell with a new character and colour
 */
void Cell::update(const char &newCharacter, const std::string_view &newColour) {
    this->character = newCharacter;
    this->colour = newColour;
}
/**
 * @brief Converts a character to a cell
 * @param character The character to convert
 * @return The cell
 */
Cell charToCell(const char &character) {
    const std::map<char, Cell> cellMap = {
        {' ', BLANK_CELL},
        {'/', REGULAR_CELL},
        {'O', PLAYER_1_CELL},
        {'X', PLAYER_2_CELL},
        {'S', POWERUP_SOURCE_CELL},
        {'#', BARRIER_CELL},
        {'~', CRUMBLY_CELL},
        {'@', PORTAL_CELL},
        {'H', HOP_CELL},
        {'P', PORTAL_POWER_CELL},
        {'D', DESTROYER_CELL},
        {'B', BISHOP_POWER_CELL},
    };
    if (!cellMap.contains(character)) {
        throw std::invalid_argument("Invalid character while converting to cell: " + std::string(1, character));
    }
    return cellMap.at(character);
}