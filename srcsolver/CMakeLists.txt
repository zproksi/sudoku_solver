cmake_minimum_required(VERSION 3.19)
set(pname solver)

# Source files
set(SOURCE_FILES
    generated_indexes.cpp
    report_generator.cpp
    solver.cpp
    stdafx.cpp
    sudoku_test_data.cpp
    sv_guessescache.cpp
    sv_sfield.cpp
    timemeasurer.cpp
)
set(HEADER_FILES
    branchless_no_indexes.h
    brute_force.h
    bs_onethread.h
    bs2_onethread.h
    bs3_threads_atomics.h
    bs4_onethread.h
    classes_to_test.h
    generated_indexes.h
    last_try_tomakefaster.h
    multithreaded_solve.h
    report_generator.h
    stdafx.h
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
