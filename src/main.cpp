
#include "game.h"
#include "globals.h"
#include "other_tools.h"
#include "settings_data.h"
#include "validation_tools.h"

/**
 * @brief Main function of the program
 */
int main() {
    auto settingsData = SettingsData();
    while (true) {
        int option = getValidInt("What would you like to do? \n1) Play\n2) Edit settings\n3) View scores\n4) Exit", 1, 4);
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
        }
    }
}