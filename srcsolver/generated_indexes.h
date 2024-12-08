#pragma once
#include <array>
#include <stdint.h>

namespace gindexes
{
constexpr const char N_CELLS = 81;
constexpr const char N_OPTIONS = 9;


/// <summary>
///  here we hold indexes for clear specfic option from the field
///  in line,row,square when we set a value for cell in this line,row,square
/// </summary>
extern const std::array<std::array<std::size_t, 20>, N_CELLS> indexes_for_remove_option;


///@brief Full indexes for lines (row)
///  first index - number of line (row)
extern const std::size_t line_indexes[N_OPTIONS][N_OPTIONS];


///@brief Full indexes for columns
///  first index - number of column
extern const std::size_t column_indexes[N_OPTIONS][N_OPTIONS];


///@brief Full indexes for squares
///  first index - number of square
extern const std::size_t square_indexes[N_OPTIONS][N_OPTIONS];

}; // gindexes