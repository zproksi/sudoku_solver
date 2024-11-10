#pragma once

#include <span>
#include <string_view>
#include "sudoku_test_data.h"
#include "timemeasurer.h"

namespace
{
   char to_hold_solutions[sudoku_test_data::nSudokuToSolve][sudoku_test_data::SUDOKU_DATA_LENGTH];
};

constexpr size_t RUNS_AMOUNT = 27;


template<size_t N>
struct StringLiteral {
   constexpr StringLiteral(const char(&str)[N]) {
      std::copy_n(str, N, value);
   }

   char value[N];
};



/// type T should implement  3 methods:
///     any InitializationLogic();
///     any SolveSudoku(const char* const toSolve, char* toSetResult);
///     any DeinitializationLogic();
///  where 'any' is any type
/// 
template<typename T, StringLiteral TestName>
struct TestingLogic
{
   void CoutDone()
   {
      std::cout << "  --- done ---  " << "\n";
   };
   bool TestCorrectness()
   {
      bool ret = true;
      for (size_t i = 0; i < sudoku_test_data::nSudokuToSolve; ++i)
      {
         ret = ret && (0 == memcmp(sudoku_test_data::SudokuSolution(i), to_hold_solutions[i], sudoku_test_data::SUDOKU_DATA_LENGTH));
      }
      return ret;
   }

   /// 
   /// //////////////////////////////////////////////////////////////////////////
   /// 

   void Initialize()
   {
      std::cout << "Initialization logic for " << TestName.value << "\n";
      static_cast<T*>(this)->InitializationLogic();
      CoutDone();
   }

   void HeatUpCall()
   {
      T* const pT = static_cast<T*>(this);
      for (size_t i = 0; i < sudoku_test_data::nSudokuToSolve; ++i)
      {
         memset(to_hold_solutions[i], 'Q', sizeof(to_hold_solutions[i]));  // to ensure - solved here
         pT->SolveSudoku(sudoku_test_data::SudokuToSolve(i), to_hold_solutions[i]);
      }
   }

   void MeasuramentLogic()
   {
      std::cout << "Measurement logic for:   -->   \"" << TestName.value << "\"   <---\n";
      std::cout << "Heat up call" << "\n";
      HeatUpCall();
      CoutDone();
      const bool bSolved = TestCorrectness();
      std::cout << "Solved - " << (bSolved ? "Correct. Continue measurement": "Wrong") <<"\n";
      CoutDone();

      T* const pT = static_cast<T*>(this);
      pT->HeatUpCall();
      std::cout << "Solving sudoku " << RUNS_AMOUNT << " times\n";
      {
         long long nanosecondsForRun;
         {
            TimeMeasurer tmLocal("test run");
            for (size_t repetition = 0; repetition < RUNS_AMOUNT; ++repetition)
            {
               for (size_t i = 0; i < sudoku_test_data::nSudokuToSolve; ++i)
               {
                  pT->SolveSudoku(sudoku_test_data::SudokuToSolve(i), to_hold_solutions[i]);
               }
            }
            nanosecondsForRun = tmLocal.NanosecondsElapsed(std::chrono::high_resolution_clock::now());
         }
         std::cout << RUNS_AMOUNT << " took " << TimeMeasurer::FormatNanoseconds(nanosecondsForRun) << " nanoseconds\n";
         std::cout << TimeMeasurer::FormatNanoseconds(nanosecondsForRun / RUNS_AMOUNT + 1) << " nanoseconds for " << sudoku_test_data::nSudokuToSolve << " sudoku in average\n\n";
      }
      CoutDone();
   }

   void Deinitialize()
   {
      std::cout << "Deinitialization logic for " << TestName.value << "\n";
      static_cast<T*>(this)->DeinitializationLogic();
      CoutDone();
   }
};

