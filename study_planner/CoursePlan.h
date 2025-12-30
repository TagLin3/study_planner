#pragma once
#include "Course.h"

class CoursePlan {
 public:
  CoursePlan(const Course& course, Period startPeriod,
             unsigned int yearOfStudies)
      : course(course),
        startPeriod(startPeriod),
        yearOfStudies(yearOfStudies) {};
  Period getStartPeriod() const { return startPeriod; };
  unsigned int getYearOfStudies() const { return yearOfStudies; };
  std::string getCourseName() const { return course.name; };
  std::string getCourseCode() const { return course.code; };
  int getCourseCredits() const { return course.credits; };
  virtual int getPlanLength() const = 0;
  virtual bool isExamPlan() const = 0;
  virtual Period getExamPeriod() const = 0;
  virtual unsigned int getExamYear() const = 0;

 protected:
  const Course& course;
  Period startPeriod;
  unsigned int yearOfStudies;
};