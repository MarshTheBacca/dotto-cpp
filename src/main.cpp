#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>

#include "game.h"
#include "globals.h"
#include "other_tools.h"
#include "settings_data.h"
#include "validation_tools.h"

using LoggerPtr = std::shared_ptr<spdlog::logger>;

/**
 * @brief Initialises the logger by creating a file sink and a console sink
 */
LoggerPtr initialiseLogger() {
    // Create a file sink and a console sink with different names for clarity
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::filesystem::path("./dotto-cpp.log"), true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // Combine the sinks into a multi-sink logger
    auto logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{file_sink, console_sink});
    spdlog::register_logger(logger);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    // Set the default log level to info
    logger->set_level(spdlog::level::info);
    return logger;
}

/**
 * @brief Main function of the program
 */
int main() {
    LoggerPtr logger = initialiseLogger();
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