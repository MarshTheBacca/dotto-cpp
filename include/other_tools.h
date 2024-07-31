#ifndef OTHER_TOOLS_H
#define OTHER_TOOLS_H

#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

std::string coordToString(const std::pair<int, int> &coord);
std::pair<int, int> stringToCoord(const std::string &coordString);
std::pair<int, int> vectorAddition(const std::pair<int, int> &vector_1, const std::pair<int, int> &vector_2);
std::vector<std::vector<std::string>> import2D(const std::filesystem::path &path);
void export2D(const std::filesystem::path &path, const std::vector<std::vector<std::string>> &data);
void showScores(const std::vector<std::vector<std::string>> &scores);
void showCoord(const std::pair<int, int> &coord);
void showMoves(const std::map<char, std::pair<int, int>, std::less<>> &moves);
std::string verboseCoord(const std::pair<int, int> &coord);

#endif  // OTHER_TOOLS_H