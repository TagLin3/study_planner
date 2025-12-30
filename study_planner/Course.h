#pragma once
#include <QMetaType>
#include <QVariant>
#include <string>
#include <vector>

#include "Period.h"
struct Course {
  Course(const std::string& code, const std::string& name, int credits,
         int length, const std::vector<Period>& startPeriods,
         bool examOnlyPossible, const std::vector<Period>& examPeriods,
         int evenOrOddYears, bool examEveryYear,
         const std::vector<std::string>& tags);
  Course()
      : credits(0),
        length(0),
        examOnlyPossible(false),
        evenOrOddYears(0),
        examEveryYear(true) {};
  bool startsInPeriod(Period period) const;
  bool examInPeriod(Period period) const;
  bool organizedInYear(int year) const;
  bool nameOrCodeOrTagsContains(std::string keyWord) const;
  const std::string code;
  const std::string name;
  const std::vector<std::string> tags;
  const unsigned int credits;
  const unsigned int length;
  const std::vector<Period> startPeriods;
  const bool examOnlyPossible;
  const std::vector<Period> examPeriods;
  const int evenOrOddYears;
  bool examEveryYear;
};