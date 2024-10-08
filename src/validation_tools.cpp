#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

#include "other_tools.h"

/**
 * @brief Trim leading and trailing whitespace from a string
 * @param str The string to trim
 * @return The trimmed string
 */
std::string trimWhite(const std::string &str) {  // cant be std::string_view because we compare it to std::string::npos
                                                 // which is size_t
    // Find start and end of non-whitespace characters
    const auto start = str.find_first_not_of(" \t\n\r\f\v");
    const auto end = str.find_last_not_of(" \t\n\r\f\v");
    // Return substring from start to end
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * @brief Prompt input from the user and trim whitespace
 * @return The trimmed input as a string
 */
std::string getCleanInput() {
    std::string input;
    std::getline(std::cin, input);
    return trimWhite(input);
}

/**
 * @brief Get a valid integer from the user
 * @param prompt The prompt to display to the user (no need for newline)
 * @param lower The lower bound of the valid range
 * @param upper The upper bound of the valid range
 * @return The valid integer
 */
int getValidInt(const std::string &prompt,
                const int lower = std::numeric_limits<int>::min(),
                const int upper = std::numeric_limits<int>::max()) {
    int value;
    while (true) {
        std::cout << prompt << "\n";
        const std::string input = getCleanInput();
        std::stringstream ss(input);
        // .eof() checks if the stream is at the end, ie, if the entire input was read
        // the >> operator will evaluate to false if parsing the input into an integer fails
        if (ss >> value && ss.eof() && value >= lower && value <= upper) {
            return value;
        } else {
            std::cout << "Invalid input. Please enter a valid integer.\n";
        }
    }
}

/**
 * @brief Get a yes or no answer from the user, no option to cancel
 * @param prompt The prompt to display to the user
 * @return True if the user enters 'y', false if the user enters 'n'
 */
bool confirm(const std::string &prompt) {
    // Create an unordered set that uses our custom hash and equality functions
    // They enable heterogeneous lookup for std::string and std::string_view,
    // allowing for transparent comparison between the two types
    const std::unordered_set<char> validInputs = {'y', 'n'};
    while (true) {
        std::cout << prompt << " (y/n):\n";
        const std::string input = getCleanInput();
        if (input.size() != 1) {
            std::cout << "Invalid input. Please enter a single character (y or n)." << std::endl;
            continue;
        }
        // Convert input to lowercase
        const auto inputChar = static_cast<char>(std::tolower(static_cast<unsigned char>(input[0])));
        if (validInputs.contains(inputChar)) {
            return inputChar == 'y';
        } else {
            std::cout << "Invalid input. Please enter y or n." << std::endl;
        }
    }
}

/**
 * @brief Get a valid string from the user
 * @param prompt The prompt to display to the user
 * @param lower The lower bound of the valid length of input
 * @param upper The upper bound of the valid length of input
 * @param cancelString The string that cancels the input
 * @param accepted The set of accepted characters
 * @param forbidden The set of forbidden characters
 * @return The valid string or nullopt if the user cancels
 */
std::optional<std::string> getValidString(const std::string &prompt, const int lower, const int upper,
                                          const std::optional<std::string_view> &cancelString = std::nullopt,
                                          const std::optional<std::set<char>> &accepted = std::nullopt,
                                          const std::optional<std::set<char>> &forbidden = std::nullopt) {
    while (true) {
        std::cout << prompt << "\n";
        const std::string input = getCleanInput();
        if (cancelString.has_value() && input == cancelString.value()) {
            return std::nullopt;
        } else if (input.empty()) {
            std::cout << "Input cannot be empty. Please enter a string." << std::endl;
        } else if (input.size() < lower || input.size() > upper) {
            std::cout << "Input must be between " << lower << " and " << upper
                      << " characters." << std::endl;
        }
        // check if the input contains non-accepted characters
        else if (accepted.has_value() &&
                 std::ranges::any_of(input, [&accepted](char c) { return (!accepted->contains(c)); })) {
            std::cout << "Input contains forbidden characters." << std::endl;
        }
        // check if the input contains forbidden characters
        else if (forbidden.has_value() && std::ranges::any_of(input, [&forbidden](char c) { return (forbidden->contains(c)); })) {
            std::cout << "Input contains forbidden characters." << std::endl;
        } else {
            return input;
        }
    }
}

/**
 * @brief Get a valid coordinate from the user
 * @param prompt The prompt to display to the user
 * @param length The length of the board
 * @param width The width of the board
 * @return The valid coordinate or nullopt if the user cancels
 */
std::optional<std::pair<int, int>> getValidCoord(const std::string &prompt, int length, int width) {
    while (true) {
        std::cout << prompt << ". Enter coordinate (e.g. 1A, 'c' to cancel):\n";
        // skip all characters until a newline is found
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        const std::string coord = getCleanInput();
        if (coord == "c") {
            return std::nullopt;
        }
        if (!std::regex_match(coord, std::regex(R"(^\d+[A-Z]+$)"))) {
            std::cout << "Invalid coordinate format\n";
            continue;
        }
        const auto [x, y] = stringToCoord(coord);
        if (x < 0 || x >= width || y < 0 || y >= length) {
            std::cout << "Coordinate is out of bounds\n";
            continue;
        }
        return std::make_pair(x, y);
    }
}
