#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "cell.h"

enum class Powerup {
    DOUBLE_JUMP,
    DESTORYER,
    PORTAL,

    COUNT  // Variable at the end to get the number of powerups
};

std::string powerupToString(Powerup powerup);

struct Player {
    const int id;
    const Cell cell;
    std::vector<Powerup> inventory;

    Player(const int id, const Cell &cell, std::vector<Powerup> inventory);
    void addPowerup(const Powerup &powerup);
    void removePowerup(const Powerup &powerup);
    bool hasPowerup(const Powerup &powerup) const;
    bool hasPowerups() const;
};

#endif  // PLAYER_H