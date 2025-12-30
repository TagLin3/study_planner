#pragma once
#include <QtWidgets>

#include "Course.h"

class CourseSelectCard : public QFrame {
  Q_OBJECT
 public:
  CourseSelectCard(const Course& course, QWidget* parent = nullptr);
  QSize sizeHint() const { return QSize(300, 50); };
  void mousePressEvent(QMouseEvent* event) { emit clicked(); };
  const Course& getCourse() const { return course; };

 signals:
  void clicked();

 private:
  const Course& course;
};