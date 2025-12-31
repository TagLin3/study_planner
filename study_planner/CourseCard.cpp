#include "CourseCard.h"

#include <QDebug>
#include <string>

#include "CoursePlan.h"

using namespace std;

CourseCard::CourseCard(std::shared_ptr<CoursePlan> coursePlan,
                       bool compactModeOn, QWidget* parent)
    : QFrame(parent), coursePlan(coursePlan) {
  this->coursePlan->isExamPlan() ? this->setObjectName("examCard")
                                 : this->setObjectName("courseCard");

  this->layout = new QGridLayout(this);

  QVBoxLayout* textLayout = new QVBoxLayout();
  QLabel* label1 = new QLabel(coursePlan->getCourseCode().c_str());
  QLabel* label2 = new QLabel(coursePlan->getCourseName().c_str());
  label2->setStyleSheet("font-size: 10px");
  QLabel* label3 =
      new QLabel(to_string(coursePlan->getCourseCredits()).c_str());
  this->labels = {label1, label2, label3};
  for (vector<QLabel*>::const_iterator i = this->labels.begin();
       i != this->labels.end(); i++) {
    textLayout->addWidget(*i);
    textLayout->setAlignment(*i, Qt::AlignHCenter);
    (*i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  }

  this->layout->addLayout(textLayout, 0, 0, Qt::AlignTop);

  QPushButton* deleteButton =
      new QPushButton(QIcon("../data/delete_button_icon.png"), "");
  deleteButton->setStyleSheet("border: none");
  deleteButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  this->layout->addWidget(deleteButton, 0, 0, Qt::AlignTop | Qt::AlignLeft);
  QObject::connect(
      deleteButton, &QPushButton::clicked, this,
      [=, this]() -> void { emit this->deleteCoursePlan(this->coursePlan); });

  this->setLayout(this->layout);

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  toggleCompactMode(compactModeOn);
}

int CourseCard::column() const {
  return this->coursePlan->getStartPeriod() +
         (this->coursePlan->getYearOfStudies() - 1) * 8;
}

void CourseCard::toggleCompactMode(bool compactMode) {
  this->layout->setSpacing(compactMode ? 1 : 5);
  this->layout->setContentsMargins(0, compactMode ? 0 : 3, 0,
                                   compactMode ? 0 : 3);
  for (vector<QLabel*>::const_iterator i = this->labels.begin();
       i != this->labels.end(); i++) {
    if (compactMode) {
      (*i)->setStyleSheet(compactMode ? "font-size: 10px" : "font-size: 12px");
    }
  }
  labels[1]->setStyleSheet(compactMode ? "font-size: 8px" : "font-size: 10px");
}