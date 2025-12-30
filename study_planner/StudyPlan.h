#pragma once
#include <QDebug>

#include "CoursePlan.h"
#include "CoursePlanDialog.h"

class StudyPlan : public QObject {
  Q_OBJECT
 public:
  StudyPlan(std::vector<Course>& availableCourses)
      : availableCourses(availableCourses) {}
  std::vector<std::shared_ptr<CoursePlan>> getCoursePlans() const {
    return this->coursePlans;
  };
  double getCreditsForPeriod(unsigned int year, Period period) const;
  unsigned int getTotalCredits();
  bool courseInPlans(const Course& course) const;

 public slots:
  void startDialog(const Course& course);
  void addCoursePlan(std::shared_ptr<CoursePlan> coursePlan);
  void deleteCoursePlan(std::shared_ptr<CoursePlan> coursePlan);
  void loadStudyPlan(QWidget* fileDialogParent);
  void saveStudyPlan(QWidget* fileDialogParent);

 signals:
  void studyPlanChanged();

 private:
  std::vector<std::shared_ptr<CoursePlan>> coursePlans;
  std::vector<Course>& availableCourses;
};