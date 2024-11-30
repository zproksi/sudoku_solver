// #include "report_generator.h"
#pragma once
#include <string_view>
#include <vector>

namespace sudoku_solver
{

/// @brief Report_Generator class generates html file with provided results
///   it builds colored html table with proportions for timing
class Report_Generator final
{
   Report_Generator(const Report_Generator&) = delete;
   Report_Generator(Report_Generator&&) = delete;
   Report_Generator& operator =(const Report_Generator&) = delete;
   Report_Generator&& operator =(Report_Generator&&) = delete;

   struct MeasurementResult
   {
      std::string_view description_;
      long long usedTimeNanoseconds_;
   };

public:
   ///@brief writes the result data to file
    Report_Generator();

    void LogEvent(const std::string_view aDescription, const long long usedTimeNanoseconds);

    ///@brief writes the result data to file
    ~Report_Generator();
protected:

   /// <summary>
   /// @brief holding measurement results here
   /// </summary>
   std::vector<MeasurementResult> results_;

};

};
