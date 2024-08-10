#ifndef VALIDATION_TOOLS_H
#define VALIDATION_TOOLS_H

#include <optional>
#include <set>
#include <string>
#include <string_view>

int getValidInt(const std::string &prompt, const int lower, const int upper);
bool confirm(const std::string &prompt);
std::optional<std::string> getValidString(const std::string &prompt, const int lower, const int upper,
                                          const std::optional<std::string_view> &cancelString = std::nullopt,
                                          const std::optional<std::set<char>> &accepted = std::nullopt,
                                          const std::optional<std::set<char>> &forbidden = std::nullopt);
std::optional<std::pair<int, int>>
getValidCoord(const std::string &prompt, const int length, const int width);

#endif  // VALIDATION_TOOLS_H