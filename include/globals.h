#ifndef GLOBALS_H
#define GLOBALS_H

#include <atomic>
#include <filesystem>

std::filesystem::path getExePath();

extern const std::filesystem::path EXE_PATH;

// Declare the global path to the scores file
extern const std::filesystem::path SCORESPATH;

// Declare the global exit flag
extern const std::atomic<bool> exitFlag;

#endif  // GLOBALS_H