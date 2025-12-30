#include "CourseSelectCard.h"

using namespace std;

CourseSelectCard::CourseSelectCard(const Course& course, QWidget* parent)
    : QFrame(parent), course(course) {
  this->setObjectName("courseSelectCard");
  this->setProperty("selected", false);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(3, 3, 3, 3);

  QLabel* label1 = new QLabel(course.code.c_str());
  QLabel* label2 = new QLabel(course.name.c_str());
  label2->setStyleSheet("font-size: 10px");
  QLabel* label3 = new QLabel(to_string(course.credits).c_str());
  vector<QLabel*> labels = {label1, label2, label3};
  for (vector<QLabel*>::const_iterator i = labels.begin(); i != labels.end();
       i++) {
    layout->addWidget(*i);
    layout->setAlignment(*i, Qt::AlignHCenter);
    (*i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  }

  this->setLayout(layout);

  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
