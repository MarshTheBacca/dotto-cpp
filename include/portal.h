#ifndef PORTAL_H
#define PORTAL_H

#include <utility>  // for the pair class

/**
 * @brief A struct to represent a portal pair
 */
struct Portal {
    std::pair<int, int> coord_1;  // first portal coordinate
    std::pair<int, int> coord_2;  // second portal coordinate

    /**
     * @brief Construct a new Portal object
     * @param coord_1 The first coordinate of the portal pair
     * @param coord_2 The second coordinate of the portal pair
     */
    Portal(std::pair<int, int> coord_1, std::pair<int, int> coord_2)
        : coord_1(coord_1), coord_2(coord_2){};

    /**
     * @brief Identify if a coordinate is a member of the portal pair
     * @param coord The coordinate to check
     * @return True if the coordinate is a member of the portal pair, false
     * otherwise
     */
    inline bool isMember(std::pair<int, int> coord) const {
        return coord == coord_1 || coord == coord_2;
    };

    /**
     * @brief Get the opposite coordinate of the portal pair
     * @param coord The coordinate to get the opposite of
     * @return The opposite coordinate
     */
    inline std::pair<int, int> getOpposite(std::pair<int, int> coord) const {
        return coord == coord_2 ? coord_1 : coord_2;
    }

    /**
     * @brief operator== overload to compare two portals
     * @param other The other portal to compare to
     * @return True if the portals are equal, false otherwise
     */
    // we can use the default implementation of operator== because we only have
    // two member variables
    bool operator==(const Portal &other) const = default;

    /**
     * @brief operator< overload to compare two portals
     * @param other The other portal to compare to
     * @return True if this portal is less than the other portal, false otherwise
     */
    bool operator<(const Portal &other) const {
        return coord_1 < other.coord_1 || (coord_1 == other.coord_1 && coord_2 < other.coord_2);
    };
};

#endif  // PORTAL_H