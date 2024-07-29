# Enforce CMake version
cmake_minimum_required(VERSION 3.10)

# Set the policy CMP0079 to NEW
cmake_policy(SET CMP0079 NEW)

# Define project
project(dotto_cpp LANGUAGES CXX)

# Enforce C++ 20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands (used in VSCode linting)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Make the directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Look for spdlog library
find_package(spdlog REQUIRED)

# Add subdirectory and execute CMakeLists.txt in that directory
add_subdirectory(src)

# Add include directory
target_include_directories(dotto-cpp.exe BEFORE PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link against spdlog
target_link_libraries(dotto-cpp.exe PRIVATE spdlog::spdlog_header_only)
