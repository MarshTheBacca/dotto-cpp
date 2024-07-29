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
#include <transparent.h>
#include <unordered_set>

/**
 * @brief Trim leading and trailing whitespace from a string
 * @param str The string to trim
 * @return The trimmed string
 */
std::string trimWhite(const std::string &str) {
  // Find start and end of non-whitespace characters
  auto start = str.find_first_not_of(" \t\n\r\f\v");
  auto end = str.find_last_not_of(" \t\n\r\f\v");
  // Return substring from start to end
  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * @brief Get a valid integer from the user
 * @param prompt The prompt to display to the user
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
    std::cin >> value;
    if (std::cin.fail() || value < lower || value > upper) {
      std::cin.clear(); // clear the error flag
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // discard all characters until a newline is found
      std::cout << "Invalid input. Please enter a valid integer.\n";
    } else {
      return value;
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
  // allowing for transparent conversion between the two types
  const std::unordered_set<std::string, TransparentHasher, TransparentEqual>
      validInputs = {"y", "n"};

  while (true) {
    std::cout << prompt << " (y/n):\n";
    std::string input;
    std::getline(std::cin, input);

    // Trim leading and trailing whitespace
    input = trimWhite(input);

    if (input.empty()) {
      std::cout << "Input cannot be empty. Please enter y or n." << std::endl;
      continue;
    }

    // Convert input to lowercase
    std::ranges::transform(input, input.begin(),
                           [](unsigned char c) { return std::tolower(c); });

    if (validInputs.contains(input)) {
      return input == "y";
    } else {
      std::cout << "Invalid input. Please enter y or n." << std::endl;
    }
  }
}

/**
 * @brief Get a valid string from the user
 * @param prompt The prompt to display to the user
 * @param lower The lower bound of the valid range
 * @param upper The upper bound of the valid range
 * @param accepted The set of accepted characters
 * @param cancelString The string that cancels the input
 * @return The valid string or nullopt if the user cancels
 */
std::optional<std::string> getValidString(
    const std::string &prompt, const int lower, const int upper,
    const std::optional<std::string_view> &cancelString = std::nullopt,
    const std::optional<std::set<char>> &accepted = std::nullopt,
    const std::optional<std::set<char>> &forbidden = std::nullopt) {
  while (true) {
    std::cout << prompt << "\n";
    std::string input;
    std::getline(std::cin, input);

    // Trim leading and trailing whitespace
    input = trimWhite(input);

    if (input.empty()) {
      std::cout << "Input cannot be empty. Please enter a string." << std::endl;
      continue;
    }

    if (cancelString != std::nullopt && input == cancelString) {
      return std::nullopt;
    }

    if (input.size() < lower || input.size() > upper) {
      std::cout << "Input must be between " << lower << " and " << upper
                << " characters." << std::endl;
      continue;
    }
    // If the user provided a set of accepted characters, check if the input
    // contains any characters not in the set
    if (accepted != std::nullopt &&
        std::ranges::any_of(
            input, [&accepted](char c) { return (!accepted->contains(c)); })) {
      std::cout << "Input contains forbidden characters." << std::endl;
      continue;
    }

    // If the user provided a set of forbidden characters, check if the input
    // contains any characters in the set
    if (forbidden != std::nullopt &&
        std::ranges::any_of(
            input, [&forbidden](char c) { return (forbidden->contains(c)); })) {
      std::cout << "Input contains forbidden characters." << std::endl;
      continue;
    }
    return input;
  }
}

/**
 * @brief Convert a string coordinate to a pair of integers
 * @param coordString The string coordinate to convert
 * @return The pair of integers
 */
std::pair<int, int> stringToCoord(const std::string &coordString) {
  int number = std::stoi(
      coordString.substr(0, coordString.find_first_not_of("0123456789")));
  char letter = coordString[coordString.find_first_not_of("0123456789")];
  return {number - 1, letter - 'A'};
}

/**
 * @brief Get a valid coordinate from the user
 * @param prompt The prompt to display to the user
 * @param length The length of the board
 * @param width The width of the board
 * @param exitString The string that cancels the input
 * @return The valid coordinate or nullopt if the user cancels
 */
std::optional<std::pair<int, int>>
getValidCoord(const std::string &prompt, int length, int width,
              const std::string &exitString = "c") {
  while (true) {
    std::string coord;
    std::cout << prompt << ". Enter coordinate (e.g. 1A, '" << exitString
              << "' to cancel)\n";
    std::cin >> coord;

    if (coord == exitString) {
      return std::nullopt;
    }
    coord = trimWhite(coord);
    if (!std::regex_match(coord, std::regex(R"(^\d+[A-Z]+$)"))) {
      std::cout << "Invalid coordinate format\n";
      continue;
    }

    auto [x, y] = stringToCoord(coord);
    if (x < 0 || x >= width || y < 0 || y >= length) {
      std::cout << "Coordinate is out of bounds\n";
      continue;
    }

    return std::make_pair(x, y);
  }
}
