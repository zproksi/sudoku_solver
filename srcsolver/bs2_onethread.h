#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>
#include "generated_indexes.h"

namespace guess_is_in_the_main_loop
{

/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const char NOTFOUND = ' ';
constexpr const char N_OPTIONS = 9;
constexpr const char N_CELLS = 81;


consteval size_t NoOptionsValue()
{
   size_t result = 0;
   for (std::size_t i = 0; i < sizeof(size_t); ++i) {
      result = (result << 8) | NOTFOUND;
   }
   return result;
}

struct SCELL final
{
   char value = NOTFOUND;
   char options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};


   /// <summary>
   ///   set option value to value or NOTFOUND
   /// </summary>
   /// <param name="c">to set This Index at this index if v is Equal to this index</param>
   /// <param name="v">to set this index if this index came inside</param>
   constexpr void inline OptionValue(const bool bValid, const char c, const char v) noexcept
   {
      const bool b = c == v;
      options[c] = options[c] * (!bValid) + bValid * (NOTFOUND * (!b) + v * (b));
   }

   /// only this method acceps characters
   /// returns if the value was established
   constexpr bool SetValue(const char c) noexcept
   {
      const bool b = c >= '1' && c <='9';
      value = b * (c - '1') + (!b) * NOTFOUND;

      OptionValue(b, 0, value); OptionValue(b, 1, value); OptionValue(b, 2, value);
      OptionValue(b, 3, value); OptionValue(b, 4, value); OptionValue(b, 5, value);
      OptionValue(b, 6, value); OptionValue(b, 7, value); OptionValue(b, 8, value);
//      options[8] = options[8] * (!b) + b * (NOTFOUND * (8 != value) + value * (value == 8));

      return b;
   }

   /// set NOTFOUND of established value for index c
   constexpr void inline DetectOptionToSet(const char c, const char v) noexcept
   {
      const bool b = c == v;
      options[c] = NOTFOUND * (!b) + v * (b);
   }

   /// set cell value to this character without any check
   constexpr void inline SetCellTo(const char v) noexcept
   {
      DetectOptionToSet(0, v); DetectOptionToSet(1, v); DetectOptionToSet(2, v);
      DetectOptionToSet(3, v); DetectOptionToSet(4, v); DetectOptionToSet(5, v);
      DetectOptionToSet(6, v); DetectOptionToSet(7, v); DetectOptionToSet(8, v);
      value = v;
   }

   constexpr void ClearOption(const char v) noexcept
   {// clear only if not established to this value
      const bool b = v == value;
      options[v] = v * (b) + NOTFOUND * (!b);
   }

}; // struct SCELL final


struct SFIELD final
{
   SCELL cells_[N_CELLS];
   size_t amount_established = 0;

   // constructs field
   SFIELD(const char* const src) noexcept
   {
      for (size_t i = 0; i < N_CELLS; ++i)
      {
         if (const bool established = cells_[i].SetValue(src[i]); established)
         {
            ++amount_established;
            const char v = cells_[i].value;
            for (size_t it : gindexes::indexes_for_remove_option[i])
            {
               // remove possible options from cells in the same line, column and square
               cells_[it].ClearOption(v);
            }
         }
      }
   }

   /// set specific cell to the value
   /// free necessary options
   /// increase amount_established
   void inline SetupCellToValue(const size_t index, const char v) noexcept
   {
      ++amount_established;
      cells_[index].SetCellTo(v);
      for (size_t it : gindexes::indexes_for_remove_option[index])
      {
         // remove possible options from cells in the same line, column and square
         cells_[it].ClearOption(v);
      }
   }

   // check Array for any possible value to set
   bool CheckIndexesArray(const size_t indexes[N_OPTIONS]) noexcept
   {
      // how many options for this index in line exists
      size_t amounts[N_OPTIONS] = {0,0,0,0,0,0,0,0,0}; // how many met  of 0,1,2,..     9+ if this index found
      size_t indexAt[N_OPTIONS] = {N_CELLS, N_CELLS, N_CELLS, N_CELLS, N_CELLS, N_CELLS, N_CELLS, N_CELLS, N_CELLS}; // where we met this option

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
         const bool established = cell.value != NOTFOUND;
         amounts[cell.value % N_OPTIONS] += established * N_OPTIONS; // add 9 if established

         //                                  set index        only if == 1          and we did not set it before
         amounts[0] += cell.options[0] == 0; {const bool b = (amounts[0] == 1) * (indexAt[0] == N_CELLS); indexAt[0] = at * b + (!b) * indexAt[0];}
         amounts[1] += cell.options[1] == 1; {const bool b = (amounts[1] == 1) * (indexAt[1] == N_CELLS); indexAt[1] = at * b + (!b) * indexAt[1];}
         amounts[2] += cell.options[2] == 2; {const bool b = (amounts[2] == 1) * (indexAt[2] == N_CELLS); indexAt[2] = at * b + (!b) * indexAt[2];}
         amounts[3] += cell.options[3] == 3; {const bool b = (amounts[3] == 1) * (indexAt[3] == N_CELLS); indexAt[3] = at * b + (!b) * indexAt[3];}
         amounts[4] += cell.options[4] == 4; {const bool b = (amounts[4] == 1) * (indexAt[4] == N_CELLS); indexAt[4] = at * b + (!b) * indexAt[4];}
         amounts[5] += cell.options[5] == 5; {const bool b = (amounts[5] == 1) * (indexAt[5] == N_CELLS); indexAt[5] = at * b + (!b) * indexAt[5];}
         amounts[6] += cell.options[6] == 6; {const bool b = (amounts[6] == 1) * (indexAt[6] == N_CELLS); indexAt[6] = at * b + (!b) * indexAt[6];}
         amounts[7] += cell.options[7] == 7; {const bool b = (amounts[7] == 1) * (indexAt[7] == N_CELLS); indexAt[7] = at * b + (!b) * indexAt[7];}
         amounts[8] += cell.options[8] == 8; {const bool b = (amounts[8] == 1) * (indexAt[8] == N_CELLS); indexAt[8] = at * b + (!b) * indexAt[8];}
      }

      bool ret = false;
      // for every 1 in amounts set the value
      for (size_t i = 0; i < sizeof(amounts) / sizeof(*amounts); ++i)
      {
         if (1 == amounts[i])
         {
            ret = true;
            SetupCellToValue(indexAt[i], static_cast<char>(i));
         }
      }
      return ret;
   }


   /// <summary>
   ///  return true if there are cells inside the field with possible options and not established value
   /// </summary>
   /// <returns>false if there are cells without options and value</returns>
   bool SolutionImpossible()
   {
      static_assert(sizeof(size_t) == sizeof(SCELL::options) - 1, "We count on size of options array");
      bool ret = false;
      for (size_t k = 0; (!ret) * (k < N_CELLS); ++k)
      {
         const SCELL& cell = cells_[k];
         ret = (cell.options[N_OPTIONS - 1] == NOTFOUND) * (*reinterpret_cast<const size_t*>(cell.options) == NoOptionsValue());
      }
      return ret;
   }

   /// <summary>
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* result) noexcept
   {
      size_t indexOfGuess = N_CELLS;

      for (size_t k = 0; k < N_CELLS; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const size_t optionsAmount =
            (cell.options[0] == 0) + (cell.options[1] == 1) + (cell.options[2] == 2) +
            (cell.options[3] == 3) + (cell.options[4] == 4) + (cell.options[5] == 5) +
            (cell.options[6] == 6) + (cell.options[7] == 7) + (cell.options[8] == 8);

         /// no options and no value
         if (0 == optionsAmount) { return false; }

         if (1 == optionsAmount)
         {
            const size_t valueToSet = (cell.options[1] == 1) * 1 + (cell.options[2] == 2) * 2
               + (cell.options[3] == 3) * 3 + (cell.options[4] == 4) * 4 + (cell.options[5] == 5) * 5
               + (cell.options[6] == 6) * 6 + (cell.options[7] == 7) * 7 + (cell.options[8] == 8) * 8;
            SetupCellToValue(k, static_cast<char>(valueToSet));
         }
         else
         {
            indexOfGuess = k;
         }
      }

      if (amount_established < N_CELLS)
      {
         const SCELL& cell = cells_[indexOfGuess];
         for (size_t i = 0; i < N_OPTIONS; ++i)
         {
            const char vv = cell.options[i];
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
         for (size_t bigIndex = 0; bigIndex < N_CELLS; ++bigIndex)
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
      size_t summ[27] = {0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0};

      for (size_t k = 0; k < N_CELLS; ++k)
      {
         const size_t row{k / N_OPTIONS};
         const size_t column{k % N_OPTIONS};
         const size_t square{(k / 27) * 3 + (column / 3)};
         const size_t x = cells_[k].value;
         summ[row] += x;
         summ[N_OPTIONS + column] += x;
         summ[N_OPTIONS * 2 + square] += x;
      }
      size_t valid = 1;
      for (size_t sm : summ)
      {
         valid *= (sm == 36);
      }
      return valid != 0;
   }

   /// <summary>
   ///   returns true if we have only 1 value of index
   ///       and more than 1 option for not established indexes
   /// </summary>
   bool ValidateIndexesArray(const size_t indexes[N_OPTIONS]) noexcept
   {
      size_t summ = 0;
      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         summ += cells_[indexes[k]].value;
      }
      return summ == 36;

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

} // namespace guess_is_in_the_main_loop
