#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>
#include "generated_indexes.h"

namespace no_separate_options
{
   using COUNTT = size_t;
//   using COUNTT = int;



/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const char NOTFOUND = ' ';
constexpr const char N_OPTIONS = 9;
constexpr const char N_CELLS = 81;


struct SFIELD final
{
   char cells_[N_CELLS];
   COUNTT amount_established = 0;

   // constructs field
   SFIELD(const char* const src) noexcept
   {
      for (COUNTT i = 0; i < N_CELLS; ++i)
      {
         const char c = src[i];
         if (const bool b = c >= '1' && c <= '9'; b)[[unlikely]]
         {
            SetupCellToValue(i, c - '1');
         }
         else
         {
             cells_[i] = NOTFOUND;
         }
      }
   }

   /// free necessary options
   /// increase amount_established
   /// no precalculation indexes but do calculation in place
   /// set specific cell to the value
   void inline SetupCellToValue(const COUNTT index, const char v) noexcept
   {
      ++amount_established;
      cells_[index] = v;
   }


   /// <summary>
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* result) noexcept
   {
      COUNTT indexOfGuess = N_CELLS;

      char optionsX[N_OPTIONS];

      for (COUNTT k = 0; k < N_CELLS; ++k)
      {
         if (NOTFOUND != cells_[k])
            continue;

         char options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
         if (1)
         {
             if (0)
             {
                 for (size_t ind : gindexes::indexes_for_remove_option[k])
                 {
                     const char vv = cells_[ind];
                     if (vv != NOTFOUND)
                     {
                         options[static_cast<int>(vv)] = NOTFOUND;
                     }
                 }
             }
             else
             {  // fastest
                 for (size_t ind = 0; ind < gindexes::indexes_for_remove_option[k].size(); ++ind)
                 {
                     const char vv = cells_[gindexes::indexes_for_remove_option[k][ind]];
                     if (vv != NOTFOUND)
                     {
                         options[static_cast<int>(vv)] = NOTFOUND;
                     }
                 }
             }
         }
         else
         {
             const COUNTT row{k / N_OPTIONS};
             const COUNTT column{k % N_OPTIONS};
             const COUNTT square{(k / 27) * 3 + (column / 3)};
             const COUNTT basic_in_sq = 27 * (square / 3) + (square % 3) * 3; // left top cell in square
             for (COUNTT ind = 0; ind < N_OPTIONS; ++ind)
             {
                 {
                    const char vv = cells_[N_OPTIONS * row + ind];
                    if (vv != NOTFOUND)
                    {
                        options[static_cast<int>(vv)] = NOTFOUND;
                    }
                 }
                 {
                     const char vv = cells_[N_OPTIONS * ind + column];
                     if (vv != NOTFOUND)
                     {
                         options[static_cast<int>(vv)] = NOTFOUND;
                     }
                 }
                 {
                     const char vv = cells_[basic_in_sq + N_OPTIONS * (ind / 3) + (ind % 3)];
                     if (vv != NOTFOUND)
                     {
                         options[static_cast<int>(vv)] = NOTFOUND;
                     }
                 }
             }
         }


         const COUNTT optionsAmount =
            (options[0] == 0) + (options[1] == 1) + (options[2] == 2) +
            (options[3] == 3) + (options[4] == 4) + (options[5] == 5) +
            (options[6] == 6) + (options[7] == 7) + (options[8] == 8);

         /// no options and no value
         if (0 == optionsAmount) { return false; }

         if (1 == optionsAmount)
         {
            const COUNTT valueToSet = (options[1] == 1) * 1 + (options[2] == 2) * 2
               + (options[3] == 3) * 3 + (options[4] == 4) * 4 + (options[5] == 5) * 5
               + (options[6] == 6) * 6 + (options[7] == 7) * 7 + (options[8] == 8) * 8;
            SetupCellToValue(k, static_cast<char>(valueToSet));
         }
         else
         {
            memcpy(optionsX, options, sizeof(optionsX));
            indexOfGuess = k;
         }
      }

      if (amount_established < N_CELLS)
      {
         for (COUNTT i = 0; i < N_OPTIONS; ++i)
         {
            const char vv = optionsX[i];
            if (vv == NOTFOUND) continue;

            // create guess
            SFIELD guessField(*this);
            guessField.SetupCellToValue(indexOfGuess, vv);
            if (guessField.Solve(result))
               return true;
         }
         return false;
      }

      const bool solved = Validate();
      if (solved)
      {
         for (COUNTT bigIndex = 0; bigIndex < N_CELLS; ++bigIndex)
         {
            result[bigIndex] = '1' + cells_[bigIndex];
         }
      }
      return solved;
   } // bool Solve(char* result) noexcept

   /// return true if sudoku is ok to solve
   /// no concontradiction in the data
   bool Validate() noexcept
   {
      COUNTT summ[27] = {0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0};

      for (COUNTT k = 0; k < N_CELLS; ++k)
      {
         const COUNTT row{k / N_OPTIONS};
         const COUNTT column{k % N_OPTIONS};
         const COUNTT square{(k / 27) * 3 + (column / 3)};
         const COUNTT x = cells_[k];
         summ[row] += x;
         summ[N_OPTIONS + column] += x;
         summ[N_OPTIONS * 2 + square] += x;
      }
      COUNTT valid = 1;
      for (COUNTT sm : summ)
      {
         valid *= (sm == 36);
      }
      return valid != 0;
   }
};

/// <summary>
///   doing solve here
/// </summary>
bool SolveSudoku(const char* const src, char* result)
{
   SFIELD field(src);

   return field.Solve(result);
}

} // namespace no_separate_options
