#include "globals.h"

// Define the global path to the scores file
const std::filesystem::path SCORESPATH = std::filesystem::path("./scores.csv");

// Global exit flag to cleanly exit when we use ctrl + c
const std::atomic<bool> exitFlag(false);