#pragma once
#include <array>
#include <stdint.h>

namespace gindexes
{
constexpr const char N_CELLS = 81;

/// <summary>
///  here we hold indexes for clear specfic option from the field
///  in line,row,square when we set a value for cell in this line,row,square
/// </summary>
extern const std::array<std::array<std::size_t, 20>, N_CELLS> indexes_for_remove_option;

}; // gindexes