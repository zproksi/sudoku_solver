#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace bs3
{
/// to change for compare against bs2
   using COUNTT = size_t;
//   using COUNTT = int;



/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const char NOTFOUND = ' ';
constexpr const char N_OPTIONS = 9;
constexpr const char N_CELLS = 81;


consteval size_t NoOptionsValue()
{
   decltype(NoOptionsValue()) result = 0;
   for (decltype(result) i = 0; i < sizeof(result); ++i) {
      result = (result << 8) | NOTFOUND;
   }
   return result;
}

//#pragma warning( disable : 4324)
//struct __declspec(align(16)) SCELL final
struct SCELL final
{
   char value = NOTFOUND;
   char options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};


   /// set cell value to this character without any check
   void inline SetCellTo(const char v) noexcept
   {
      *( reinterpret_cast<decltype(NoOptionsValue())*>(options) ) = NoOptionsValue();
      options[N_OPTIONS - 1] = NOTFOUND;
      value = v;
      options[v] = v;
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
      const COUNTT row{index / N_OPTIONS};
      const COUNTT column{index % N_OPTIONS};
      const COUNTT square{(index / 27) * 3 + (column / 3)};
      const COUNTT basicIndexSquare = (square / 3) * 27 + (square % 3) * 3;
      for(COUNTT k = 0; k < N_OPTIONS; ++k)
      {
         cells_[row * N_OPTIONS + k].options[v] = NOTFOUND; // row
         cells_[column + N_OPTIONS * k].options[v] = NOTFOUND; // column
         cells_[basicIndexSquare + N_OPTIONS * (k / 3) + (k % 3)].options[v] = NOTFOUND; // column
      }

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

      for (COUNTT k = 0; k < N_CELLS; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const COUNTT optionsAmount =
            (cell.options[0] == 0) + (cell.options[1] == 1) + (cell.options[2] == 2) +
            (cell.options[3] == 3) + (cell.options[4] == 4) + (cell.options[5] == 5) +
            (cell.options[6] == 6) + (cell.options[7] == 7) + (cell.options[8] == 8);

         /// no options and no value
         if (0 == optionsAmount) { return false; }

         if (1 == optionsAmount)
         {
            const COUNTT valueToSet = (cell.options[1] == 1) * 1 + (cell.options[2] == 2) * 2
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
         for (COUNTT i = 0; i < N_OPTIONS; ++i)
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

} // namespace bs2
