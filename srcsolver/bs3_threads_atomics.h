#pragma once

#include "classes_to_test.h"

#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace threads_atomics
{
/// to change for compare against bs2
   using COUNTT = size_t;
   using SELLTYPE = char;
   using ATOMIC_ST = int64_t;
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
/// -------------------------------------------------------



struct SFIELD;
#define modeLeaveThread  0  // to request exit from thread
#define modeDoWork       1  // to request do the logic
#define modeWorkDone     2  // to signal that the work has been done

/// the ThreadRelated struct is hardly to prove workking
///  because it can read and write the same cell from 2 or more threads
///  from the other hand writing is happening only for the 1 value state
///  single options could be cleared only as single items

#pragma warning( disable : 4324)
struct
#ifdef __linux__
__attribute__((aligned(std::hardware_destructive_interference_size)))
#else
alignas(std::hardware_destructive_interference_size)
#endif
ThreadRelated final
{
   std::atomic<ATOMIC_ST> workMode{modeWorkDone};
   SFIELD* toWorkWith = nullptr;
   COUNTT nFound = 0; // how many items found in this thread
   COUNTT nFrom = 0;
   COUNTT nTo = 0;
   COUNTT guess_index{N_CELLS};
   COUNTT failOption = 0; // set to 1 if we have a Cell without any option
   COUNTT value_to_makeSomething{0};
};

// static_assert(sizeof(ThreadRelated) == std::hardware_destructive_interference_size, "must be this size");
#pragma warning( default : 4324)

//--------------------------------------
struct SFIELD final
{
   SCELL cells_[N_CELLS];
   COUNTT amount_established = 0;
   std::span<ThreadRelated> threads_Related;
   COUNTT indexMainThread = 0;

   // constructs field
   SFIELD(const char* const src, std::span<ThreadRelated> argForThreads) noexcept
      : threads_Related(argForThreads)
      , indexMainThread((N_CELLS / (threads_Related.size() + 1)) * threads_Related.size())
   {
      for (COUNTT i = 0; i < N_CELLS; ++i)
      {
         const char c = src[i];
         if ((c >= '1') && (c <= '9'))
         {
            SetupCellToValue(i, static_cast<SELLTYPE>(c - '1'));
            ++amount_established;
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
      for(COUNTT k = 0; k < N_OPTIONS; ++k)
      {
         cells_[row * N_OPTIONS + k].options[v] = NOTFOUND; // row
         cells_[column + N_OPTIONS * k].options[v] = NOTFOUND; // column
         cells_[basicIndexSquare + N_OPTIONS * (k / 3) + (k % 3)].options[v] = NOTFOUND; // column
      }

      cells_[index].SetCellTo(v);
   }

   void PartialWork_Thread(ThreadRelated* const ptr) noexcept
   {
      // do the rest of the job in the current thread:
      for (COUNTT k = ptr->nFrom; k < ptr->nTo; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const COUNTT optionsAmount = cell.OptionsAmount();

         /// no options and no value
         if (0 == optionsAmount) { ptr->failOption = 1; return; }

         if (1 == optionsAmount)
         {
            SetupCellToValue(k, cell.Option());
            ++ptr->nFound;
         }
         else
         {
            ptr->guess_index = k;
         }
      }
   }



   /// <summary>
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* const result) noexcept
   {
      COUNTT indexOfGuess = N_CELLS;
      COUNTT failOption = 0;

      for (decltype(threads_Related.size()) i = 0; i < threads_Related.size(); ++i)
      { // should start threads_Related.size() threads
         ThreadRelated& thItem = threads_Related[i];
         thItem.toWorkWith = this;
         thItem.nFound = 0;
         thItem.failOption = 0;
         thItem.guess_index = N_CELLS;
         thItem.workMode.store(modeDoWork, std::memory_order_release);
      }

      // do the rest of the job in the current thread:
      for (COUNTT k = indexMainThread; k < N_CELLS; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const COUNTT optionsAmount = cell.OptionsAmount();

         /// no options and no value
         if (0 == optionsAmount) { failOption = 1; break; }

         if (1 == optionsAmount)
         {
            SetupCellToValue(k, cell.Option());
            ++amount_established;
         }
         else
         {
            indexOfGuess = k;
         }
      }

      // wait the end of the jobs
      for (decltype(threads_Related.size()) i = 0; i < threads_Related.size(); ++i)
      { // should get results of the threads work
         ThreadRelated& thItem = threads_Related[i];
         if (thItem.workMode.load(std::memory_order_relaxed) != modeWorkDone)
         {
            while (thItem.workMode.load(std::memory_order_acquire) != modeWorkDone)
            {
               ++thItem.value_to_makeSomething;
               thItem.value_to_makeSomething |= 0;
            }
         }

         amount_established += thItem.nFound;
         if (N_CELLS != thItem.guess_index)
         {
            indexOfGuess = thItem.guess_index;
         }
         failOption += thItem.failOption;
      }

      // cell with no options has been found
      if (failOption) return false;


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
            ++guessField.amount_established;
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



//--------------------------------------
class MultithreadedAtomic final : public TestingLogic<MultithreadedAtomic, "Multithreaded with atomics. 8 threads">
{
protected:
   decltype(std::thread::hardware_concurrency()) numCores = std::thread::hardware_concurrency();
   std::unique_ptr<ThreadRelated[]> threadsData;
   std::unique_ptr<std::future<void>[]> retValues;


public:
   MultithreadedAtomic(const decltype(std::thread::hardware_concurrency()) NCORES = 8)
   {
      if (numCores > NCORES) // agreement to do all in 8 threads
      {
         numCores = NCORES;
      }
   }
   void InitializationLogic()
   {
      threadsData.reset(new ThreadRelated[numCores - 1]);
      retValues.reset(new std::future<void>[numCores - 1]);
      COUNTT screenRange = N_CELLS / numCores;

      for (decltype(numCores) i = 0; i < numCores - 1; ++i)
      {
         threadsData[i].nFrom = i * screenRange;
         threadsData[i].nTo = (i + 1) * screenRange;

         ThreadRelated& atr = threadsData[i];
         retValues[i] = std::async(std::launch::async,
            [&atr]()
            {
               ATOMIC_ST mode = modeWorkDone;
               while (modeLeaveThread != mode)
               {
                  mode = atr.workMode.load(std::memory_order_acquire);
                  if (mode == modeDoWork)
                  {
                     atr.toWorkWith->PartialWork_Thread(&atr);
                      //TODO: add thread logic here
                     atr.workMode.store(modeWorkDone, std::memory_order_release);
                     mode = modeWorkDone;
                  }
                  else
                  {
                     // we need to do something empty but fast;
                     atr.value_to_makeSomething |= 0;
                     atr.value_to_makeSomething &= ~0;
                  }
               }
            });
      }
   }

   void SolveSudoku(const char* const toSolve, char* toSetResult)
   {
      SFIELD field(toSolve, std::span<ThreadRelated>{threadsData.get(), numCores - 1});

      field.Solve(toSetResult);
   }

   void DeinitializationLogic()
   {
      decltype(numCores) i = 0;
      for (; i < numCores - 1; ++i)
      {
         threadsData[i].workMode.store(modeLeaveThread, std::memory_order_release);
//         threadsData[i].workMode.store(modeLeaveThread, std::memory_order_relaxed); // we are not insist but it will happen in the end
      }
      for (i = 0; i < numCores - 1; ++i)
      {
         retValues[i].get();
      }
      retValues.reset();
      threadsData.reset();
   }
};



} // namespace threads_atomics
