#include "stdafx.h"
#include "report_generator.h"
#include "sudoku_test_data.h"
#include "timemeasurer.h"

namespace sudoku_solver
{

///@brief writes the result data to file
Report_Generator::Report_Generator()
{
}

void Report_Generator::LogEvent(const std::string_view aDescription, const long long usedTimeNanoseconds)
{
   results_.emplace_back(MeasurementResult{aDescription, usedTimeNanoseconds});
}

///@brief writes the result data to file
Report_Generator::~Report_Generator()
{
   std::sort(results_.begin(), results_.end(), [](MeasurementResult& l, MeasurementResult& r)
      {
         return l.usedTimeNanoseconds_ < r.usedTimeNanoseconds_;
      });

   std::ofstream ofs;
   ofs.open("sudosore.html", std::ofstream::out | std::ofstream::trunc);

   ofs << "<html><head><title>Sudoku Solver Results</title></head><body>";
   ofs << "<br/>The unique set of sudoku to solve contains <B>" << sudoku_test_data::nSudokuToSolve << "</B> items<br/>";
   ofs << "Each set being solved <B>" << sudoku_test_data::RUNS_AMOUNT << "</B> times<br/>";

   const long long nMax = results_.rbegin()->usedTimeNanoseconds_;

   for (std::vector<MeasurementResult>::const_iterator cit = results_.cbegin(); cit != results_.cend(); ++cit)
   {
      const MeasurementResult& mr = *cit;

      long long percentage = mr.usedTimeNanoseconds_ * ((long long)100) / nMax;
      if (percentage > 100)
         percentage = 100;
      const long long reminder = 100 - percentage;
      char colorArr[7] = {'9', '0', '0', '0', '0', '9', '\0'};
      colorArr[0] -= static_cast<char>(reminder / 10);
      colorArr[1] = colorArr[0];
      colorArr[2] += static_cast<char>(reminder / 10);
      colorArr[3] = colorArr[0];

      ofs << std::format(R"(</br>{} (It takes <B>{} ns</B> for {} sudoku)</br><B>{}</B> nanoseconds for 1 sudoku in average</br>
         <table border="1" width="100%">
            <tr><td width="{}%" bgcolor=#{}>&nbsp</td><td width="{}%" bgcolor="white"/></tr>
         </table></br>)", mr.description_, // description
                           TimeMeasurer::FormatNanoseconds(mr.usedTimeNanoseconds_), sudoku_test_data::nSudokuToSolve,
                           TimeMeasurer::FormatNanoseconds(mr.usedTimeNanoseconds_ / sudoku_test_data::nSudokuToSolve),
                           percentage, colorArr, reminder); // percentage, color, reminder
   }


   ofs << "</body></html>";
   ofs.close();

}

};// sudoku_solver namespace
