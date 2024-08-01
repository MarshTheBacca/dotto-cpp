#include <iostream>

#include "game.h"
#include "globals.h"
#include "other_tools.h"
#include "settings_data.h"
#include "validation_tools.h"

/**
 * @brief Displays a welcome message in the console
 */
void welcomeMessage() {
    std::cout << std::string(21, '=') << "\n";
    std::cout << "  Welcome to Dotto!\n";
    std::cout << std::string(21, '=') << "\n";
}

/**
 * @brief Main function of the program
 */
int main() {
    welcomeMessage();
    auto settingsData = SettingsData();
    while (true) {
        const int option = getValidInt("What would you like to do? \n1) Play\n2) Edit settings\n3) View scores\n4) Exit", 1, 4);
        switch (option) {
            case 1: {
                Game game(settingsData);
                game.play();
                break;
            }
            case 2: {
                settingsData.edit();
                break;
            }
            case 3: {
                showScores(import2D(SCORESPATH));
                break;
            }
            case 4:
                return 0;
            default:  // Should never happen
                break;
        }
    }
}