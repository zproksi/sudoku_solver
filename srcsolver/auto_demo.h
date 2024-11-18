#pragma once

#include "classes_to_test.h"

#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace black_smith__
{
/// to change for compare against bs2
   using COUNTT = size_t;
   using SELLTYPE = char;
//   using COUNTT = int;



/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const SELLTYPE NOTFOUND = ' ';
constexpr const SELLTYPE N_OPTIONS = 9;
constexpr const SELLTYPE N_CELLS = 81;


consteval size_t NoOptionsValue()
{
   decltype(NoOptionsValue()) result = 0;
   for (decltype(result) i = 0; i < sizeof(result); ++i) {
      result = (result << 8) | NOTFOUND;
   }
   return result;
}

//#pragma warning( disable : 4324)
//struct alignas(std::hardware_destructive_interference_size) SCELL final
struct SCELL final
{
   SELLTYPE value = NOTFOUND;
   SELLTYPE options[N_OPTIONS] = {0, 1, 2, 3, 4, 5, 6, 7, 8};


   template<typename T>
   void SetCellTo(const T v)
   {
      if constexpr (sizeof(T) == 1)
      {
         *(reinterpret_cast<decltype(NoOptionsValue())*>(options)) = NoOptionsValue();
         options[N_OPTIONS - 1] = NOTFOUND;
      }
      else
      {
         for (COUNTT i = 0; i < N_OPTIONS; ++i) options[i] = NOTFOUND;
      }
      options[v] = v;
      value = v;
   }

   /// returns amount of possible options
   COUNTT inline OptionsAmount() const
   {
      COUNTT ret = 0;
      for (COUNTT i = 0; i < N_OPTIONS; ++i) ret += (options[i] != NOTFOUND);
//      for (const auto opt : options) ret += (opt != NOTFOUND); // !!! Demonstrate !!!
      return ret;
   }

   // return single opened option Index or garbage
   SELLTYPE inline Option() const
   {
      const COUNTT valueToSet = (options[1] == 1) * 1 + (options[2] == 2) * 2
         + (options[3] == 3) * 3 + (options[4] == 4) * 4 + (options[5] == 5) * 5
         + (options[6] == 6) * 6 + (options[7] == 7) * 7 + (options[8] == 8) * 8;

      return static_cast<SELLTYPE>(valueToSet);
   }
}; // struct SCELL final
//#pragma warning( default : 4324)

//--------------------------------------
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
         if ((c >= '1') && (c <= '9'))
         {
            SetupCellToValue(i, static_cast<SELLTYPE>(c - '1'));
         }
      }
   }

   /// free necessary options
   /// increase amount_established
   /// no precalculation indexes but do calculation in place
   /// set specific cell to the value
   void inline SetupCellToValue(const COUNTT index, const SELLTYPE v) noexcept
   {
      const COUNTT row{index / N_OPTIONS};
      const COUNTT column{index % N_OPTIONS};
      const COUNTT square{(index / 27) * 3 + (column / 3)};
      const COUNTT basicIndexSquare = (square / 3) * 27 + (square % 3) * 3;
      for (COUNTT k = 0; k < N_OPTIONS; ++k)
      {
         cells_[row * N_OPTIONS + k].options[v] = NOTFOUND; // row
         cells_[column + N_OPTIONS * k].options[v] = NOTFOUND; // column
         cells_[basicIndexSquare + N_OPTIONS * (k / 3) + (k % 3)].options[v] = NOTFOUND; // column
      }

      cells_[index].SetCellTo(v);
      ++amount_established;
   }



   /// <summary>
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* const result) noexcept
   {
      COUNTT indexOfGuess = N_CELLS;

      for (COUNTT k = 0; k < N_CELLS; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const COUNTT optionsAmount = cell.OptionsAmount();

         /// no options and no value
         if (0 == optionsAmount) { return false; }

         if (1 == optionsAmount)
         {
            SetupCellToValue(k, cell.Option());
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
            const SELLTYPE vv = cell.options[i];
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
            result[bigIndex] = '1' + static_cast<char>(cells_[bigIndex].value);
         }
      }
      return solved;
   } // bool Solve(SELLTYPE* result) noexcept

   /// return true if sudoku is ok to solve
   /// no concontradiction in the data
   bool Validate() noexcept
   {
      // calculate summs for each row, column and square
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


/// -------------------------------------------------------


struct SS
{

};

//--------------------------------------

class MultithreadedAtomic final : public TestingLogic<MultithreadedAtomic, "Multithreaded with atomics">
{
protected:
   unsigned int numCores = std::thread::hardware_concurrency();


public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      SFIELD field(toSolve);

      field.Solve(toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};



} // namespace black_smith__
