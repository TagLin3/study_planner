#pragma once
#include <QtWidgets>

#include "StudyPlan.h"

class CourseTimeLine : public QGridLayout {
  Q_OBJECT
 public:
  CourseTimeLine(const StudyPlan *);

 public slots:
  void fitCourses();
  void compactModeStateChanged(Qt::CheckState checkState);

 signals:
  void toggleCompactMode(bool state);

 private:
  const StudyPlan *studyPlan;
  bool compactModeOn;
};