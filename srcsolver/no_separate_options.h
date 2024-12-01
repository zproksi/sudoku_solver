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


//#pragma warning( disable : 4324)
//struct __declspec(align(16)) SCELL final
struct SCELL final
{
   char value = NOTFOUND;
   void inline SetCellTo(const char v) noexcept
   {
      value = v;
   }

}; // struct SCELL final
//#pragma warning( default : 4324)

struct SFIELD final
{
   SCELL cells_[N_CELLS];
   COUNTT amount_established = 0;

   // constructs field
   SFIELD(const char* const src) noexcept
   {
      for (COUNTT i = 0; i < N_CELLS; ++i)
      {
         const char c = src[i];
         if (const bool b = c >= '1' && c <= '9'; b)
         {
            SetupCellToValue(i, c - '1');
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
      cells_[index].SetCellTo(v);
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
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         char options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
         for (auto ind : gindexes::indexes_for_remove_option[k])
         {
             const char vv = cells_[ind].value;
             if (const bool present = vv != NOTFOUND; present)
             {
                 options[vv] = NOTFOUND;
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
            result[bigIndex] = '1' + cells_[bigIndex].value;
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
         const COUNTT x = cells_[k].value;
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
