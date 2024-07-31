# Create an executable
add_executable(dotto-cpp.exe)

# Add source files
target_sources(dotto-cpp.exe PRIVATE
    board.cpp
    game.cpp
    globals.cpp
    main.cpp
    other_tools.cpp
    random.cpp
    settings_data.cpp
    validation_tools.cpp
    )
