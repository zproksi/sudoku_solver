#pragma once

#include <memory>
#include <span>
#include <utility>
#include "sv_types.h"
#include <fstream>
#include <iostream>

namespace sv
{

class GuessesCache;

struct scell
{
    /// <summary>
    ///   Set value_ of the cell to the value. Clear all other options to EMPTY_VALUE
    /// </summary>
    /// <param name="value"> value to set - one of options</param>
    /// <returns>true - if other value was not set before
    ///          false - for any other case
    /// </returns>
    bool SetValue(const VALUE value) noexcept;

    /// <returns>true if cell already has nonempty value</returns>
    bool Established() const noexcept{ return EMPTY_VALUE != value_; }

    /// <summary>
    ///   set option to Empty
    /// </summary>
    /// <param name="value">option for his value will be empty after call</param>
    void ClearOption(const VALUE value) noexcept;

    VALUE options[NOWHERE] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    VALUE value_ = EMPTY_VALUE;
};



struct sfield
{
    /// <summary>
    ///    Set Field to the value '1'..'9'
    ///  remove options from related cells
    /// </summary>
    /// <param name="x">0..8</param>
    /// <param name="y">0..8</param>
    /// <param name="value">'1'..'9'</param>
    /// <returns>true  - in case the set have no conflicts
    ///          false - in case the value cannot be set at the coordinates
    /// </returns>
    bool SetFieldCell(const COORDINATE x, const COORDINATE y, const VALUE value) noexcept;

    /// <summary>
    ///   check line at y to set `value` only in one place
    /// </summary>
    /// <param name="y">line coordinate</param>
    /// <param name="value">check for this value only</param>
    /// <returns>COORDINATE '1'..'9' or NOWHERE</returns>
    COORDINATE CheckLine(const COORDINATE y, const VALUE value) const  noexcept;

    /// <summary>
    ///   check column at x to set `value` only in one place
    /// </summary>
    /// <param name="x">column coordinate</param>
    /// <param name="value">check for this value only</param>
    /// <returns>COORDINATE '1'..'9' or NOWHERE</returns>
    COORDINATE CheckColumn(const COORDINATE x, const VALUE value) const noexcept;

    /// <summary>
    ///   check square at sq to set `value` only in one place
    ///  |coordinate grid|
    ///  |   0   1   2   |
    ///  |   3   4   5   |
    ///  |   6   7   8   |
    ///  |~~~~~~~~~~~~~~~|
    /// </summary>
    /// <param name="sq">square coordinate</param>
    /// <param name="value">check for this value only</param>
    /// <returns>
    ///   the return coordinates to set value
    ///  or <NOWHERE, NOWHERE>
    /// </returns>
    std::pair<COORDINATE, COORDINATE> CheckSquare(const COORDINATE sq, const VALUE value) const noexcept;

    /// <summary>
    ///   return solved & result valid statuses
    /// </summary>
    /// <returns>
    ///   0        - solved correct
    ///   1        - not solved yet
    ///   2        - one of the cells do not contain possible options
    ///                or rules mistake
    ///   3        - cell is empty and no more options for cell
    /// </returns>
    int Status() const noexcept;


    scell cells_[CELLS_COUNT];

    // how many cells have value
    unsigned char found_ = 0;


};

class GuessCache;
/// <summary>
///    Solve sudoku field 
/// </summary>
/// <param name="field"> sudoku field for solve</param>
/// <param name="cache"> to place field instances for guesses</param>
/// <param name="solved"> to place solved instance</param>
/// <returns> true - solved; false - cannot solve</returns>
bool SolveSudoku(sfield& field, std::unique_ptr<GuessesCache>& cache, sfield& solved);



} // namspace sv
