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
        if (option == 1) {
            Game game(settingsData);
            game.play();
            std::cout << "Game over!\n"
                      << std::endl;
        } else if (option == 2) {
            settingsData.edit();
        } else if (option == 3) {
            showScores(import2D(SCORESPATH));
        } else if (option == 4) {
            return 0;
        }
    }
}
