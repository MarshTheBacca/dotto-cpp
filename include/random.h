#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>  // std::shuffle
#include <random>
#include <ranges>
#include <set>
#include <vector>

/**
 * @brief A class to generate random numbers using the Mersenne Twister algorithm
 * and avoids repeated code for random number generation
 */
class Random {
   public:
    Random();

    int getInt(int min, int max);
    float getFloat(float min, float max);

    /**
     * @brief Shuffle a vector
     * @tparam T The type of the elements
     */
    template <typename T>
    void shuffleVector(std::vector<T>& vector) {
        std::ranges::shuffle(vector, generator);
    }

    // define template functions inside header file
    /**
     * @brief Get a random element from a vector
     * @tparam T The type of the elements
     * @param elements The vector of elements
     * @return A random element
     */
    template <typename T>
    T getRandomElement(const std::vector<T>& elements) {
        return elements[getInt(0, elements.size() - 1)];
    }

    /**
     * @brief Get a random element from a set
     * @tparam T The type of the elements
     * @param elements The set of elements
     * @return A random element
     */
    template <typename T>
    T getRandomElement(const std::set<T>& elements) {
        // Convert to vector and call the other function
        std::vector<T> vec(elements.begin(), elements.end());
        return getRandomElement(vec);
    }

    // Public method to get singleton instance
    // static so it is only defined ONCE in the program
    static Random& getInstance();

   private:
    std::mt19937 generator;  // Mersenne Twister random number generator

    // Delete copy constructor and assignment operator to prevent copying
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    // define static member variable for the singleton instance
    static Random instance;
};

#endif  // RANDOM_H