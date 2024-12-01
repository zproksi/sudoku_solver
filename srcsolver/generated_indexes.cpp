#pragma once
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>
#include "generated_indexes.h"

namespace gindexes
{
constexpr const char N_OPTIONS = 9;


template<std::size_t pos>
struct GenerateLineIndexes
{
   static_assert(pos < N_CELLS, "POS must be within the range [0..80]");

   static constexpr std::size_t pos_in_line = pos % (std::size_t(N_OPTIONS));
   static constexpr std::size_t line = pos / (std::size_t(N_OPTIONS));  // Calculate which line POS belongs to
   static constexpr std::size_t start_of_line = line * (std::size_t(N_OPTIONS));  // Starting index of the line

   static constexpr std::array<std::size_t, ((std::size_t(N_OPTIONS)) - 1)> generate()
   {
      std::array<std::size_t, ((std::size_t(N_OPTIONS)) - 1)> result{};
      std::size_t result_index = 0;

      for (std::size_t i = 0; i < (std::size_t(N_OPTIONS)); ++i)
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

   static constexpr std::array<std::size_t, (N_OPTIONS - 1)> generate()
   {
      std::array<std::size_t, (N_OPTIONS - 1)> result{};
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




/// Helper structure for computing the total size of the resulting array
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

/// Function to concatenate arrays
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
   return std::array<std::array<std::size_t, 20>, sizeof...(Indices)>
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
constexpr const std::array<std::array<std::size_t, 20>, 81> indexes_for_remove_option = GenerateAllIndexes<81>();

}; // gindexes