#include "ExamPlan.h"

#include <stdexcept>

ExamPlan::ExamPlan(const Course& course, int length, Period startPeriod,
                   Period examPeriod, unsigned int yearOfStudies,
                   unsigned int examYear)
    : CoursePlan(course, startPeriod, yearOfStudies),
      examPeriod(examPeriod),
      length(length),
      examYear(examYear) {
  if (!this->course.examOnlyPossible) {
    throw std::logic_error("Exam only isn't possible for this course.");
  } else if (!this->course.examInPeriod(examPeriod)) {
    throw std::logic_error("No exam for this course in selected period.");
  } else if (!this->course.organizedInYear(examYear) &&
             !this->course.examEveryYear) {
    throw std::logic_error("No exam for this course in selected year.");
  } else {
  }
}