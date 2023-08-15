cmake_minimum_required(VERSION 3.16)
project(magisterka)

add_executable(magisterka
    # main.cpp 
    genkat_aead.cpp
    src/rom/romulus.cpp
    src/sparkle/sparkle.cpp
    src/schwaemm/schwaemm.cpp
)

include_directories(
    inc/
)






