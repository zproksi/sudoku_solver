#include "stdafx.h"
#include "sudoku_test_data.h"

namespace sudoku_test_data
{

const char* const testdata[] =
{
#include "test_sudoku.txt"
};


const char* const SudokuToSolve(const size_t at)
{
   return testdata[at * 2];
}
const char* const SudokuSolution(const size_t at)
{
   return testdata[at * 2 + 1];
}

} // namespace sudoku_test_data


