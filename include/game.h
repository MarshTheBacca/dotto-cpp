#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "board.h"
#include "player.h"
#include "settings_data.h"

struct Game {
    const SettingsData settings;                              // Settings of the game
    Board board;                                              // Game board
    const std::shared_ptr<Player> player1;                    // Player 1
    const std::shared_ptr<Player> player2;                    // Player 2
    std::set<std::pair<int, int>> crumbliesCoords;            // Crumblies coordinates
    const std::set<std::pair<int, int>> powerupSourceCoords;  // Powerup source coordinates
    std::set<std::pair<int, int>> barrierCoords;              // Barrier coordinates
    std::set<Portal> portals{};                               // Portal objects

    int turnNumber{1};       // Current turn number
    int currentPlayerID{1};  // Current player's ID

    explicit Game(const SettingsData &settingsData);

    std::optional<std::pair<int, int>> editCoord(const std::string &prompt, const Cell &targetCell, const Cell &newCell);
    std::pair<int, int> updatePortals(const std::pair<int, int> &coord);
    void processMove(const std::pair<int, int> &origin, std::pair<int, int> &destination);

    bool checkDefeat() const;
    bool usePowerup();
    void scoreSave() const;
    void placePowerup();
    void play();

    const Cell &getTargetCell() const;
    const Cell &getTargetBishopCell() const;
    const Cell &getAllyCell() const;
    const Cell &getAllyBishopCell() const;

    const std::shared_ptr<Player> &getTargetPlayer() const;
    const std::shared_ptr<Player> &getAllyPlayer() const;
};

#endif  // GAME_H