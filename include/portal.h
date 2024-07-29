#include <utility> // for the pair class

/**
 * @brief A struct to represent a portal pair
 */
struct Portal {
  std::pair<int, int> coord_1;
  std::pair<int, int> coord_2;

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
};