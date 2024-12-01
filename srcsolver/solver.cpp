#include "stdafx.h"
#include "classes_to_test.h"
#include "sv_guessescache.h"
#include "no_separate_options_demo.h"
#include "sv_sfield.h"
#include "bs_onethread.h"
#include "bs2_onethread.h"
#include "branchless_no_indexes.h"
#include "bs4_onethread.h"
#include "brute_force.h"
#include "bs3_threads_atomics.h"
#include "report_generator.h"



/// <summary>
///  Very first option - simple solving solution. No speed optimisations
/// </summary>
/// 

class SimpleSolveLogic final : public TestingLogic<SimpleSolveLogic, "Very first, Simple Solution. No tries to optimize">
{
   std::unique_ptr<sv::GuessesCache> cache;
public:
   void InitializationLogic()
   {
      using namespace sv;
      cache.reset(new GuessesCache);
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      using namespace sv;
      using namespace sudoku_test_data;
      sfield solvedSudoku;
      sfield srcSudoku;
      for (size_t pos = 0; pos < SUDOKU_DATA_LENGTH; ++pos)
      {
         auto& v = toSolve[pos];
         if ('1' <= v && v <= '9')
         {
            srcSudoku.SetFieldCell(pos % NOWHERE, pos / NOWHERE, v - '1');
         }
      }

      sv::SolveSudoku(srcSudoku, cache, solvedSudoku);
      for (size_t pos = 0; pos < SUDOKU_DATA_LENGTH; ++pos)
      {
         toSetResult[pos] = static_cast<char>('1' + solvedSudoku.cells_[pos].value_);
      }

   }

   void DeinitializationLogic()
   {
      cache.reset();
   }
};


/// -------------------------------------------------------
/// <summary>
///  Branchless Logic - speed optimisations
/// </summary>
/// 
class BranchlessSolveLogic final : public TestingLogic<BranchlessSolveLogic, "Branchless Logic Solution. Is it faster for this case?">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      blacksmith::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};


/// -------------------------------------------------------
/// <summary>
///  Branchless Logic - go for all 81. and if it is still not solved do the guess at once.
///  even if we get any value set during this iteration
/// </summary>
/// 
class GuessAndBranchless final : public TestingLogic<GuessAndBranchless, "Branchless Logic + Guess Solution. 1 full run for 81 and guess at once. One more iteration of Optimization.">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      guess_is_in_the_main_loop::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};

/// -------------------------------------------------------
/// <summary>
///  Branchless Logic - no precalculated indexes.
///  some simplification
/// </summary>
/// 
class BranchlessSolveLogic3 final : public TestingLogic<BranchlessSolveLogic3, "Branchless Logic. Optimization. try 3. No precalculated indexes">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      branchless_no_indexes::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};

/// -------------------------------------------------------
/// <summary>
///  Branchless Logic - no precalculated indexes.
///  Bits for options have been used
/// </summary>
/// 
class BitsForOptions final : public TestingLogic<BitsForOptions, "Optimization. try 4 - Bit for Options">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      bits_options::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};

/// -------------------------------------------------------
/// <summary>
///  Brute force logic. Simple Enumeration
/// </summary>
/// 
class EnumerationSolveLogic final : public TestingLogic<EnumerationSolveLogic, "Brute force logic. Enumeration. Ported from python code by chat GPT">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      brute_force::solve_sudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};
/// -------------------------------------------------------
/// <summary>
///  Brute force logic. Simple Enumeration
/// </summary>
/// 
class NoOptionsButCalculations final : public TestingLogic<NoOptionsButCalculations, "Do Search of possible options during run. calculated Indexes">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      no_separate_options::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};

/// -------------------------------------------------------
//int main(int argc, char* argv[])
int main()
{
    sudoku_solver::Report_Generator report_gen;
    std::cout << "I am Sudoku solver demo project!" << std::endl;
    std::cout << "I have " << sudoku_test_data::nSudokuToSolve << " sudoku to solve" << std::endl;

    //{
    //   SimpleSolveLogic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic(report_gen);
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    //{
    //   EnumerationSolveLogic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic(report_gen);
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    //{
    //    BranchlessSolveLogic es;
    //    es.Initialize();
    //    es.HeatUpCall();
    //    es.MeasuramentLogic(report_gen);
    //    es.Deinitialize();
    //    std::cout << std::endl << std::endl << std::endl;
    //}
    {
       GuessAndBranchless es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic(report_gen);
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    {
       BranchlessSolveLogic3 es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic(report_gen);
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    {
        NoOptionsButCalculations es;
        es.Initialize();
        es.HeatUpCall();
        es.MeasuramentLogic(report_gen);
        es.Deinitialize();
        std::cout << std::endl << std::endl << std::endl;
    }
    //{
    //   BitsForOptions es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic(report_gen);
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    //{
    //   threads_atomics::MultithreadedAtomic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic(report_gen);
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    return EXIT_SUCCESS;
}
