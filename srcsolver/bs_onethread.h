#pragma once

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
   char value;// = NOTFOUND;
   char options[N_OPTIONS];// = {0, 1, 2, 3, 4, 5, 6, 7, 8};

   /// <summary>
   ///   set option value to value or NOTFOUND
   /// </summary>
   /// <param name="c">to set This Index at this index if v is Equal to this index</param>
   /// <param name="v">to set this index if this index came inside</param>
   constexpr void OptionValue(const char c, const char v) noexcept
   {
      const bool b = c == v;
      options[c] = b * c + NOTFOUND *(!b);
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

   constexpr void ClearOption(const char v) noexcept
   {// clear only if not established to this value
      const bool b = v == value;
      options[v] = v * (b) + NOTFOUND * (!b);
   }

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
        {LineIndex<0>(1), LineIndex<0>(1), LineIndex<0>(2), LineIndex<0>(3), LineIndex<0>(4), LineIndex<0>(5), LineIndex<0>(6), LineIndex<0>(7), LineIndex<0>(8)}
      , {LineIndex<1>(1), LineIndex<1>(1), LineIndex<1>(2), LineIndex<1>(3), LineIndex<1>(4), LineIndex<1>(5), LineIndex<1>(6), LineIndex<1>(7), LineIndex<1>(8)}
      , {LineIndex<2>(1), LineIndex<2>(1), LineIndex<2>(2), LineIndex<2>(3), LineIndex<2>(4), LineIndex<2>(5), LineIndex<2>(6), LineIndex<2>(7), LineIndex<2>(8)}
      , {LineIndex<3>(1), LineIndex<3>(1), LineIndex<3>(2), LineIndex<3>(3), LineIndex<3>(4), LineIndex<3>(5), LineIndex<3>(6), LineIndex<3>(7), LineIndex<3>(8)}
      , {LineIndex<4>(1), LineIndex<4>(1), LineIndex<4>(2), LineIndex<4>(3), LineIndex<4>(4), LineIndex<4>(5), LineIndex<4>(6), LineIndex<4>(7), LineIndex<4>(8)}
      , {LineIndex<5>(1), LineIndex<5>(1), LineIndex<5>(2), LineIndex<5>(3), LineIndex<5>(4), LineIndex<5>(5), LineIndex<5>(6), LineIndex<5>(7), LineIndex<5>(8)}
      , {LineIndex<6>(1), LineIndex<6>(1), LineIndex<6>(2), LineIndex<6>(3), LineIndex<6>(4), LineIndex<6>(5), LineIndex<6>(6), LineIndex<6>(7), LineIndex<6>(8)}
      , {LineIndex<7>(1), LineIndex<7>(1), LineIndex<7>(2), LineIndex<7>(3), LineIndex<7>(4), LineIndex<7>(5), LineIndex<7>(6), LineIndex<7>(7), LineIndex<7>(8)}
      , {LineIndex<8>(1), LineIndex<8>(1), LineIndex<8>(2), LineIndex<8>(3), LineIndex<8>(4), LineIndex<8>(5), LineIndex<8>(6), LineIndex<8>(7), LineIndex<8>(8)}
   };

   template <size_t Column>
   constexpr size_t ColumnIndex(const size_t pos)
   {
      return Column + N_OPTIONS * pos;
   }
   // indexes for columns
   constexpr const size_t column_indexes[N_OPTIONS][N_OPTIONS] =
   {
        {ColumnIndex<0>(1), ColumnIndex<0>(1), ColumnIndex<0>(2), ColumnIndex<0>(3), ColumnIndex<0>(4), ColumnIndex<0>(5), ColumnIndex<0>(6), ColumnIndex<0>(7), ColumnIndex<0>(8)}
      , {ColumnIndex<1>(1), ColumnIndex<1>(1), ColumnIndex<1>(2), ColumnIndex<1>(3), ColumnIndex<1>(4), ColumnIndex<1>(5), ColumnIndex<1>(6), ColumnIndex<1>(7), ColumnIndex<1>(8)}
      , {ColumnIndex<2>(1), ColumnIndex<2>(1), ColumnIndex<2>(2), ColumnIndex<2>(3), ColumnIndex<2>(4), ColumnIndex<2>(5), ColumnIndex<2>(6), ColumnIndex<2>(7), ColumnIndex<2>(8)}
      , {ColumnIndex<3>(1), ColumnIndex<3>(1), ColumnIndex<3>(2), ColumnIndex<3>(3), ColumnIndex<3>(4), ColumnIndex<3>(5), ColumnIndex<3>(6), ColumnIndex<3>(7), ColumnIndex<3>(8)}
      , {ColumnIndex<4>(1), ColumnIndex<4>(1), ColumnIndex<4>(2), ColumnIndex<4>(3), ColumnIndex<4>(4), ColumnIndex<4>(5), ColumnIndex<4>(6), ColumnIndex<4>(7), ColumnIndex<4>(8)}
      , {ColumnIndex<5>(1), ColumnIndex<5>(1), ColumnIndex<5>(2), ColumnIndex<5>(3), ColumnIndex<5>(4), ColumnIndex<5>(5), ColumnIndex<5>(6), ColumnIndex<5>(7), ColumnIndex<5>(8)}
      , {ColumnIndex<6>(1), ColumnIndex<6>(1), ColumnIndex<6>(2), ColumnIndex<6>(3), ColumnIndex<6>(4), ColumnIndex<6>(5), ColumnIndex<6>(6), ColumnIndex<6>(7), ColumnIndex<6>(8)}
      , {ColumnIndex<7>(1), ColumnIndex<7>(1), ColumnIndex<7>(2), ColumnIndex<7>(3), ColumnIndex<7>(4), ColumnIndex<7>(5), ColumnIndex<7>(6), ColumnIndex<7>(7), ColumnIndex<7>(8)}
      , {ColumnIndex<8>(1), ColumnIndex<8>(1), ColumnIndex<8>(2), ColumnIndex<8>(3), ColumnIndex<8>(4), ColumnIndex<8>(5), ColumnIndex<8>(6), ColumnIndex<8>(7), ColumnIndex<8>(8)}
   };
}

struct SFIELD final
{
   SCELL cells_[N_CELLS];

   // constructs field
   SFIELD(const char* const src) noexcept
   {
      for (size_t i = 0; i < N_CELLS; ++i)
      {
         const bool established = cells_[i].SetValue(src[i]);
      }
   }

   /// <summary>
   ///   Remove options for the line, column and square
   /// </summary>
   void FreeOptions(size_t const i) noexcept
   {
   }

};



bool SolveSudoku(const char* const src, char* result)
{
   return true;
}






} // namespac bs
