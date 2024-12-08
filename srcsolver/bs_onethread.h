#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>
#include "generated_indexes.h"

namespace blacksmith
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
               for (char i = 0; i < N_OPTIONS; ++i)
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

      const bool solved = (amount_established >= N_CELLS) && Validate();
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
      size_t amounts[N_OPTIONS] = {0,0,0,0,0,0,0,0,0};
      size_t found[N_OPTIONS] = {0,0,0,0,0,0,0,0,0}; // if we found - set to 1 at this value

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
         const bool bFound = cell.value != NOTFOUND;
         found[cell.value % N_OPTIONS] += bFound;

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

      return N_OPTIONS * 2 ==
         (amounts[0] > 0) + (amounts[1] > 0) + (amounts[2] > 0) +
         (amounts[3] > 0) + (amounts[4] > 0) + (amounts[5] > 0) +
         (amounts[6] > 0) + (amounts[7] > 0) + (amounts[8] > 0) +
         (found[0] < 2) + (found[1] < 2) + (found[2] < 2) +
         (found[3] < 2) + (found[4] < 2) + (found[5] < 2) +
         (found[6] < 2) + (found[7] < 2) + (found[8] < 2)
         ;

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

} // namespace blacksmith
