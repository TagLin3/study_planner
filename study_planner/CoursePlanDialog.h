#pragma once
#include <QtWidgets>

#include "Course.h"
#include "CoursePlan.h"
#include "Period.h"
#include "StudyPlan.h"

class CoursePlanDialog : public QDialog {
  Q_OBJECT
 public:
  CoursePlanDialog(const Course& course);

 signals:
  void addCoursePlan(std::shared_ptr<CoursePlan> coursePlan);

 private:
  Period period;
};