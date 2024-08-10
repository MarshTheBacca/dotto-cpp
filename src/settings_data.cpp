#include "settings_data.h"

#include <functional>
#include <iostream>
#include <map>
#include <tabulate/table.hpp>

#include "other_tools.h"
#include "validation_tools.h"

/**
 * @brief Prompts the user to edit the settings and updates any changes
 */
void SettingsData::edit() {
    std::map<int, std::function<void()>> actions = {
        {1, [this]() { this->map = getValidMap(); }},
        {2, [this]() { this->length = getValidInt("Enter the new length", 5, 15); }},
        {3, [this]() { this->width = getValidInt("Enter the new width", 5, 15); }},
        {4, [this]() { this->numDots = getValidInt("Enter the new number of dots", 3, 10); }},
        {5, [this]() { this->numInitialPowerups = getValidInt("Enter the new number of initial powerups", 5, 10); }},
        {6, [this]() { this->powerupPlacementFrequency = getValidInt("Enter the new powerup placement frequency", 3, 10); }},
        {7, [this]() { this->numInitialCrumblies = getValidInt("Enter the new number of initial crumblies", 3, 10); }},
        {8, [this]() { this->barrierDensity = getValidInt("Enter the new barrier density", 4, 10); }},
        {9, [this]() { this->numDeletes = getValidInt("Enter the new number of deletes", 3, 10); }},
        {10, [this]() { this->numCreates = getValidInt("Enter the new number of creates", 3, 10); }}};

    while (true) {
        tabulate();
        const int option = getValidInt("What would you like to edit? (11 to exit)", 1, 11);
        if (option == 11) {
            break;
        }
        auto it = actions.find(option);
        it->second();
    }
}

/**
 * @brief Prints the settings to the console in a tabulated format
 * @note Uses the tabulate library
 */
void SettingsData::tabulate() const {
    tabulate::Table table;
    table.add_row({"Number", "Name", "Value"});
    table.add_row({"1", "Map", mapToString(map)});
    table.add_row({"2", "Length", std::to_string(length)});
    table.add_row({"3", "Width", std::to_string(width)});
    table.add_row({"4", "Number of Dots", std::to_string(numDots)});
    table.add_row({"5", "Number of Initial Powerups", std::to_string(numInitialPowerups)});
    table.add_row({"6", "Powerup Placement Frequency", std::to_string(powerupPlacementFrequency)});
    table.add_row({"7", "Number of Initial Crumblies", std::to_string(numInitialCrumblies)});
    table.add_row({"8", "Barrier Density", std::to_string(barrierDensity)});
    table.add_row({"9", "Number of Deletes", std::to_string(numDeletes)});
    table.add_row({"10", "Number of Creates", std::to_string(numCreates)});
    std::cout << table << std::endl;
};