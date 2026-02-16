cmake_minimum_required(VERSION 3.10)

project(MyGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(MyGame
    main.cpp
    Cell.cpp
    Player.cpp
    Enemy.cpp
    GameField.cpp
)
