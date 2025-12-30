#include "Course.h"

#include <QDebug>
#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std;

Course::Course(const string& code, const string& name, int credits, int length,
               const vector<Period>& startPeriods, bool examOnlyPossible,
               const vector<Period>& examPeriods, int evenOrOddYears,
               bool examEveryYear, const vector<string>& tags)
    : code(code),
      name(name),
      credits(credits),
      length(length),
      startPeriods(startPeriods),
      examOnlyPossible(examOnlyPossible),
      examPeriods(examPeriods),
      evenOrOddYears(evenOrOddYears),
      examEveryYear(examEveryYear),
      tags(tags) {
  if (examOnlyPossible == examPeriods.empty())
    throw logic_error(
        "Examperiods should be empty if and only if exam isn't possible.");
}

bool Course::organizedInYear(int year) const {
  switch (this->evenOrOddYears) {
    case 1:
      return year % 2 == 0;
    case -1:
      return year % 2 == 1;
  }
  return true;
}

bool Course::nameOrCodeOrTagsContains(string keyWord) const {
  transform(keyWord.begin(), keyWord.end(), keyWord.begin(),
            [](unsigned char c) { return tolower(c); });
  string codeInLower = string(this->code);
  string nameInLower = string(this->name);
  transform(this->code.begin(), this->code.end(), codeInLower.begin(),
            [](unsigned char c) { return tolower(c); });
  transform(this->name.begin(), this->name.end(), nameInLower.begin(),
            [](unsigned char c) { return tolower(c); });
  vector<string> tagsInLower = vector<string>(this->tags);
  transform(this->tags.begin(), this->tags.end(), tagsInLower.begin(),
            [](string tag) {
              string tagInLower = string(tag);
              transform(tag.begin(), tag.end(), tagInLower.begin(),
                        [](unsigned char c) { return tolower(c); });
              return tagInLower;
            });
  const bool foundInTags =
      find_if(tagsInLower.begin(), tagsInLower.end(), [=](string tagInLower) {
        return tagInLower.find(keyWord) != string::npos;
      }) != tagsInLower.end();
  return codeInLower.find(keyWord) != string::npos ||
         nameInLower.find(keyWord) != string::npos || foundInTags;
}

bool Course::startsInPeriod(Period period) const {
  return find(startPeriods.begin(), startPeriods.end(), period) !=
         startPeriods.end();
}

bool Course::examInPeriod(Period period) const {
  return find(examPeriods.begin(), examPeriods.end(), period) !=
         examPeriods.end();
}