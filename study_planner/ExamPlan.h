#pragma once
#include "Course.h"
#include "CoursePlan.h"

class ExamPlan : public CoursePlan {
 public:
  ExamPlan(const Course& course, int length, Period startPeriod,
           Period examPeriod, unsigned int yearOfStudies,
           unsigned int examYear);
  int getPlanLength() const { return length; };
  bool isExamPlan() const { return true; };
  Period getExamPeriod() const { return examPeriod; };
  unsigned int getExamYear() const { return examYear; };

 private:
  int length;
  Period examPeriod;
  unsigned int examYear;
};