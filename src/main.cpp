#include "validation_tools.h"
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// Global exit flag to cleanly exit when we use ctrl + c
const std::atomic<bool> exitFlag(false);

using LoggerPtr = std::shared_ptr<spdlog::logger>;

/**
 * @brief Initialises the logger by creating a file sink and a console sink
 */
LoggerPtr initialiseLogger() {
  // Create a file sink and a console sink with different names for clarity
  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      std::filesystem::path("./dotto-cpp.log"), true);
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  // Combine the sinks into a multi-sink logger
  auto logger = std::make_shared<spdlog::logger>(
      "multi_sink", spdlog::sinks_init_list{file_sink, console_sink});
  spdlog::register_logger(logger);
  logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

  // Set the default log level to info
  logger->set_level(spdlog::level::info);
  return logger;
}

int main() {
  LoggerPtr logger = initialiseLogger();
  logger->info("Hello World");
  std::optional<std::pair<int, int>> coord =
      getValidCoord("Enter a coordinate", 5, 5, "c");
  if (coord.has_value()) {
    logger->info("Coordinate: ({}, {})", coord->first, coord->second);
  } else {
    logger->info("User cancelled input");
  }
  return 0;
}