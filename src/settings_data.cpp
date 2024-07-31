#include "settings_data.h"

#include <functional>
#include <iostream>
#include <map>
#include <tabulate/table.hpp>

#include "validation_tools.h"

/**
 * @brief Prompts the user to edit the settings and updates any changes
 */
void SettingsData::edit() {
    std::map<int, std::function<void()>> actions = {
        {1, [this]() { this->length = getValidInt("Enter the new length", 5, 15); }},
        {2, [this]() { this->width = getValidInt("Enter the new width", 5, 15); }},
        {3, [this]() { this->numDots = getValidInt("Enter the new number of dots", 3, 10); }},
        {4, [this]() { this->numInitialPowerups = getValidInt("Enter the new number of initial powerups", 5, 10); }},
        {5, [this]() { this->powerupPlacementFrequency = getValidInt("Enter the new powerup placement frequency", 3, 10); }},
        {6, [this]() { this->numInitialCrumblies = getValidInt("Enter the new number of initial crumblies", 3, 10); }},
        {7, [this]() { this->barrierDensity = getValidInt("Enter the new barrier density", 4, 10); }},
        {8, [this]() { this->numDeletes = getValidInt("Enter the new number of deletes", 3, 10); }},
        {9, [this]() { this->numCreates = getValidInt("Enter the new number of creates", 3, 10); }}};

    while (true) {
        tabulate();
        int option = getValidInt("What would you like to edit? (10 to exit)", 1, 10);
        if (option == 10) {
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
    table.add_row({"1", "Length", std::to_string(length)});
    table.add_row({"2", "Width", std::to_string(width)});
    table.add_row({"3", "Number of Dots", std::to_string(numDots)});
    table.add_row({"4", "Number of Initial Powerups", std::to_string(numInitialPowerups)});
    table.add_row({"5", "Powerup Placement Frequency", std::to_string(powerupPlacementFrequency)});
    table.add_row({"6", "Number of Initial Crumblies", std::to_string(numInitialCrumblies)});
    table.add_row({"7", "Barrier Density", std::to_string(barrierDensity)});
    table.add_row({"8", "Number of Deletes", std::to_string(numDeletes)});
    table.add_row({"9", "Number of Creates", std::to_string(numCreates)});
    std::cout << table << std::endl;
};