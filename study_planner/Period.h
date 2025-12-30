#pragma once
#include <QtWidgets>
#include <string>
#include <vector>

enum Period : unsigned int {
  PERIOD1 = 1,
  PERIOD2 = 2,
  CHRISTMAS = 3,
  PERIOD3 = 4,
  PERIOD4 = 5,
  PERIOD5 = 6,
  SUMMER1 = 7,
  SUMMER2 = 8
};

std::string periodToString(const Period& period);

void addPeriodsToDropDown(const std::vector<Period>& periods,
                          QComboBox* dropDown);

inline Period& cyclePeriod(Period& period) {
  int value = (int)period;
  value++;
  if (value > SUMMER2) {
    value = PERIOD1;
  }
  return period = (Period)value;
};