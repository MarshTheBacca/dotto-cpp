#ifndef VALIDATION_TOOLS_H
#define VALIDATION_TOOLS_H

#include <optional>
#include <string>

int getValidInt(const std::string &prompt, const int lower, const int upper);
bool confirm(const std::string &prompt);
std::optional<std::string>
getValidString(const std::string &prompt, const int lower, const int upper,
               const std::string &cancelString,
               const std::optional<std::string> &accepted = std::nullopt,
               const std::optional<std::string> &forbidden = std::nullopt);
std::optional<std::pair<int, int>>
getValidCoord(const std::string &prompt, const int length, const int width,
              const std::string &cancelString);

#endif  // VALIDATION_TOOLS_H