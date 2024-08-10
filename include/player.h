#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "board.h"
#include "cell.h"
#include "enums.h"
#include "piece.h"

struct Player {
    const int id;
    const Cell cell;
    const Cell bishopCell;

    std::set<Piece> pieces = {};
    std::vector<Powerup> inventory = {};

    Player(const int id, const Cell &cell, const Cell &bishopCell, const std::set<Piece> &pieces);
    void addPowerup(const Powerup &powerup);
    void removePowerup(const Powerup &powerup);
    void addPiece(const Piece &piece);
    void removePiece(const Piece &piece);
    void removePiece(const std::pair<int, int> &coord);
    void updatePiece(Piece &piece, const std::pair<int, int> &newCoord);
    void updatePiece(const std::pair<int, int> &coord, std::pair<int, int> &newCoord);

    bool hasPowerup(const Powerup &powerup) const;
    bool hasPowerups() const;
    std::optional<Powerup> selectPowerup() const;

    std::optional<Piece> selectPiece() const;
    std::optional<std::pair<int, int>> getDestination(const Board &board,
                                                      const std::pair<int, int> &origin,
                                                      const std::pair<int, int> &vector) const;
    std::map<DirectionData, std::pair<int, int>> detectMoves(const Board &board, const Piece &piece, const bool isHop) const;

    std::optional<std::pair<int, int>> selectDestination(const std::map<DirectionData, std::pair<int, int>> &moves) const;
    std::optional<std::pair<std::pair<int, int>, std::pair<int, int>>> attemptMove(const Board &board, const bool isHop) const;
    bool upgradePiece(const Piece &piece);
};

#endif  // PLAYER_H