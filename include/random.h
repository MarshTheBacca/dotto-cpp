#ifndef RANDOM_H
#define RANDOM_H

#include <random>

/**
 * @brief A class to generate random numbers using the Mersenne Twister algorithm
 * and avoids repeated code for random number generation
 */
class Random {
   public:
    Random();

    int getInt(int min, int max);
    float getFloat(float min, float max);

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