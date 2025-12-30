#pragma once
#include "CoursePlan.h"

class ParticipationPlan : public CoursePlan {
 public:
  ParticipationPlan(const Course& course, Period startPeriod,
                    unsigned int yearOfStudies);
  int getPlanLength() const { return course.length; };
  bool isExamPlan() const { return false; };
  unsigned int getExamYear() const { return yearOfStudies; };
  Period getExamPeriod() const { return startPeriod; };
};