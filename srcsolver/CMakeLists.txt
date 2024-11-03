cmake_minimum_required(VERSION 3.19)
set(pname solver)

# Source files
set(SOURCE_FILES
    solver.cpp
    stdafx.cpp
    simple_solver.cpp
    sudoku_test_data.cpp
    sv_guessescache.cpp
    sv_sfield.cpp
    timemeasurer.cpp
)
set(HEADER_FILES
    classes_to_test.h
    stdafx.h
    simple_solver.h
    sudoku_test_data.h
    sv_guessescache.h
    sv_sfield.h
    sv_types.h
    timemeasurer.h
)

# Define the executable target
add_executable(${pname} ${SOURCE_FILES} ${HEADER_FILES})

# Precompiled header
target_precompile_headers(${pname} PRIVATE stdafx.h)
