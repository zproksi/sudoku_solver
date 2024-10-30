#pragma once

namespace sudoku_test_data
{
constexpr size_t SUDOKU_DATA_LENGTH = 81;

///@brief how many sudoku to solve
constexpr size_t nSudokuToSolve = 8;


/// <summary>
///  format of the data:
///    pairs of
///    1) sudoku data - 81 bytes length,
///        with 1,2,3,4,5,6,7,8,9 and any other character instead of empty position
///    2) the same sudoku, but solved
///    3) last line is nullptr
///    @note: sudoku must be solved as 1 only possible solution
/// </summary>
extern const char* const testdata[nSudokuToSolve * 2];

/// <summary>
///    return a pointer to sudoku for solve at pos 'at'
/// </summary>
/// <param name="at">which sudoku to solve</param>
/// <returns>pointer to the sudoku data with length of SUDOKU_DATA_LENGTH</returns>
const char* const SudokuToSolve(const size_t at);

/// <summary>
///    return a pointer to solved sudoku at pos 'at'
/// </summary>
/// <param name="at">solved sudoku position</param>
/// <returns>pointer to the solved sudoku data with length of SUDOKU_DATA_LENGTH</returns>
const char* const SudokuSolution(const size_t at);

};

