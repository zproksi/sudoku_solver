#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace bs
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
   constexpr void inline OptionValue(const char c, const char v) noexcept
   {
      const bool b = c == v;
      options[c] = b * c + NOTFOUND * (!b);
   }

   /// only this method acceps characters
   /// returns if the value was established
   constexpr bool SetValue(const char c) noexcept
   {
      const bool b = c >= '1' && c <='9';
      value = b * (c - '1') + (!b) * NOTFOUND;
      OptionValue(0, value); OptionValue(1, value); OptionValue(2, value);
      OptionValue(3, value); OptionValue(4, value); OptionValue(5, value);
      OptionValue(6, value); OptionValue(7, value); OptionValue(8, value);
      return b;
   }

   /// set cell value to this character without any check
   constexpr void inline SetCellTo(const char v) noexcept
   {
      OptionValue(0, v); OptionValue(1, v); OptionValue(2, v);
      OptionValue(3, v); OptionValue(4, v); OptionValue(5, v);
      OptionValue(6, v); OptionValue(7, v); OptionValue(8, v);
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



namespace
{

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
   template<std::size_t POS>
   struct Concatenate3Arrays
   {
      static constexpr std::array<std::size_t, 8> line_indexes = GenerateLineIndexes<POS>().generate();
      static constexpr std::array<std::size_t, 8> column_indexes = GenerateColumnIndexes<POS>().generate();
      static constexpr std::array<std::size_t, 4> square_indexes = GenerateSquareIndices<POS>().generate();

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
      return GenerateIndexArray(std::make_index_sequence<N>{});
   }

   /// <summary>
   ///  here we hold indexes for clear specfic option from the field
   ///  in line,row,square when we set a value for cell in this line,row,square
   /// </summary>
   constexpr const std::array<std::array<size_t, 20>, 81> indexes_for_remove_option = GenerateAllIndexes<81>();

} // nameless namespace

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
      size_t amounts[N_OPTIONS] = {0}; // how many met  of 0,1,2,..     9+ if this index found
      size_t indexAt[N_OPTIONS];       // where we met this option

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
         const bool established = cell.value != NOTFOUND;
         amounts[at % N_OPTIONS] += established * N_OPTIONS; // add 9 if established

         amounts[0] += cell.options[0] == 0; indexAt[0] = at * (amounts[0] == 1);
         amounts[1] += cell.options[1] == 1; indexAt[1] = at * (amounts[1] == 1);
         amounts[2] += cell.options[2] == 2; indexAt[2] = at * (amounts[2] == 1);
         amounts[3] += cell.options[3] == 3; indexAt[3] = at * (amounts[3] == 1);
         amounts[4] += cell.options[4] == 4; indexAt[4] = at * (amounts[4] == 1);
         amounts[5] += cell.options[5] == 5; indexAt[5] = at * (amounts[5] == 1);
         amounts[6] += cell.options[6] == 6; indexAt[6] = at * (amounts[6] == 1);
         amounts[7] += cell.options[7] == 7; indexAt[7] = at * (amounts[7] == 1);
         amounts[8] += cell.options[8] == 8; indexAt[8] = at * (amounts[8] == 1);
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
         foundN = 0;
         foundN += CheckIndexesArray(line_indexes[0]);
         foundN += CheckIndexesArray(line_indexes[1]);
         foundN += CheckIndexesArray(line_indexes[2]);
         foundN += CheckIndexesArray(line_indexes[3]);
         foundN += CheckIndexesArray(line_indexes[4]);
         foundN += CheckIndexesArray(line_indexes[5]);
         foundN += CheckIndexesArray(line_indexes[6]);
         foundN += CheckIndexesArray(line_indexes[7]);
         foundN += CheckIndexesArray(line_indexes[8]);
         foundN += CheckIndexesArray(column_indexes[0]);
         foundN += CheckIndexesArray(column_indexes[1]);
         foundN += CheckIndexesArray(column_indexes[2]);
         foundN += CheckIndexesArray(column_indexes[3]);
         foundN += CheckIndexesArray(column_indexes[4]);
         foundN += CheckIndexesArray(column_indexes[5]);
         foundN += CheckIndexesArray(column_indexes[6]);
         foundN += CheckIndexesArray(column_indexes[7]);
         foundN += CheckIndexesArray(column_indexes[8]);
         foundN += CheckIndexesArray(square_indexes[0]);
         foundN += CheckIndexesArray(square_indexes[1]);
         foundN += CheckIndexesArray(square_indexes[2]);
         foundN += CheckIndexesArray(square_indexes[3]);
         foundN += CheckIndexesArray(square_indexes[4]);
         foundN += CheckIndexesArray(square_indexes[5]);
         foundN += CheckIndexesArray(square_indexes[6]);
         foundN += CheckIndexesArray(square_indexes[7]);
         foundN += CheckIndexesArray(square_indexes[8]);
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
               for (size_t i = 0; i < N_OPTIONS; ++i)
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
      size_t amounts[N_OPTIONS] = {0};

      for (size_t k = 0; k < N_OPTIONS; ++k)
      {
         const size_t at = indexes[k];
         const SCELL& cell = cells_[at];
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

      return N_OPTIONS == (
         (amounts[0] > 0) + (amounts[1] > 0) + (amounts[2] > 0) +
         (amounts[3] > 0) + (amounts[4] > 0) + (amounts[5] > 0) +
         (amounts[6] > 0) + (amounts[7] > 0) + (amounts[8] > 0) );

   }
};



bool SolveSudoku(const char* const src, char* result)
{
   SFIELD field(src);

   return field.Solve(result);
}






} // namespac bs
