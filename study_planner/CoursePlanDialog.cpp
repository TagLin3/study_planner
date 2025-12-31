#include "CoursePlanDialog.h"

#include <string>

#include "ExamPlan.h"
#include "ParticipationPlan.h"
#include "Period.h"
using namespace std;
#include <QDebug>

CoursePlanDialog::CoursePlanDialog(const Course& course) : period(PERIOD1) {
  QVBoxLayout* dialogLayout = new QVBoxLayout();

  QLabel* yearDropDownLabel = new QLabel("Year of studies");
  QComboBox* yearDropDown = new QComboBox();
  for (int i = 1; i <= 4; i++)
    if (course.organizedInYear(i))
      yearDropDown->addItem((string("Year ") + to_string(i)).c_str(), i);

  QLabel* periodDropDownLabel = new QLabel("Start period");
  QComboBox* periodDropDown = new QComboBox();
  addPeriodsToDropDown(course.startPeriods, periodDropDown);

  QLabel* examYearDropDownLabel = new QLabel("Year of exam");
  QComboBox* examYearDropDown = new QComboBox();
  for (int i = 1; i <= 4; i++)
    if (course.organizedInYear(i) || course.examEveryYear)
      examYearDropDown->addItem((string("Year ") + to_string(i)).c_str(), i);
  examYearDropDownLabel->hide();
  examYearDropDown->hide();

  QLabel* examPeriodDropDownLabel = new QLabel("Exam period");
  QLabel* examPeriodWarning = new QLabel("WARNING: Check exam period and year");
  examPeriodWarning->setStyleSheet("QLabel { color : red }");
  QComboBox* examPeriodDropDown = new QComboBox();
  examPeriodDropDownLabel->hide();
  examPeriodWarning->hide();
  examPeriodDropDown->hide();
  addPeriodsToDropDown(course.examPeriods, examPeriodDropDown);

  QLabel* lengthDropDownLabel = new QLabel("Length");
  QComboBox* lengthDropDown = new QComboBox();
  lengthDropDownLabel->hide();
  lengthDropDown->hide();
  for (int i = 1; i <= 8; i++)
    lengthDropDown->addItem((to_string(i) + " periods").c_str(), i);

  QCheckBox* examOnlyCheckbox = new QCheckBox("Exam only");
  if (!course.examOnlyPossible) {
    examOnlyCheckbox->hide();
    this->adjustSize();
  }

  QObject::connect(
      examOnlyCheckbox, &QCheckBox::checkStateChanged, periodDropDown,
      [=, this]() -> void {
        for (int i = 1; i <= 8; i++) {
          periodDropDown->removeItem(0);
          yearDropDown->removeItem(0);
        }
        if (examOnlyCheckbox->isChecked()) {
          const vector<Period> allPeriods = {PERIOD1, PERIOD2, CHRISTMAS,
                                             PERIOD3, PERIOD4, PERIOD5,
                                             SUMMER1, SUMMER2};
          addPeriodsToDropDown(allPeriods, periodDropDown);
          for (int i = 1; i <= 4; i++)
            yearDropDown->addItem((string("Year ") + to_string(i)).c_str(), i);
          examYearDropDownLabel->show();
          examYearDropDown->show();
          examPeriodDropDownLabel->show();
          if (course.examEveryYear && course.evenOrOddYears != 0)
            examPeriodWarning->show();
          examPeriodDropDown->show();
          lengthDropDownLabel->show();
          lengthDropDown->show();
        } else {
          addPeriodsToDropDown(course.startPeriods, periodDropDown);
          for (int i = 1; i <= 4; i++)
            if (course.organizedInYear(i))
              yearDropDown->addItem((string("Year ") + to_string(i)).c_str(),
                                    i);

          examYearDropDownLabel->hide();
          examYearDropDown->hide();
          examPeriodDropDownLabel->hide();
          examPeriodWarning->hide();
          examPeriodDropDown->hide();
          lengthDropDownLabel->hide();
          lengthDropDown->hide();
          this->adjustSize();
        }
      });

  QPushButton* addCoursePlanButton = new QPushButton("Add");
  QObject::connect(addCoursePlanButton, &QPushButton::clicked, this,
                   &QDialog::accept);
  QObject::connect(this, &QDialog::accepted, this, [=, this]() {
    if (examOnlyCheckbox->isChecked()) {
      shared_ptr<CoursePlan> coursePlanToAdd = shared_ptr<ExamPlan>(
          new ExamPlan(course, lengthDropDown->currentData().toInt(),
                       periodDropDown->currentData().value<Period>(),
                       examPeriodDropDown->currentData().value<Period>(),
                       yearDropDown->currentData().toUInt(),
                       examYearDropDown->currentData().toUInt()));
      emit addCoursePlan(coursePlanToAdd);
    } else {
      shared_ptr<CoursePlan> coursePlanToAdd =
          shared_ptr<ParticipationPlan>(new ParticipationPlan(
              course, periodDropDown->currentData().value<Period>(),
              yearDropDown->currentData().toUInt()));
      emit addCoursePlan(coursePlanToAdd);
    }
  });
  dialogLayout->addWidget(yearDropDownLabel);
  dialogLayout->addWidget(yearDropDown);
  dialogLayout->addWidget(examOnlyCheckbox);
  dialogLayout->addWidget(periodDropDownLabel);
  dialogLayout->addWidget(periodDropDown);
  dialogLayout->addWidget(lengthDropDownLabel);
  dialogLayout->addWidget(lengthDropDown);
  dialogLayout->addWidget(examYearDropDownLabel);
  dialogLayout->addWidget(examYearDropDown);
  dialogLayout->addWidget(examPeriodDropDownLabel);
  dialogLayout->addWidget(examPeriodWarning);
  dialogLayout->addWidget(examPeriodDropDown);
  dialogLayout->addWidget(addCoursePlanButton);
  this->setLayout(dialogLayout);
}
