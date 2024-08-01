#include "player.h"

#include <algorithm>
#include <ranges>

#include "player.h"

std::string powerupToString(Powerup powerup) {
    switch (powerup) {
        case Powerup::DOUBLE_JUMP:
            return "Double Jump";
        case Powerup::DESTORYER:
            return "Destroyer";
        case Powerup::PORTAL:
            return "Portal";
        default:
            return "Unknown";
    }
}

Player::Player(const int id, const Cell &cell, std::vector<Powerup> inventory)
    : id(id), cell(cell), inventory(std::move(inventory)) {}

void Player::addPowerup(const Powerup &powerup) {
    inventory.push_back(powerup);
}

void Player::removePowerup(const Powerup &powerup) {
    inventory.erase(std::ranges::remove(inventory, powerup).begin(), inventory.end());
}
bool Player::hasPowerup(const Powerup &powerup) const {
    return std::ranges::find(inventory, powerup) != std::ranges::end(inventory);
}

bool Player::hasPowerups() const {
    return !inventory.empty();
}