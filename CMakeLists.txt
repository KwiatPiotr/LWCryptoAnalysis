cmake_minimum_required(VERSION 3.16)
project(magisterka)

add_executable(magisterka
    main.cpp 
    src/rom/romulus.cpp
    src/sparkle/sparkle.cpp
)

include_directories(
    inc/
)






