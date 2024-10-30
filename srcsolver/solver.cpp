#include "stdafx.h"
#include "classes_to_test.h"



class TestingLogicNothing : public TestingLogic<TestingLogicNothing, "no work in scope">
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


//int main(int argc, char* argv[])
int main()
{
    std::cout << "I am Sudoku solver demo project!" << std::endl;
    std::cout << "I have " << sudoku_test_data::nSudokuToSolve << " sudoku to solve" << std::endl;

    TestingLogicNothing es;
    es.Initialize();
    es.HeatUpCall();
    es.MeasuramentLogic();
    es.Deinitialize();
    return EXIT_SUCCESS;
}
