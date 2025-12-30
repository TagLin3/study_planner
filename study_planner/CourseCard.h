#pragma once
#include <QtWidgets>
#include <vector>

#include "CoursePlan.h"

class CourseCard : public QFrame {
  Q_OBJECT
 public:
  CourseCard(std::shared_ptr<CoursePlan> coursePlan, bool compactModeOn,
             QWidget* parent = nullptr);
  int column() const;

 public slots:
  void toggleCompactMode(bool compactMode);

 signals:
  void deleteCoursePlan(std::shared_ptr<CoursePlan> coursePlan);

 private:
  std::shared_ptr<CoursePlan> coursePlan;
  QGridLayout* layout;
  std::vector<QLabel*> labels;
};