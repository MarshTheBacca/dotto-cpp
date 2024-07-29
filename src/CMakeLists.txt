# Create an executable
add_executable(dotto-cpp.exe)

# Add source files
target_sources(dotto-cpp.exe PRIVATE
    main.cpp
    validation_tools.cpp
    settings_data.cpp
)
