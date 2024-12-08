#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>
#include <thread>
#include <atomic>
#include "generated_indexes.h"


namespace bs_multithreaded
{

/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const char NOTFOUND = ' ';
constexpr const char N_OPTIONS = 9;
constexpr const char N_CELLS = 81;

struct SCELL final
{
   char value = NOTFOUND;
   char options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

   /// <summary>
   ///   set option value to value or NOTFOUND
   /// </summary>
   /// <param name="c">to set This Index at this index if v is Equal to this index</param>
   /// <param name="v">to set this index if this index came inside</param>
   constexpr void inline OptionValue(const char c, const char v) noexcept
   {
      const bool b = c == v;
      options[c] = b * c + NOTFOUND * (!b);
   }

   /// only this method acceps characters
   /// returns if the value was established
   constexpr bool SetValue(const char c) noexcept
   {
      const bool b = c >= '1' && c <='9';
      value = b * (c - '1') + (!b) * NOTFOUND;
      OptionValue(0, value); OptionValue(1, value); OptionValue(2, value);
      OptionValue(3, value); OptionValue(4, value); OptionValue(5, value);
      OptionValue(6, value); OptionValue(7, value); OptionValue(8, value);
      return b;
   }

   /// set cell value to this character without any check
   constexpr void inline SetCellTo(const char v) noexcept
   {
      OptionValue(0, v); OptionValue(1, v); OptionValue(2, v);
      OptionValue(3, v); OptionValue(4, v); OptionValue(5, v);
      OptionValue(6, v); OptionValue(7, v); OptionValue(8, v);
      value = v;
   }

   constexpr void ClearOption(const char v) noexcept
   {// clear only if not established to this value
      const bool b = v == value;
      options[v] = v * (b) + NOTFOUND * (!b);
   }

   /// <summary>
   ///   returns amount of opened options
   /// </summary>
   /// <returns>amount of opened options</returns>
   constexpr char PossibleOptionsAmount() const noexcept
   {
      return (options[0] == NOTFOUND) + (options[1] == NOTFOUND) + (options[2] == NOTFOUND)
           + (options[3] == NOTFOUND) + (options[4] == NOTFOUND) + (options[5] == NOTFOUND)
           + (options[6] == NOTFOUND) + (options[7] == NOTFOUND) + (options[8] == NOTFOUND);
   }

   /// if PossibleOptionsAmount returns 1
   /// this logic returns this index for the value
   constexpr char SpecifyValueIndex() const noexcept
   {
      return 
           (options[0] != NOTFOUND)*0 + (options[1] != NOTFOUND)*1  + (options[2] != NOTFOUND)*2
         + (options[3] != NOTFOUND)*3 + (options[4] != NOTFOUND)*4 + (options[5] != NOTFOUND)*5
         + (options[6] != NOTFOUND)*6 + (options[7] != NOTFOUND)*7 + (options[8] != NOTFOUND)*8;
   }

   constexpr bool Established() const noexcept {return value != NOTFOUND;}
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
      size_t amounts[N_OPTIONS] = {0}; // how many met  of 0,1,2,..     9+ if this index found
      size_t indexAt[N_OPTIONS];       // where we met this option

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
         const bool established = cell.value != NOTFOUND;
         amounts[at % N_OPTIONS] += established * N_OPTIONS; // add 9 if established

         amounts[0] += cell.options[0] == 0; indexAt[0] = at * (amounts[0] == 1);
         amounts[1] += cell.options[1] == 1; indexAt[1] = at * (amounts[1] == 1);
         amounts[2] += cell.options[2] == 2; indexAt[2] = at * (amounts[2] == 1);
         amounts[3] += cell.options[3] == 3; indexAt[3] = at * (amounts[3] == 1);
         amounts[4] += cell.options[4] == 4; indexAt[4] = at * (amounts[4] == 1);
         amounts[5] += cell.options[5] == 5; indexAt[5] = at * (amounts[5] == 1);
         amounts[6] += cell.options[6] == 6; indexAt[6] = at * (amounts[6] == 1);
         amounts[7] += cell.options[7] == 7; indexAt[7] = at * (amounts[7] == 1);
         amounts[8] += cell.options[8] == 8; indexAt[8] = at * (amounts[8] == 1);
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
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* result) noexcept
   {
      int foundN = 1;
      while (foundN > 0 && amount_established < N_CELLS)
      {
         using namespace gindexes;
         foundN =
            CheckIndexesArray(line_indexes[0])
          + CheckIndexesArray(line_indexes[1])
          + CheckIndexesArray(line_indexes[2])
          + CheckIndexesArray(line_indexes[3])
          + CheckIndexesArray(line_indexes[4])
          + CheckIndexesArray(line_indexes[5])
          + CheckIndexesArray(line_indexes[6])
          + CheckIndexesArray(line_indexes[7])
          + CheckIndexesArray(line_indexes[8])
          + CheckIndexesArray(column_indexes[0])
          + CheckIndexesArray(column_indexes[1])
          + CheckIndexesArray(column_indexes[2])
          + CheckIndexesArray(column_indexes[3])
          + CheckIndexesArray(column_indexes[4])
          + CheckIndexesArray(column_indexes[5])
          + CheckIndexesArray(column_indexes[6])
          + CheckIndexesArray(column_indexes[7])
          + CheckIndexesArray(column_indexes[8])
          + CheckIndexesArray(square_indexes[0])
          + CheckIndexesArray(square_indexes[1])
          + CheckIndexesArray(square_indexes[2])
          + CheckIndexesArray(square_indexes[3])
          + CheckIndexesArray(square_indexes[4])
          + CheckIndexesArray(square_indexes[5])
          + CheckIndexesArray(square_indexes[6])
          + CheckIndexesArray(square_indexes[7])
          + CheckIndexesArray(square_indexes[8]);
      };

      if (!Validate())
         return false;

      if (amount_established < N_CELLS)
      {
         // need to guess
         //
         size_t guesses[N_OPTIONS];
         size_t nGuesses = 0;
         size_t bigIndex = 0;
         for (; bigIndex < N_CELLS; ++bigIndex)
         {
            SCELL& cell = cells_[bigIndex];
            if (cell.value == NOTFOUND)
            {
               for (size_t i = 0; i < N_OPTIONS; ++i)
               {
                  const bool bOption = cell.options[i] == i;
                  guesses[nGuesses] = bOption * i;
                  nGuesses += bOption;
               }
               break;
            }
         }

         for (size_t i = 0; i < nGuesses; ++i)
         {
            // create guess
            SFIELD guessField(*this);
            guessField.SetupCellToValue(bigIndex, static_cast<char>(guesses[i]));
            if (guessField.Solve(result))
               return true;
         }
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
      using namespace gindexes;
      return (N_OPTIONS * 3 ==
        ValidateIndexesArray(line_indexes[0])
      + ValidateIndexesArray(line_indexes[1])
      + ValidateIndexesArray(line_indexes[2])
      + ValidateIndexesArray(line_indexes[3])
      + ValidateIndexesArray(line_indexes[4])
      + ValidateIndexesArray(line_indexes[5])
      + ValidateIndexesArray(line_indexes[6])
      + ValidateIndexesArray(line_indexes[7])
      + ValidateIndexesArray(line_indexes[8])
      + ValidateIndexesArray(column_indexes[0])
      + ValidateIndexesArray(column_indexes[1])
      + ValidateIndexesArray(column_indexes[2])
      + ValidateIndexesArray(column_indexes[3])
      + ValidateIndexesArray(column_indexes[4])
      + ValidateIndexesArray(column_indexes[5])
      + ValidateIndexesArray(column_indexes[6])
      + ValidateIndexesArray(column_indexes[7])
      + ValidateIndexesArray(column_indexes[8])
      + ValidateIndexesArray(square_indexes[0])
      + ValidateIndexesArray(square_indexes[1])
      + ValidateIndexesArray(square_indexes[2])
      + ValidateIndexesArray(square_indexes[3])
      + ValidateIndexesArray(square_indexes[4])
      + ValidateIndexesArray(square_indexes[5])
      + ValidateIndexesArray(square_indexes[6])
      + ValidateIndexesArray(square_indexes[7])
      + ValidateIndexesArray(square_indexes[8])
      );
   }

   /// <summary>
   ///   returns true if we have only 1 value of index
   ///       and more than 1 option for not established indexes
   /// </summary>
   bool ValidateIndexesArray(const size_t indexes[N_OPTIONS]) noexcept
   {
      size_t amounts[N_OPTIONS] = {0};

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
         amounts[0] += cell.options[0] != NOTFOUND;
         amounts[1] += cell.options[1] != NOTFOUND;
         amounts[2] += cell.options[2] != NOTFOUND;
         amounts[3] += cell.options[3] != NOTFOUND;
         amounts[4] += cell.options[4] != NOTFOUND;
         amounts[5] += cell.options[5] != NOTFOUND;
         amounts[6] += cell.options[6] != NOTFOUND;
         amounts[7] += cell.options[7] != NOTFOUND;
         amounts[8] += cell.options[8] != NOTFOUND;
      }

      return N_OPTIONS == (
         (amounts[0] > 0) + (amounts[1] > 0) + (amounts[2] > 0) +
         (amounts[3] > 0) + (amounts[4] > 0) + (amounts[5] > 0) +
         (amounts[6] > 0) + (amounts[7] > 0) + (amounts[8] > 0) );

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

} // namespac bs
