#include "stdafx.h"
#include "classes_to_test.h"
#include "sv_guessescache.h"
#include "sv_sfield.h"
#include "bs_onethread.h"
#include "bs2_onethread.h"
#include "bs3_onethread.h"
#include "brute_force.h"


/// <summary>
///  Testing option - doing nothing - just copy solution from solved solutions
/// </summary>

class TestingLogicNothing final : public TestingLogic<TestingLogicNothing, "no work in scope">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      using namespace sudoku_test_data;
      for (size_t i = 0; i < nSudokuToSolve; ++i)
      {
         if (0 == memcmp(SudokuToSolve(i), toSolve, SUDOKU_DATA_LENGTH))
         {
            memcpy(toSetResult, SudokuSolution(i), SUDOKU_DATA_LENGTH);
            return;
         }
      }
   }

   void DeinitializationLogic()
   {
   }
};
/// -------------------------------------------------------


/// <summary>
///  Very first option - simple solving solution. No speed optimisations
/// </summary>
/// 

class SimpleSolveLogic final : public TestingLogic<SimpleSolveLogic, "Very first, Simple Solution">
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

class BranchlessSolveLogic final : public TestingLogic<BranchlessSolveLogic, "Branchless Logic Solution">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      bs::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};


class BranchlessSolveLogic2 final : public TestingLogic<BranchlessSolveLogic2, "Branchless Logic Solution 2">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      bs2::SolveSudoku(toSolve, toSetResult);
   }

   void DeinitializationLogic()
   {
   }
};


class BranchlessSolveLogic3 final : public TestingLogic<BranchlessSolveLogic3, "Branchless Logic Solution 3">
{
public:
   void InitializationLogic()
   {
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      bs3::SolveSudoku(toSolve, toSetResult);
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

class EnumerationSolveLogic final : public TestingLogic<EnumerationSolveLogic, "Brute force logic. Enumeration">
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

//int main(int argc, char* argv[])
int main()
{
    std::cout << "I am Sudoku solver demo project!" << std::endl;
    std::cout << "I have " << sudoku_test_data::nSudokuToSolve << " sudoku to solve" << std::endl;

    //{
    //   TestingLogicNothing es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic();
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    //{
    //   SimpleSolveLogic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic();
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    //{
    //   BranchlessSolveLogic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic();
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    {
       BranchlessSolveLogic2 es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic();
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    {
       BranchlessSolveLogic3 es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic();
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    //{
    //   EnumerationSolveLogic es;
    //   es.Initialize();
    //   es.HeatUpCall();
    //   es.MeasuramentLogic();
    //   es.Deinitialize();
    //   std::cout << std::endl << std::endl << std::endl;
    //}
    return EXIT_SUCCESS;
}
