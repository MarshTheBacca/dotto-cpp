# Create an executable
add_executable(dotto-cpp)

# Add source files
target_sources(dotto-cpp PRIVATE
    board.cpp
    cell.cpp
    enums.cpp
    game.cpp
    globals.cpp
    main.cpp
    other_tools.cpp
    piece.cpp
    player.cpp
    random.cpp
    settings_data.cpp
    validation_tools.cpp
    )
