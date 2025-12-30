#pragma once
#include <QtWidgets>

#include "Course.h"
#include "CourseSelectCard.h"
#include "StudyPlan.h"

class CourseSelector : public QVBoxLayout {
  Q_OBJECT
 public:
  CourseSelector(const std::vector<Course> &courses,
                 const StudyPlan *studyPlan);

 signals:
  void selectCourse(const Course &course);

 public slots:
  void updateCourseList();

 private:
  CourseSelectCard *selectedCourseCard;
  const StudyPlan *studyPlan;

  Period startPeriodFilter;
  unsigned int lengthFilter;
  bool examOnlyFilter;
  Period examPeriodFilter;
  std::string nameOrCodeOrTagsFilter;

  QVBoxLayout *courseList;
  QPushButton *addCourseButton;
  const std::vector<Course> &courses;
};
