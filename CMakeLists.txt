# cmake_minimum_required(VERSION <specify CMake version here>)
project(Pokedex C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Pokedex
        abb.c
        abb.h
        lista.c
        lista.h
        main.c
        pokedex.c
        pokedex.h
        testing.c)
