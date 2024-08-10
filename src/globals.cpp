#include "globals.h"

// Include Windows.h for getting the path of the executable on Windows
#ifdef _WIN32
#include <windows.h>
#else
// Include unistd.h for getting the path of the executable on Linux
#include <unistd.h>
#endif

std::filesystem::path getExePath() {
    std::filesystem::path exePath;
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    exePath = std::filesystem::path(buffer).parent_path();
#else
    char buffer[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (count == -1) {
        throw std::runtime_error("Failed to get executable path");
    }
    exePath = std::filesystem::path(std::string(buffer, count)).parent_path();
#endif
    return exePath;
}

const std::filesystem::path EXE_PATH = getExePath();

// Define the global path to the scores file
const std::filesystem::path SCORESPATH = std::filesystem::path("./scores.csv");

// Global exit flag to cleanly exit when we use ctrl + c
const std::atomic<bool> exitFlag(false);