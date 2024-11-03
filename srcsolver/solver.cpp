#include "stdafx.h"
#include "classes_to_test.h"
#include "sv_guessescache.h"
#include "sv_sfield.h"


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
         auto& v = toSolve[pos];
         if ('1' <= v && v <= '9')
         {
            toSetResult[pos] = static_cast<char>('1' + solvedSudoku.cells_[pos].value_);
         }
      }
      

   }

   void DeinitializationLogic()
   {
      cache.reset();
   }
};


/// -------------------------------------------------------


//int main(int argc, char* argv[])
int main()
{
    std::cout << "I am Sudoku solver demo project!" << std::endl;
    std::cout << "I have " << sudoku_test_data::nSudokuToSolve << " sudoku to solve" << std::endl;

    {
       TestingLogicNothing es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic();
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    {
       SimpleSolveLogic es;
       es.Initialize();
       es.HeatUpCall();
       es.MeasuramentLogic();
       es.Deinitialize();
       std::cout << std::endl << std::endl << std::endl;
    }
    return EXIT_SUCCESS;
}
