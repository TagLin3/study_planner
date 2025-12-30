
#include "ParticipationPlan.h"

#include <stdexcept>

ParticipationPlan::ParticipationPlan(const Course& course, Period startPeriod,
                                     unsigned int yearOfStudies)
    : CoursePlan(course, startPeriod, yearOfStudies) {
  if (!this->course.startsInPeriod(startPeriod)) {
    throw std::logic_error("Course doesn't start in this period.");
  } else if (!this->course.organizedInYear(yearOfStudies)) {
    throw std::logic_error("Course isn't organized this year.");
  } else {
    this->startPeriod = startPeriod;
  }
}