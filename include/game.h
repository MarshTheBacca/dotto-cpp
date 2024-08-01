#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "board.h"
#include "player.h"
#include "settings_data.h"

struct Game {
    const SettingsData settings;            // Settings of the game
    Board board;                            // Game board
    const std::shared_ptr<Player> player1;  // Player 1
    const std::shared_ptr<Player> player2;  // Player 2
    int turnNumber{1};                      // Current turn number
    int turn{1};                            // Current player's turn

    explicit Game(const SettingsData &settingsData);

    std::optional<std::pair<int, int>> editCoord(const std::string &prompt, const Cell &targetCell, const Cell &newCell);
    std::optional<std::pair<int, int>> getOrigin() const;
    std::optional<std::pair<int, int>> getDestination(const std::map<char, std::pair<int, int>, std::less<>> &moves) const;

    std::optional<std::pair<int, int>> calculateMove(const std::pair<int, int> &position, const std::pair<int, int> &vector) const;
    std::pair<int, int> updatePortals(const std::pair<int, int> &coord);
    void processMove(const std::pair<int, int> &origin, std::pair<int, int> &destination);
    std::map<char, std::pair<int, int>, std::less<>> detectMoves(const std::pair<int, int> &origin,
                                                                 const std::vector<std::pair<int, int>> &vectors) const;
    bool attemptMove(const std::vector<std::pair<int, int>> &vectors);
    bool checkDefeat() const;
    bool usePowerup();
    void scoreSave();
    void play();

    const Cell &getTargetCell() const;
    const Cell &getAllyCell() const;
    const std::shared_ptr<Player> &getTargetPlayer() const;
    const std::shared_ptr<Player> &getAllyPlayer() const;
};

#endif  // GAME_H