#include "cell.h"

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