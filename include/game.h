#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "board.h"
#include "settings_data.h"

struct Game {
    const SettingsData settings;                        // Settings of the game
    Board board;                                        // Game board
    std::map<int, std::vector<std::string>> inventory;  // Player inventories
    std::map<int, int> deletes;                         // Number of deletes left for each player
    std::map<int, int> creates;                         // Number of creates left for each player
    int turnNumber;                                     // Current turn number
    int turn;                                           // Current player's turn

    explicit Game(const SettingsData &settingsData);

    std::optional<std::pair<int, int>> editCoord(const std::string &prompt, const char &targetChar, const char &newChar);
    std::optional<std::pair<int, int>> getOrigin() const;
    std::optional<std::pair<int, int>> getDestination(const std::map<char, std::pair<int, int>, std::less<>> &moves) const;

    std::optional<std::pair<int, int>> calculateMove(const std::pair<int, int> &position, const std::pair<int, int> &vector) const;
    std::pair<int, int> updatePortals(const std::pair<int, int> &coord);
    void processMove(const std::pair<int, int> &origin, std::pair<int, int> &destination);
    std::map<char, std::pair<int, int>, std::less<>> detectMoves(const std::pair<int, int> &origin,
                                                                 const std::vector<std::pair<int, int>> &vectors) const;
    bool attemptMove(const std::vector<std::pair<int, int>> &vectors);
    bool checkDefeat() const;

    bool handleDeleteCreate(const std::string_view &actionName, std::map<int, int> &countDict, const std::string &prompt,
                            const char targetChar, const char newChar);
    bool usePowerup();
    void scoreSave();
    void play();

    char getTargetChar() const;
    char getAllyChar() const;
};

#endif  // GAME_H