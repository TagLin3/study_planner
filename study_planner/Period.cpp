#include "Period.h"

std::string periodToString(const Period& period) {
  switch (period) {
    case PERIOD1:
      return "Period 1";
    case PERIOD2:
      return "Period 2";
    case CHRISTMAS:
      return "Christmas break";
    case PERIOD3:
      return "Period 3";
    case PERIOD4:
      return "Period 4";
    case PERIOD5:
      return "Period 5";
    case SUMMER1:
      return "Summer 1";
    case SUMMER2:
      return "Summer 2";
  }
  return "";
};

void addPeriodsToDropDown(const std::vector<Period>& periods,
                          QComboBox* dropDown) {
  for (std::vector<Period>::const_iterator i = periods.begin();
       i != periods.end(); i++) {
    std::string title = periodToString(*i);
    dropDown->addItem(title.c_str(), *i);
  }
}
