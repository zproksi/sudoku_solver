#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace bs2
{

/// working with indexes only :
///   0,1,2,3,4,5,6,7,8 for '1','2','3','4','5','6','7','8','9'
/// 
constexpr const char NOTFOUND = ' ';
constexpr const char N_OPTIONS = 9;
constexpr const char N_CELLS = 81;


consteval size_t NoOptionsValue()
{
   size_t result = 0;
   for (std::size_t i = 0; i < sizeof(size_t); ++i) {
      result = (result << 8) | NOTFOUND;
   }
   return result;
}

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



template <size_t Line>
constexpr size_t LineIndex(const size_t pos)
{
   return Line * N_OPTIONS + pos;
}
// indexes for lines
constexpr const size_t line_indexes[N_OPTIONS][N_OPTIONS] =
{
     {LineIndex<0>(0), LineIndex<0>(1), LineIndex<0>(2), LineIndex<0>(3), LineIndex<0>(4), LineIndex<0>(5), LineIndex<0>(6), LineIndex<0>(7), LineIndex<0>(8)}
   , {LineIndex<1>(0), LineIndex<1>(1), LineIndex<1>(2), LineIndex<1>(3), LineIndex<1>(4), LineIndex<1>(5), LineIndex<1>(6), LineIndex<1>(7), LineIndex<1>(8)}
   , {LineIndex<2>(0), LineIndex<2>(1), LineIndex<2>(2), LineIndex<2>(3), LineIndex<2>(4), LineIndex<2>(5), LineIndex<2>(6), LineIndex<2>(7), LineIndex<2>(8)}
   , {LineIndex<3>(0), LineIndex<3>(1), LineIndex<3>(2), LineIndex<3>(3), LineIndex<3>(4), LineIndex<3>(5), LineIndex<3>(6), LineIndex<3>(7), LineIndex<3>(8)}
   , {LineIndex<4>(0), LineIndex<4>(1), LineIndex<4>(2), LineIndex<4>(3), LineIndex<4>(4), LineIndex<4>(5), LineIndex<4>(6), LineIndex<4>(7), LineIndex<4>(8)}
   , {LineIndex<5>(0), LineIndex<5>(1), LineIndex<5>(2), LineIndex<5>(3), LineIndex<5>(4), LineIndex<5>(5), LineIndex<5>(6), LineIndex<5>(7), LineIndex<5>(8)}
   , {LineIndex<6>(0), LineIndex<6>(1), LineIndex<6>(2), LineIndex<6>(3), LineIndex<6>(4), LineIndex<6>(5), LineIndex<6>(6), LineIndex<6>(7), LineIndex<6>(8)}
   , {LineIndex<7>(0), LineIndex<7>(1), LineIndex<7>(2), LineIndex<7>(3), LineIndex<7>(4), LineIndex<7>(5), LineIndex<7>(6), LineIndex<7>(7), LineIndex<7>(8)}
   , {LineIndex<8>(0), LineIndex<8>(1), LineIndex<8>(2), LineIndex<8>(3), LineIndex<8>(4), LineIndex<8>(5), LineIndex<8>(6), LineIndex<8>(7), LineIndex<8>(8)}
};

template <size_t Column>
constexpr size_t ColumnIndex(const size_t pos)
{
   return Column + N_OPTIONS * pos;
}
// indexes for columns
constexpr const size_t column_indexes[N_OPTIONS][N_OPTIONS] =
{
      {ColumnIndex<0>(0), ColumnIndex<0>(1), ColumnIndex<0>(2), ColumnIndex<0>(3), ColumnIndex<0>(4), ColumnIndex<0>(5), ColumnIndex<0>(6), ColumnIndex<0>(7), ColumnIndex<0>(8)}
   , {ColumnIndex<1>(0), ColumnIndex<1>(1), ColumnIndex<1>(2), ColumnIndex<1>(3), ColumnIndex<1>(4), ColumnIndex<1>(5), ColumnIndex<1>(6), ColumnIndex<1>(7), ColumnIndex<1>(8)}
   , {ColumnIndex<2>(0), ColumnIndex<2>(1), ColumnIndex<2>(2), ColumnIndex<2>(3), ColumnIndex<2>(4), ColumnIndex<2>(5), ColumnIndex<2>(6), ColumnIndex<2>(7), ColumnIndex<2>(8)}
   , {ColumnIndex<3>(0), ColumnIndex<3>(1), ColumnIndex<3>(2), ColumnIndex<3>(3), ColumnIndex<3>(4), ColumnIndex<3>(5), ColumnIndex<3>(6), ColumnIndex<3>(7), ColumnIndex<3>(8)}
   , {ColumnIndex<4>(0), ColumnIndex<4>(1), ColumnIndex<4>(2), ColumnIndex<4>(3), ColumnIndex<4>(4), ColumnIndex<4>(5), ColumnIndex<4>(6), ColumnIndex<4>(7), ColumnIndex<4>(8)}
   , {ColumnIndex<5>(0), ColumnIndex<5>(1), ColumnIndex<5>(2), ColumnIndex<5>(3), ColumnIndex<5>(4), ColumnIndex<5>(5), ColumnIndex<5>(6), ColumnIndex<5>(7), ColumnIndex<5>(8)}
   , {ColumnIndex<6>(0), ColumnIndex<6>(1), ColumnIndex<6>(2), ColumnIndex<6>(3), ColumnIndex<6>(4), ColumnIndex<6>(5), ColumnIndex<6>(6), ColumnIndex<6>(7), ColumnIndex<6>(8)}
   , {ColumnIndex<7>(0), ColumnIndex<7>(1), ColumnIndex<7>(2), ColumnIndex<7>(3), ColumnIndex<7>(4), ColumnIndex<7>(5), ColumnIndex<7>(6), ColumnIndex<7>(7), ColumnIndex<7>(8)}
   , {ColumnIndex<8>(0), ColumnIndex<8>(1), ColumnIndex<8>(2), ColumnIndex<8>(3), ColumnIndex<8>(4), ColumnIndex<8>(5), ColumnIndex<8>(6), ColumnIndex<8>(7), ColumnIndex<8>(8)}
};

template <size_t Square>
constexpr size_t SquareIndex(const size_t pos)
{
      return ((Square % 3) + N_OPTIONS * (Square / 3)) * 3 // base index - left top corner
         + (pos % 3) + N_OPTIONS * (pos / 3);

}
// indexes for columns
constexpr const size_t square_indexes[N_OPTIONS][N_OPTIONS] =
{
      {SquareIndex<0>(0), SquareIndex<0>(1), SquareIndex<0>(2), SquareIndex<0>(3), SquareIndex<0>(4), SquareIndex<0>(5), SquareIndex<0>(6), SquareIndex<0>(7), SquareIndex<0>(8)}
   , {SquareIndex<1>(0), SquareIndex<1>(1), SquareIndex<1>(2), SquareIndex<1>(3), SquareIndex<1>(4), SquareIndex<1>(5), SquareIndex<1>(6), SquareIndex<1>(7), SquareIndex<1>(8)}
   , {SquareIndex<2>(0), SquareIndex<2>(1), SquareIndex<2>(2), SquareIndex<2>(3), SquareIndex<2>(4), SquareIndex<2>(5), SquareIndex<2>(6), SquareIndex<2>(7), SquareIndex<2>(8)}
   , {SquareIndex<3>(0), SquareIndex<3>(1), SquareIndex<3>(2), SquareIndex<3>(3), SquareIndex<3>(4), SquareIndex<3>(5), SquareIndex<3>(6), SquareIndex<3>(7), SquareIndex<3>(8)}
   , {SquareIndex<4>(0), SquareIndex<4>(1), SquareIndex<4>(2), SquareIndex<4>(3), SquareIndex<4>(4), SquareIndex<4>(5), SquareIndex<4>(6), SquareIndex<4>(7), SquareIndex<4>(8)}
   , {SquareIndex<5>(0), SquareIndex<5>(1), SquareIndex<5>(2), SquareIndex<5>(3), SquareIndex<5>(4), SquareIndex<5>(5), SquareIndex<5>(6), SquareIndex<5>(7), SquareIndex<5>(8)}
   , {SquareIndex<6>(0), SquareIndex<6>(1), SquareIndex<6>(2), SquareIndex<6>(3), SquareIndex<6>(4), SquareIndex<6>(5), SquareIndex<6>(6), SquareIndex<6>(7), SquareIndex<6>(8)}
   , {SquareIndex<7>(0), SquareIndex<7>(1), SquareIndex<7>(2), SquareIndex<7>(3), SquareIndex<7>(4), SquareIndex<7>(5), SquareIndex<7>(6), SquareIndex<7>(7), SquareIndex<7>(8)}
   , {SquareIndex<8>(0), SquareIndex<8>(1), SquareIndex<8>(2), SquareIndex<8>(3), SquareIndex<8>(4), SquareIndex<8>(5), SquareIndex<8>(6), SquareIndex<8>(7), SquareIndex<8>(8)}
};


template<std::size_t pos>
struct GenerateLineIndexes
{
   static_assert(pos < N_CELLS, "POS must be within the range [0..80]");

   static constexpr std::size_t pos_in_line = pos % (size_t(N_OPTIONS));
   static constexpr std::size_t line = pos / (size_t(N_OPTIONS));  // Calculate which line POS belongs to
   static constexpr std::size_t start_of_line = line * (size_t(N_OPTIONS));  // Starting index of the line

   static constexpr std::array<size_t, ((size_t(N_OPTIONS)) - 1)> generate()
   {
      std::array<size_t, ((size_t(N_OPTIONS)) - 1)> result{};
      std::size_t result_index = 0;

      for (std::size_t i = 0; i < (size_t(N_OPTIONS)); ++i)
      {
         if (i != pos_in_line)
         {
            result[result_index++] = start_of_line + i;
         }
      }
      return result;
   }
};


template<std::size_t pos>
struct GenerateColumnIndexes
{
   static_assert(pos < N_CELLS, "POS must be within the range [0..80]");

   static constexpr std::size_t pos_in_column = pos / N_OPTIONS;
   static constexpr std::size_t column = pos % N_OPTIONS;  // Calculate which column POS belongs to
   static constexpr std::size_t start_of_column = column;  // Starting index of the column

   static constexpr std::array<size_t, (N_OPTIONS - 1)> generate()
   {
      std::array<size_t, (N_OPTIONS - 1)> result{};
      std::size_t result_index = 0;

      for (std::size_t i = 0; i < N_OPTIONS; ++i)
      {
         if (i != pos_in_column)
         {
            result[result_index++] = start_of_column + i * N_OPTIONS;
         }
      }
      return result;
   }
};

template<std::size_t pos>
struct GenerateSquareIndices
{
   static_assert(pos < N_CELLS, "POS must be within the range [0..80]");

   static constexpr std::size_t row = pos / N_OPTIONS;            // Row number (0-8)
   static constexpr std::size_t col = pos % N_OPTIONS;            // Column number (0-8)
   static constexpr std::size_t start_row_of_box = (row / 3) * 3; // First row of the 3x3 box
   static constexpr std::size_t start_col_of_box = (col / 3) * 3; // First column of the 3x3 box

   static constexpr std::array<std::size_t, 4> generate()
   {
      std::array<std::size_t, 4> result{};
      std::size_t index = 0;

      for (std::size_t r = start_row_of_box; r < start_row_of_box + 3; ++r)
      {
         for (std::size_t c = start_col_of_box; c < start_col_of_box + 3; ++c)
         {
            if (r != row && c != col)
            {  // Skip elements in the same row or column
               result[index++] = r * N_OPTIONS + c;
            }
         }
      }
      return result;
   }
};


// Helper structure for computing the total size of the resulting array
template<typename... Arrays>
struct TotalSize;

template<typename T, std::size_t N>
struct TotalSize<std::array<T, N>> {
   static constexpr std::size_t value = N;
};

template<typename T, std::size_t N, typename... Rest>
struct TotalSize<std::array<T, N>, Rest...> {
   static constexpr std::size_t value = N + TotalSize<Rest...>::value;
};

// Function to concatenate arrays
template<typename T, std::size_t N, typename... Arrays>
constexpr auto ConcatenateArrays(const std::array<T, N>& arr, const Arrays&... rest) {
   constexpr std::size_t total_size = TotalSize<std::array<T, N>, Arrays...>::value;
   std::array<T, total_size> result{};
   std::size_t index = 0;

   auto copy_into_result = [&index, &result](const auto& source) {
      for (const auto& item : source) {
         result[index++] = item;
      }
      };

   // Copy elements from each array into the result array
   copy_into_result(arr);
   (copy_into_result(rest), ...);

   return result;
}

/// <summary>
///   returns array of 20 indexes for remove option
/// </summary>
/// <typeparam name="POS"></typeparam>
template<std::size_t pos>
struct Concatenate3Arrays
{
   static_assert(pos < N_CELLS, "POS must be within the range [0..80]");

   static constexpr std::array<std::size_t, 8> line_indexes = GenerateLineIndexes<pos>().generate();
   static constexpr std::array<std::size_t, 8> column_indexes = GenerateColumnIndexes<pos>().generate();
   static constexpr std::array<std::size_t, 4> square_indexes = GenerateSquareIndices<pos>().generate();

   static constexpr std::array<std::size_t, 20> generate()
   {
      return ConcatenateArrays(line_indexes, column_indexes, square_indexes);
   }
};

template<std::size_t... Indices>
constexpr auto GenerateIndexArray(std::index_sequence<Indices...>)
{
   return std::array<std::array<size_t, 20>, sizeof...(Indices)>
   {
      Concatenate3Arrays<Indices>().generate()...
   };
}

template<std::size_t N>
constexpr auto GenerateAllIndexes()
{
   static_assert(N == N_CELLS, "N must be within 81");
   return GenerateIndexArray(std::make_index_sequence<N>{});
}

/// <summary>
///  here we hold indexes for clear specfic option from the field
///  in line,row,square when we set a value for cell in this line,row,square
/// </summary>
constexpr const std::array<std::array<size_t, 20>, 81> indexes_for_remove_option = GenerateAllIndexes<81>();

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
            for (size_t it : indexes_for_remove_option[i])
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
      for (size_t it : indexes_for_remove_option[index])
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
   ///  return true if there are cells inside the field with possible options and not established value
   /// </summary>
   /// <returns>false if there are cells without options and value</returns>
   bool SolutionImpossible()
   {
      static_assert(sizeof(size_t) == sizeof(SCELL::options) - 1, "We count on size of options array");
      bool ret = false;
      for (size_t k = 0; (!ret) * (k < N_CELLS); ++k)
      {
         const SCELL& cell = cells_[k];
         ret = (cell.options[N_OPTIONS - 1] == NOTFOUND) * (*reinterpret_cast<const size_t*>(cell.options) == NoOptionsValue());
      }
      return ret;
   }

   /// <summary>
   ///   return true in case it has been solved
   ///   returns false if it is impossible
   /// </summary>
   bool Solve(char* result) noexcept
   {
      size_t indexOfGuess = N_CELLS;

      for (size_t k = 0; k < N_CELLS; ++k)
      {
         const SCELL& cell = cells_[k];
         if (NOTFOUND != cell.value)
            continue;

         const size_t optionsAmount =
            (cell.options[0] == 0) + (cell.options[1] == 1) + (cell.options[2] == 2) +
            (cell.options[3] == 3) + (cell.options[4] == 4) + (cell.options[5] == 5) +
            (cell.options[6] == 6) + (cell.options[7] == 7) + (cell.options[8] == 8);

         /// no options and no value
         if (0 == optionsAmount) { return false; }

         if (1 == optionsAmount)
         {
            const size_t valueToSet = (cell.options[1] == 1) * 1 + (cell.options[2] == 2) * 2
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
         for (size_t i = 0; i < N_OPTIONS; ++i)
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
      size_t summ[27] = {0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0};

      for (size_t k = 0; k < N_CELLS; ++k)
      {
         const size_t row{k / N_OPTIONS};
         const size_t column{k % N_OPTIONS};
         const size_t square{(k / 27) * 3 + (column / 3)};
         const size_t x = cells_[k].value;
         summ[row] += x;
         summ[N_OPTIONS + column] += x;
         summ[N_OPTIONS * 2 + square] += x;
      }
      size_t valid = 1;
      for (size_t sm : summ)
      {
         valid *= (sm == 36);
      }
      return valid != 0;
   }

   /// <summary>
   ///   returns true if we have only 1 value of index
   ///       and more than 1 option for not established indexes
   /// </summary>
   bool ValidateIndexesArray(const size_t indexes[N_OPTIONS]) noexcept
   {
      size_t summ = 0;
      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         summ += cells_[indexes[k]].value;
      }
      return summ == 36;

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
