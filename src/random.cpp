#include "random.h"

// Define the static member variable
Random Random::instance;

/**
 * @brief Construct a new Random::Random object
 * Initialize the random number generator with a seed
 * The seed is generated using std::random_device
 */
Random::Random() : generator(std::random_device{}()) {}

/**
 * @brief Get the singleton instance
 * @return The singleton instance
 */
Random& Random::getInstance() {
    return instance;
}

/**
 * @brief Get a random integer
 * @param min The minimum value
 * @param max The maximum value
 * @return A random integer
 */
int Random::getInt(int min, int max) {
    std::uniform_int_distribution dist(min, max);
    return dist(generator);
}

/**
 * @brief Get a random float
 * @param min The minimum value
 * @param max The maximum value
 * @return A random float
 */
float Random::getFloat(float min, float max) {
    std::uniform_real_distribution dist(min, max);
    return dist(generator);
}